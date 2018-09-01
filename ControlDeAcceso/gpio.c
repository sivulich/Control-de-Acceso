/*
 * gpio.c
 *
 *  Created on: Aug 17, 2018
 *      Author: Santi
 */

#include "gpio.h"
#define NUMBER_OF_PORTS 5
#define NUMBER_OF_PINS 32
#ifndef _WIN32
#include "hardware.h"


typedef enum
{
	PORT_eDisabled = 0x00,
	PORT_eDMARising = 0x01,
	PORT_eDMAFalling = 0x02,
	PORT_eDMAEither = 0x03,
	PORT_eInterruptDisasserted = 0x08,
	PORT_eInterruptRising = 0x09,
	PORT_eInterruptFalling = 0x0A,
	PORT_eInterruptEither = 0x0B,
	PORT_eInterruptAsserted = 0x0C,
} PORTEvent_t;

GPIO_Type*  gpioLU[NUMBER_OF_PORTS] = GPIO_BASE_PTRS;
PORT_Type*  portLU[NUMBER_OF_PORTS] = PORT_BASE_PTRS;
PORTEvent_t mapInterruptConfig[GPIO_IRQ_CANT_MODES] = {
	PORT_eDisabled,
	PORT_eInterruptRising,
	PORT_eInterruptFalling,
	PORT_eInterruptEither,
};
uint8_t mapIntPortEnable[NUMBER_OF_PORTS] = {
	PORTA_IRQn,
	PORTB_IRQn,
	PORTC_IRQn,
	PORTD_IRQn,
	PORTE_IRQn
};

pinIrqFun_t  interruptFun[NUMBER_OF_PORTS][NUMBER_OF_PINS];
void IRQFuncSel(uint8_t port);

void pinMode(uint8_t pin, uint8_t mode)
{
	int port = pin >> 5;

	int portPin = pin % 32;

	GPIO_Type* ptr = gpioLU[port];
	PORT_Type* pp = portLU[port];
	if (mode == INPUT || mode == OUTPUT)
	{
		ptr->PDDR = ((ptr->PDDR & ~(1 << portPin)) | mode << portPin);
		pp->PCR[portPin] = (pp->PCR[portPin] & ~(PORT_PCR_MUX_MASK)) | PORT_PCR_MUX(1);
		pp->PCR[portPin] = (pp->PCR[portPin] & ~(PORT_PCR_ODE_MASK)) | PORT_PCR_ODE(0);
		pp->PCR[portPin] = (pp->PCR[portPin] & ~(PORT_PCR_PE_MASK)) | PORT_PCR_PE(0);
	}
	else if (mode == INPUT_PULLUP || mode == INPUT_PULLDOWN)
	{
		ptr->PDDR = ((ptr->PDDR & ~(1 << portPin)) | INPUT << portPin);
		pp->PCR[portPin] = (pp->PCR[portPin] & ~(PORT_PCR_MUX_MASK)) | PORT_PCR_MUX(1);
		pp->PCR[portPin] = (pp->PCR[portPin] & ~(PORT_PCR_ODE_MASK)) | PORT_PCR_ODE(0);
		pp->PCR[portPin] = (pp->PCR[portPin] & ~(PORT_PCR_PE_MASK)) | PORT_PCR_PE(1);
		pp->PCR[portPin] = (pp->PCR[portPin] & ~(PORT_PCR_PS_MASK)) | PORT_PCR_PS(!(mode % 2));
	}
}
uint8_t pinConfigureIRQ(uint8_t pin, uint8_t irqMode, pinIrqFun_t irqFun)
{
	int port = pin >> 5;
	int portPin = pin % 32;
	PORT_Type* pp = portLU[port];
	PORTEvent_t irConfig = mapInterruptConfig[irqMode];
	pp->PCR[portPin] = (pp->PCR[portPin] & ~(PORT_PCR_IRQC_MASK)) | PORT_PCR_IRQC(irConfig); //Se guarda configuración del interrupt
	NVIC_EnableIRQ(mapIntPortEnable[port]);
	interruptFun[port][portPin] = irqFun; //Guardamos la función a llamar para el interrupt del pin
	return 0;
}

void digitalWrite(uint8_t pin, uint8_t value)
{
	int port = pin >> 5;

	int portPin = pin % 32;

	GPIO_Type* ptr = gpioLU[port];

	ptr->PDOR = ((ptr->PDOR & ~(1 << portPin)) | value << portPin);
}
void digitalToggle(uint8_t pin)
{
	int port = pin >> 5;

	int portPin = pin % 32;

	GPIO_Type* ptr = gpioLU[port];

	ptr->PDOR ^= 1 << portPin;
}
uint8_t digitalRead(uint8_t pin)
{
	int port = pin >> 5;

	int portPin = pin % 32;
	GPIO_Type* ptr = gpioLU[port];

	return (ptr->PDIR & (1 << portPin)) && 1;
}

__ISR__ PORTA_IRQHandler(void)
{
	IRQFuncSel(0);
}
__ISR__ PORTB_IRQHandler(void)
{
	IRQFuncSel(1);
}
__ISR__ PORTC_IRQHandler(void)
{
	IRQFuncSel(2);
}
__ISR__ PORTD_IRQHandler(void)
{
	IRQFuncSel(3);
}
__ISR__ PORTE_IRQHandler(void)
{
	IRQFuncSel(4);
}
void IRQFuncSel(uint8_t port)
{
	PORT_Type* pp = portLU[port];
	for (unsigned int pin = 0; pin<NUMBER_OF_PINS; pin++)
	{
		if ((pp->PCR[pin] & PORT_PCR_ISF_MASK) != 0) //Cuando se encuentra el pin
		{
			pp->PCR[pin] |= PORT_PCR_ISF_MASK; // Se apaga el ISF
			interruptFun[port][pin]();
			break;
		}
	}
}
#else
#include <stdio.h>
//#define GPIO_DEBUG
static char ports[] = { 'A','B','C','D','E','F' };
uint8_t digitalRead(uint8_t pin)
{
	int port = pin >> 5;

	int portPin = pin % 32;
#ifdef GPIO_DEBUG
	printf("Reading port %c pin %d\n", ports[port], portPin);
#endif
	return 0;
}
void pinMode(uint8_t pin, uint8_t mode)
{
	int port = pin >> 5;

	int portPin = pin % 32;
#ifdef GPIO_DEBUG
	printf("Setting port %c pin %d, mode = %d\n", ports[port], portPin, mode);
#endif
}
void digitalWrite(uint8_t pin, uint8_t value)
{
	int port = pin >> 5;

	int portPin = pin % 32;
#ifdef GPIO_DEBUG
	printf("Writing port %c pin %d, value = %d\n", ports[port], portPin, value);
#endif
}
void digitalToggle(uint8_t pin)
{
	int port = pin >> 5;

	int portPin = pin % 32;
#ifdef GPIO_DEBUG
	printf("Toggling port %c pin %d\n", ports[port], portPin);
#endif
}
uint8_t pinConfigureIRQ(uint8_t pin, uint8_t irqMode, pinIrqFun_t irqFun)
{
	int port = pin >> 5;

	int portPin = pin % 32;
#ifdef GPIO_DEBUG
	printf("Setting interrupt for port %c pin %d\n", ports[port], portPin);
#endif
}

#endif // !_WIN32



