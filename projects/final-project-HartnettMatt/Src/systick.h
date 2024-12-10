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
 * @file    sleep.h
 * @brief Matt Hartnett's systick header file.
 *
 * @author  Matt Hartnett
 * @date    12/07/2024
 *
 */

#ifndef SYSTICK_H
#define SYSTICK_H

#define F_SYS_CLK (48000000L)

/**
 * Initializes the SysTick timer to generate periodic interrupts.
 *
 * This function configures the SysTick timer to produce an interrupt
 * every 100ms. The timer is initialized with a specific reload value
 * and interrupt priority.
 *
 */
void systick_init(void);

/**
 * @brief Starts sys tick
 *
 */
void systick_start(void);

#endif // SYSTICK_H