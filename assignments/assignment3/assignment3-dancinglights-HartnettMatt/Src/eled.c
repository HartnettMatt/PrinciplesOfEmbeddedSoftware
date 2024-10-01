/*
 * eled.c
 *
 *  Created on: Sep 29, 2024
 *      Author: Matt Hartnett
 */

#include "log.h"
#include <stm32f091xc.h>
#include "eled.h"
#include "utilities.h"

int eled_init(void) {
	// Enable peripheral clock of GPIOB (for PB3)
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	// Configure PB3 in output mode (01=1)
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER3, ESF_GPIO_MODER_OUTPUT);
	// Turn off PB3
	GPIOB->BSRR |= PB3_OFF_MSK;

	return 0;

}

int eled_on(void){
	LOG("ELED ON\r\n");
	GPIOB->BSRR |= PB3_ON_MSK;
	return 0;
}

int eled_off(void){
	LOG("ELED OFF\r\n");
	GPIOB->BSRR |= PB3_OFF_MSK;
	return 0;
}
