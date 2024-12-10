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
 * @file    systick.c
 * @brief Matt Hartnett's systick file.
 *
 * @author  Matt Hartnett
 * @date    12/07/2024
 *
 */

#include "systick.h"
#include <stm32f091xc.h>

void systick_init(void) {
    // SysTick is defined in core_cm0.h
    // Set reload to get 100ms interrupts
    SysTick->LOAD = (F_SYS_CLK / 80) - 1;

    // Set interrupt priority
    NVIC_SetPriority(SysTick_IRQn, 3);
    // Force load of reload value
    SysTick->VAL = 0;
}

void systick_start(void){
    // Enable interrupt, enable SysTick timer
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}