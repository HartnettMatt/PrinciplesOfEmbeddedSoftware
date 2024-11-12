/** DEPRECATED FILE, DO NOT USE
 * File Name: main.c
 *  Author: Matt Hartnett
 *  Tools: gcc
 *  Dependancies: stdint, functions.h, test_functions.h
 */
 
#include <stdio.h>
#include "test_functions.h"

int main(void) {
    // Run tests for each function
    if (test_rotate_right() == 0) {
        printf("test_rotate_right passed.\n");
    } else {
        printf("test_rotate_right failed.\n");
    }

    if (test_binstr_to_uint() == 0) {
        printf("test_binstr_to_uint passed.\n");
    } else {
        printf("test_binstr_to_uint failed.\n");
    }

    if (test_int_to_binstr() == 0) {
        printf("test_int_to_binstr passed.\n");
    } else {
        printf("test_int_to_binstr failed.\n");
    }

    if (test_hexstr_to_uint() == 0) {
        printf("test_hexstr_to_uint passed.\n");
    } else {
        printf("test_hexstr_to_uint failed.\n");
    }

    if (test_twiggle_except_bit() == 0) {
        printf("test_twiggle_except_bit passed.\n");
    } else {
        printf("test_twiggle_except_bit failed.\n");
    }

    if (test_grab_four_bits() == 0) {
        printf("test_grab_four_bits passed.\n");
    } else {
        printf("test_grab_four_bits failed.\n");
    }

    if (test_hexdump() == 0) {
        printf("test_hexdump passed.\n");
    } else {
        printf("test_hexdump failed.\n");
    }

    return 0;
}
