#pragma once
/***************************************************************************//**
  @file     gpio.h
  @brief    Simple GPIO Pin services, similar to Arduino
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"

#include <stdint.h>
//Sacar esto
#include <stdio.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Configures the specified pin to behave either as an input or an output
 * @param pin the pin whose mode you wish to set (according PORTNUM2PIN)
 * @param mode INPUT, OUTPUT, INPUT_PULLUP or INPUT_PULLDOWN.
 */
void pinMode(uint8_t pin, uint8_t mode)
{
	int port = pin >> 5;

	int portPin = pin % 32;
	printf("Setting pin %d as %d\n",pin, mode);
}

/**
 * @brief Write a HIGH or a LOW value to a digital pin
 * @param id the pin to write (according PORTNUM2PIN)
 * @param val Desired value (HIGH or LOW)
 */
void digitalWrite(uint8_t pin, uint8_t value)
{
	int port = pin >> 5;

	int portPin = pin % 32;
	printf("Writing to pin %d the value %d\n", pin, value);
}

/**
 * @brief Toggle the value of a digital pin (HIGH<->LOW)
 * @param id the pin to toggle (according PORTNUM2PIN)
 */
void digitalToggle(uint8_t pin)
{
	int port = pin >> 5;

	int portPin = pin % 32;

	printf("Toggling pin %d\n", pin);
}

/**
 * @brief Reads the value from a specified digital pin, either HIGH or LOW.
 * @param id the pin to read (according PORTNUM2PIN)
 * @return HIGH or LOW
 */
uint8_t digitalRead(uint8_t pin)
{
	int port = pin >> 5;

	int portPin = pin % 32;
	printf("Reading pin %d\n", pin);
	return 0;
}


/*******************************************************************************
 ******************************************************************************/

#endif // _GPIO_H_
