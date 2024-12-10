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
 * @brief Matt Hartnett's sleep header file.
 *
 * @author  Matt Hartnett
 * @date    12/07/2024
 *
 */

#ifndef SLEEP_H
#define SLEEP_H

#include <stdint.h>

/**
 * @brief Initializes the sleep mode configuration.
 *
 * This function configures the processor to use the default low-power sleep mode.
 * The deep sleep mode is disabled by clearing the SLEEPDEEP bit in the SCR register.
 * Ensure that other wake-up sources are configured appropriately before entering sleep mode.
 */
void sleep_init(void);

/**
 * @brief Puts the processor into sleep mode.
 *
 * This function executes the Wait For Interrupt (WFI) instruction, putting the processor
 * into sleep mode. The processor will remain in sleep mode until an interrupt occurs.
 */
void enter_sleep(void);

#endif // SLEEP_H