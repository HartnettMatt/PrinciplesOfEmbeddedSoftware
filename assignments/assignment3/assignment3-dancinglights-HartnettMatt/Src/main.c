/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Matt Hartnett
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <stm32f091xc.h>
#include "uled.h"
#include "button.h"
#include "utilities.h"
#include "timer.h"
#include "eled.h"

#define STARTUP_LONG_TIME 2000
#define STARTUP_SHORT_TIME 1000

#define ULED_ON_TIME 3000
#define ULED_OFF_TIME 1500
#define ELED_ON_TIME 1000
#define ELED_OFF_TIME 500

/*
 * Initializes the peripherals and runs through start-up sequence
 *
 * Parameters:
 *   none
 *
 * Returns: 0 if everything runs correctly, 0 if an error occurs
 */
int init(void) {

	// Configure peripherals
	uled_init();
	eled_init();
	button_init();

	// Start up sequence (uninterruptible)
	uled_on();
	delay(STARTUP_LONG_TIME, 0);
	uled_off();
	delay(STARTUP_SHORT_TIME, 0);
	eled_on();
	delay(STARTUP_LONG_TIME, 0);
	eled_off();
	delay(STARTUP_SHORT_TIME, 0);
	uled_on();
	delay(STARTUP_LONG_TIME, 0);
	uled_off();
	delay(STARTUP_SHORT_TIME, 0);

	return 0;

}

/*
 * Blink the ELED on and off, if a button is pressed: return
 *
 * Parameters:
 *   none
 *
 * Returns: 0 once the button is pressed, 1 if an error occurs
 */
int sequence1(void) {
	// Loop the blinking infinitely unless a button is pressed
	while (1) {
		eled_on();
		// These if statements run an interruptible delay loop
		// If the button interrupts the loop, exit the sequence
		if (delay(ELED_ON_TIME, 1)) {
			return 0;
		}
		eled_off();
		if (delay(ELED_OFF_TIME, 1)) {
			return 0;
		}
	}
	return 1;
}

/*
 * Blink the ULED on and off, if a button is pressed: return
 *
 * Parameters:
 *   none
 *
 * Returns: 0 once the button is pressed, 1 if an error occurs
 */
int sequence2(void) {
	while (1) {
		uled_on();
		if (delay(ULED_ON_TIME, 1)) {
			return 0;
		}
		uled_off();
		if (delay(ULED_OFF_TIME, 1)) {
			return 0;
		}
	}
	return 1;
}

int main(void) {
	/* You can use the Command Shell Console in STM32CubeIDE or an
	 * external program such as Putty or TeraTerm to view printf output.
	 * The settings are: Baud: 9600, Parity: None, Stop Bits: 1 with
	 * the ST-Link COM port setting.
	 * Clocks: Processor = 48 Mhz. AHB = 48 MHz. APB = 24 MHz.
	 */
#ifdef DEBUG
	printf("Hello World !\r\n");
#endif //DEBUG
	init();

	// Loop infinitely - if one sequence returns, switch to the other
	while (1) {
		sequence1(); 	// Blink ELED
		eled_off(); 	// Ensure the ELED is off before moving on
#ifdef DEBUG
		printf("CHANGE LED TO ULED\r\n");
#endif //DEBUG
		sequence2(); 	// Blink ULED
		uled_off(); 	// Ensure the ULED is off before moving on
#ifdef DEBUG
		printf("CHANGE LED TO ELED\r\n");
#endif //DEBUG
	}
}
