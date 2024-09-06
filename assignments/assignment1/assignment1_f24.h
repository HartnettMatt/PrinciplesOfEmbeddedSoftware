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
 * @brief The header file for assignment 1
 *
 *
 * @author Matt Hartnett
 * @date   9/3/2024
 *
 */

#ifndef ASSIGNMENT1_F24_H
#define ASSIGNMENT1_F24_H

#include <stdint.h>     // for uint32_t and similar definitions
#include <stdlib.h>     // for size_t

#define ROTATE_BITSIZE 8
#define BINSTR_BITSIZE 32
#define INTTOBINSTR_BITSIZE 32
#define HEXSTR_BITSIZE 32
#define ERROR 0xFFFFFFFFU
#define NBYTES_MAXSIZE 65535
#define BUS_SIZE 32

// NOTE: FOR THIS ASSIGNMENT, YOU MAY NOT USE STANDARD C LIBRARY
//       FUNCTIONS, STRINGS FUNCTIONS, INCLUDING THE PRINTF FAMILY, IN YOUR
//       PRODUCTION CODE. HOWEVER, YOU MAY USE SUCH FUNCTIONS IN ANY OF THE
//       TEST CODE. USE THE MACRO DEBUG IN MAKEFILE TO SELECT BETWEEN PRODUCTION
//       VS TEST CODE.

typedef enum {
    CLEAR,    // Set the bits to 0
    SET,      // Set the bits to 1
    TOGGLE    // Change bit value 1 to 0 and vice versa
} operation_t;

uint8_t rotate_right(uint8_t num, uint8_t nbits);
uint32_t binstr_to_uint(const char *str);
int32_t int_to_binstr(char *str, size_t size, int32_t num, uint8_t nbits);
uint32_t hexstr_to_uint(const char *str);

uint32_t twiggle_except_bit(uint32_t input, int bit, operation_t operation);
uint32_t grab_four_bits(uint32_t input, int start_bit);
char *hexdump(char *str, size_t size, const void *loc, size_t nbytes);

#endif  /* ASSIGNMENT1_F24_H */
