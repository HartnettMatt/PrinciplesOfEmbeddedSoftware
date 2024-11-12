/** DEPRECATED FILE, DO NOT USE
 * File Name: test_functions.c
 *  Author: Matt Hartnett
 *  Tools: gcc
 *  Dependancies: stdint, stdio, test_functions.h, functions.h
 */

#include "test_functions.h"
#include "functions.h"
#include <stdio.h>
#include <stdint.h>

/** TODO:
 *  Change if statements to asserts
 */

uint8_t test_rotate_right() {
    if (rotate_right(0b1, 1) != 0b10000000) {
        printf("Test point 1 failure.\n");
        return 1;
    }
    if (rotate_right(0b1, 2) != 0b01000000) {
        printf("Test point 2 failure.\n");
        return 1;
    }
    if (rotate_right(0b10, 4) != 0b00100000) {
        printf("Test point 3 failure.\n");
        return 1;
    }
    if (rotate_right(0b1, 8) != 0b00000001) {
        printf("Test point 4 failure.\n");
        return 1;
    }
    if (rotate_right(0b1, 9) != 0b10000000) {
        printf("Test point 5 failure.\n");
        return 1;
    }
    if (rotate_right(0b10101111, 4) != 0b11111010) {
        printf("Test point 6 failure.\n");
        return 1;
    }
    // TODO: ADD MORE TEST FUNCTIONS
    return 0;
}

uint8_t test_binstr_to_uint() {
    if (binstr_to_uint(NULL) != 0xFFFFFFFF) {
        printf("Test point 1 failure.\n");
        return 1;
    }
    if (binstr_to_uint("0b00") != 0) {
        printf("Test point 2 failure.\n");
        return 1;
    }
    if (binstr_to_uint("0b05100") != 0xFFFFFFFF) {
        printf("Test point 3 failure.\n");
        return 1;
    }
    if (binstr_to_uint("0b01") != 1) {
        printf("Test point 4 failure.\n");
        return 1;
    }
    if (binstr_to_uint("0b110") != 6) {
        printf("Test point 5 failure.\n");
        return 1;
    }
    // TODO: ADD MORE TEST FUNCTIONS
    return 0;
}

uint8_t test_int_to_binstr() {
    return 0;
}

uint8_t test_hexstr_to_uint() {
    return 0;
}

uint8_t test_twiggle_except_bit() {
    return 0;
}

uint8_t test_grab_four_bits() {
    return 0;
}

uint8_t test_hexdump() {
    return 0;
}