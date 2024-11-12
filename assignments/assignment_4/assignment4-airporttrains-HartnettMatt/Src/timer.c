/*
 * timer.c
 *
 *  Created on: Oct 14, 2024
 *      Author: Matt Hartnett and Alex Dean
 */

#include <stm32f091xc.h>
#include <core_cm0.h>
#include "timer.h"
#include "fsm.h"

uint8_t brightness = 0;
// This section of code was taken from Alex Dean's Github and modified by Matt Hartnett
// https://github.com/alexander-g-dean/ESF/blob/master/ST/Code/ch7/SysTick/main.c
void Init_SysTick(void) {
	// SysTick is defined in core_cm0.h
	// Set reload to get 100ms interrupts
	SysTick->LOAD = (F_SYS_CLK / 80) - 1;

	// Set interrupt priority
	NVIC_SetPriority(SysTick_IRQn, 3);
	// Force load of reload value
	SysTick->VAL = 0;
	// Enable interrupt, enable SysTick timer
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk |
			SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void){
    // increment 100ms counter for CSL
    fsm_increment();
}

