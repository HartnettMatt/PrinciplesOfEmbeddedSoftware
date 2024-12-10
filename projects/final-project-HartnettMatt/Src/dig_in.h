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
// TODO: write comments
int dig_in_init(void);

int dig_in_read(void);

#endif // DIG_IN_H