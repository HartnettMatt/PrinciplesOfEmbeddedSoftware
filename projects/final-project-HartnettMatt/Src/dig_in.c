/*******************************************************************************
 * Copyright (C) 2024 by Matt Hartnett
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Matt Hartnett and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/

/**
 * @file    dig_in.c
 * @brief Matt Hartnett's digital input file.
 *
 * @author  Matt Hartnett
 * @date    12/06/2024
 *
 */

#include "log.h"
#include <stm32f091xc.h>
#include "dig_in.h"
#include "utilities.h"

void dig_in_init(void) {
	// Enable peripheral clock of GPIOB (for PB3)
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	// Configure PB3 in input mode
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER3, ESF_GPIO_MODER_INPUT);
}

int dig_in_read(void){
    return (GPIOB->IDR & PB3_READ_MSK) >> PB3_READ_OFFSET;
}

void dig_in_test(void){
    int val = (GPIOB->IDR & PB3_READ_MSK) >> PB3_READ_OFFSET;
    printf("Current digital value: %d\r\n", val);
}

