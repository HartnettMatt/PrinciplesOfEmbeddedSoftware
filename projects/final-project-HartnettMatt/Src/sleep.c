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
 * @file    sleep.c
 * @brief Matt Hartnett's sleep file.
 *
 * @author  Matt Hartnett
 * @date    12/07/2024
 *
 */

#include <stm32f091xc.h>
#include "sleep.h"
#include "utilities.h"

void sleep_init(void) {
    // Ensure low-power sleep mode (default mode without deep sleep)
    MODIFY_FIELD(SCB->SCR, SCB_SCR_SLEEPDEEP, 0);
}

void enter_sleep(void) {
    // Enter sleep mode by executing the WFI instruction
    __WFI(); // Wait for interrupt
}
