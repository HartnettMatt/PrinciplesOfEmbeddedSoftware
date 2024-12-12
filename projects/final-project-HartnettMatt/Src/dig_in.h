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
 * @file    dig_in.h
 * @brief Matt Hartnett's digital input header file.
 *
 * @author  Matt Hartnett
 * @date    12/06/2024
 *
 */

#ifndef DIG_IN_H
#define DIG_IN_H

#include <stdint.h>

#define PB3_READ_MSK	(GPIO_IDR_3)
#define PB3_READ_OFFSET 3

/**
 * @brief Initializes the digital input on PB3.
 *
 * Configures GPIOB pin 3 (PB3) as an input by enabling the GPIOB peripheral clock
 * and setting the appropriate pin mode.
 *
 */
void dig_in_init(void);

/**
 * @brief Reads the state of the digital input on PB3.
 *
 * Returns the current logic level of PB3 as a boolean value (0 or 1).
 *
 * @return The state of PB3: 1 if the pin is high, 0 if it is low.
 */
int dig_in_read(void);

/**
 * @brief Self test of digital input
 *
 * Prints the current logic level of PB3 as a boolean value (0 or 1).
 *
 */
void dig_in_test(void);

#endif // DIG_IN_H