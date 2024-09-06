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
 * @file    main.c
 * @brief The main file for assignment 1 of ECEN 5813, Programming
 * Embedded Systems. This file runs all of the test code for
 * assignment 1.
 *
 * @author  Matt Hartnett
 * @date    9/3/2024
 *
 */

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "assignment1_f24.h"
#include "test_assignment1_f24.h"

/**
 * @brief      main
 *
 * @param[in]  argc  The count of arguments
 * @param      argv  The arguments array
 *
 * @return     always 0
 */
int32_t main(int32_t argc, char **argv) {
 printf("Testing rotate_right:\n");
 assert(test_rotate_right() == 1);
 printf("Tests passed!\n");

 printf("Testing binstr_to_uint:\n");
 assert(test_binstr_to_uint() == 1);
 printf("Tests passed!\n");

 printf("Testing int_to_binstr:\n");
 assert(test_int_to_binstr() == 1);
 printf("Tests passed!\n");

 printf("Testing hexstr_to_uint:\n");
 assert(test_hexstr_to_uint() == 1);
 printf("Tests passed!\n");

 printf("Testing twiggle_except_bit:\n");
 assert(test_twiggle_except_bit() == 1);
 printf("Tests passed!\n");

 printf("Testing grab_four_bits:\n");
 assert(test_grab_four_bits() == 1);
 printf("Tests passed!\n");

 printf("Testing hexdump:\n");
 assert(test_hexdump() == 1);
 printf("Tests passed!\n");

 return 0;
}
