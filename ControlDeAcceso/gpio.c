/*
 * gpio.c
 *
 *  Created on: Aug 17, 2018
 *      Author: Santi
 */

#include "gpio.h"
#ifdef _WIN32 
#include <stdio.h>
//#define GPIO_DEBUG
static char ports[] = { 'A','B','C','D','E','F' };
uint8_t digitalRead(uint8_t pin)
{
	int port = pin >> 5;

	int portPin = pin % 32;
#ifdef GPIO_DEBUG
	printf("Reading port %c pin %d\n",ports[port],portPin);
#endif
	return 0;
}
void pinMode(uint8_t pin, uint8_t mode)
{
	int port = pin >> 5;

	int portPin = pin % 32;
#ifdef GPIO_DEBUG
	printf("Setting port %c pin %d, mode = %d\n", ports[port], portPin,mode);
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

#else
#include "hardware.h"

GPIO_Type*  gpioLU[5] = GPIO_BASE_PTRS;
PORT_Type*  portLU[5] = PORT_BASE_PTRS;

void pinMode (uint8_t pin, uint8_t mode)
{
	int port=pin>>5;

	int portPin=pin%32;

	GPIO_Type* ptr=gpioLU[port];
	PORT_Type* pp=portLU[port];
	if(mode==INPUT || mode==OUTPUT)
	{
		ptr->PDDR=( (ptr->PDDR & ~(1<<portPin)) | mode<<portPin);
		pp->PCR[portPin]=(pp->PCR[portPin] & ~(PORT_PCR_MUX_MASK ))|PORT_PCR_MUX(1);
		pp->PCR[portPin]=(pp->PCR[portPin] & ~(PORT_PCR_ODE_MASK ))|PORT_PCR_ODE(0);
		pp->PCR[portPin]=(pp->PCR[portPin] & ~(PORT_PCR_PE_MASK ))|PORT_PCR_PE(0);
	}
	else if(mode==INPUT_PULLUP || mode==INPUT_PULLDOWN)
	{
		ptr->PDDR=( (ptr->PDDR & ~(1<<portPin)) | INPUT<<portPin);
		pp->PCR[portPin]=(pp->PCR[portPin] & ~(PORT_PCR_MUX_MASK ))|PORT_PCR_MUX(1);
		pp->PCR[portPin]=(pp->PCR[portPin] & ~(PORT_PCR_ODE_MASK ))|PORT_PCR_ODE(0);
		pp->PCR[portPin]=(pp->PCR[portPin] & ~(PORT_PCR_PE_MASK ))|PORT_PCR_PE(1);
		pp->PCR[portPin]=(pp->PCR[portPin] & ~(PORT_PCR_PS_MASK ))|PORT_PCR_PS(!(mode%2));
	}
}

void digitalWrite (uint8_t pin, uint8_t value)
{
	int port=pin>>5;

	int portPin=pin%32;

	GPIO_Type* ptr=gpioLU[port];

	ptr->PDOR=( (ptr->PDOR & ~(1<<portPin)) | value<<portPin);
}
void digitalToggle (uint8_t pin)
{
	int port=pin>>5;

	int portPin=pin%32;

	GPIO_Type* ptr=gpioLU[port];

	ptr->PDOR^= 1<<portPin;
}
uint8_t digitalRead (uint8_t pin)
{
	int port=pin>>5;

	int portPin=pin%32;
	GPIO_Type* ptr=gpioLU[port];

	return (ptr->PDIR>>portPin & 1) ;
}
#endif
