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
 * @file    assignment1_f24.h
 * @brief Matt Hartnett's work for ECEN5813 HW1.
 * This homework assignment focuses on bitwise functions
 * in C.
 * @author  Matt Hartnett
 * @date    9/3/2024
 *
 */

#ifndef TEST_ASSIGNMENT1_F24_H
#define TEST_ASSIGNMENT1_F24_H

#include <stdint.h>
#include <stddef.h>

uint8_t test_rotate_right(void);
uint8_t test_binstr_to_uint(void);
uint8_t test_int_to_binstr(void);
uint8_t test_hexstr_to_uint(void);
uint8_t test_twiggle_except_bit(void);
uint8_t test_grab_four_bits(void);
uint8_t test_hexdump(void);

#endif // TEST_ASSIGNMENT1_F24_H

