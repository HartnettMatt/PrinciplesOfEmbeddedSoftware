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
 * @file    test_assignment1_f24.c
 * @brief   This file contains all of the test functions to
 * verify the code from assignment 1 of ECEN 5813.
 *
 * @author  Matt Hartnett
 * @date    9/3/2024
 *
 */

#include "test_assignment1_f24.h"
#include "assignment1_f24.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief Tests the rotate_right function.
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_rotate_right() {
    uint8_t result;

    // Test point 1: Rotate by 0 bits (no rotation should occur)
    result = rotate_right(0xFF, 0);
    if (result != 0xFF) {
        printf("Test point 1 failure. Actual result = %02X\n", result);
        return 0;
    }

    // Test point 2: Rotate by 8 bits (full rotation should return the same number)
    result = rotate_right(0x7E, 8);
    if (result != 0x7E) {
        printf("Test point 2 failure. Actual result = %02X\n", result);
        return 0;
    }

    // Test point 3: Rotate by 1 bit
    result = rotate_right(0b11010010, 1);
    if (result != 0b01101001) {
        printf("Test point 3 failure. Actual result = %02X\n", result);
        return 0;
    }

    // Test point 4: Rotate by 4 bits
    result = rotate_right(0b10100101, 4);
    if (result != 0b01011010) {
        printf("Test point 4 failure. Actual result = %02X\n", result);
        return 0;
    }

    // Test point 5: Rotate by more than 8 bits (effectively rotates by nbits % 8)
    result = rotate_right(0b10100001, 9);
    if (result != 0b11010000) {
        printf("Test point 5 failure. Actual result = %02X\n", result);
        return 0;
    }

    return 1;
}


/**
 * @brief Tests the binstr_to_uint function.
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_binstr_to_uint() {
    uint32_t result;

    // Test point 1: Null input should return error
    result = binstr_to_uint(NULL);
    if (result != ERROR) {
        printf("Test point 1 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 2: Empty string should return error
    result = binstr_to_uint("");
    if (result != ERROR) {
        printf("Test point 2 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 3: String without 0b prefix should return error
    result = binstr_to_uint("1010");
    if (result != ERROR) {
        printf("Test point 3 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 4: String with non-binary characters should return error
    result = binstr_to_uint("0b10201");
    if (result != ERROR) {
        printf("Test point 4 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 5: String longer than 32 bits should return error
    result = binstr_to_uint("0b111111111111111111111111111111111");
    if (result != ERROR) {
        printf("Test point 5 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 6: Valid binary string with prefix (small number)
    result = binstr_to_uint("0b101");
    if (result != 5) {
        printf("Test point 6 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 7: Valid binary string representing 32-bit unsigned integer
    result = binstr_to_uint("0b11111111111111111111111111111111");
    if (result != 0xFFFFFFFF) {
        printf("Test point 7 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 8: Binary string representing 0
    result = binstr_to_uint("0b0");
    if (result != 0) {
        printf("Test point 8 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 9: Binary string representing maximum unsigned 16-bit value
    result = binstr_to_uint("0b1111111111111111");
    if (result != 0xFFFF) {
        printf("Test point 9 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 10: Binary string with mixed 1s and 0s
    result = binstr_to_uint("0b1010101010101010");
    if (result != 0xAAAA) {
        printf("Test point 10 failure. Actual result = %u\n", result);
        return 0;
    }

    return 1;
}


/**
 * @brief Tests the int_to_binstr function.
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_int_to_binstr() {
    char buffer[50];
    int32_t result;

    // Test point 1: Null buffer should return error
    result = int_to_binstr(NULL, 50, 5, 8);
    if (result != ERROR) {
        printf("Test point 1 failure. Actual result = %d\n", result);
        return 0;
    }

    // Test point 2: Buffer size too small should return error
    result = int_to_binstr(buffer, 5, 5, 8);
    if (result != ERROR) {
        printf("Test point 2 failure. Actual result = %d\n", result);
        return 0;
    }

    // Test point 3: Negative number with 32-bit size
    result = int_to_binstr(buffer, 50, -5, 32);
    if (result != 34 || strcmp(buffer, "0b11111111111111111111111111111011") != 0) {
        printf("Test point 3 failure. Actual result = %s\n", buffer);
        return 0;
    }

    // Test point 4: Integer is 0 with 1 bit
    result = int_to_binstr(buffer, 50, 0, 1);
    if (result != 3 || strcmp(buffer, "0b0") != 0) {
        printf("Test point 4 failure. Actual result = %s\n", buffer);
        return 0;
    }

    // Test point 5: Integer is 0 with 4 bits
    result = int_to_binstr(buffer, 50, 0, 4);
    if (result != 6 || strcmp(buffer, "0b0000") != 0) {
        printf("Test point 5 failure. Actual result = %s\n", buffer);
        return 0;
    }

    // Test point 6: Integer is positive with nbits matching size
    result = int_to_binstr(buffer, 50, 10, 4);
    if (result != 6 || strcmp(buffer, "0b1010") != 0) {
        printf("Test point 6 failure. Actual result = %s\n", buffer);
        return 0;
    }

    // Test point 7: nbits too small to represent the integer should return error
    result = int_to_binstr(buffer, 50, 10, 3);
    if (result != ERROR) {
        printf("Test point 7 failure. Actual result = %d\n", result);
        return 0;
    }

    // Test point 8: Exact size of buffer should work
    result = int_to_binstr(buffer, 6, 5, 3);
    if (result != 5 || strcmp(buffer, "0b101") != 0) {
        printf("Test point 8 failure. Actual result = %s\n", buffer);
        return 0;
    }

    // Test point 9: Positive integer with 8 bits
    result = int_to_binstr(buffer, 50, 255, 8);
    if (result != 10 || strcmp(buffer, "0b11111111") != 0) {
        printf("Test point 9 failure. Actual result = %s\n", buffer);
        return 0;
    }

    // Test point 10: Negative number that requires full 32-bit size
    result = int_to_binstr(buffer, 50, -1, 32);
    if (result != 34 || strcmp(buffer, "0b11111111111111111111111111111111") != 0) {
        printf("Test point 10 failure. Actual result = %s\n", buffer);
        return 0;
    }

    return 1;
}



/**
 * @brief Tests the hexstr_to_uint function.
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_hexstr_to_uint() {

    uint32_t result;

    // Test point 1: NULL input should return error
    result = hexstr_to_uint(NULL);
    if (result != ERROR) {
        printf("Test point 1 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 2: Empty string should return error
    result = hexstr_to_uint("");
    if (result != ERROR) {
        printf("Test point 2 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 3: String without 0x prefix should return error
    result = hexstr_to_uint("1A");
    if (result != ERROR) {
        printf("Test point 3 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 4: Invalid character in the string should return error
    result = hexstr_to_uint("0xH1");
    if (result != ERROR) {
        printf("Test point 4 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 5: Hexadecimal string that exceeds 32 bits should return error
    result = hexstr_to_uint("0x123456789");
    if (result != ERROR) {
        printf("Test point 5 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 6: Valid hexadecimal with 0x prefix
    result = hexstr_to_uint("0xFF");
    if (result != 0xFF) {
        printf("Test point 6 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 7: Lowercase hexadecimal should work
    result = hexstr_to_uint("0x1a3f");
    if (result != 0x1A3F) {
        printf("Test point 7 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 8: Maximum valid 32-bit hexadecimal number
    result = hexstr_to_uint("0xFFFFFFFF");
    if (result != 0xFFFFFFFF) {
        printf("Test point 8 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 9: Short valid hexadecimal string
    result = hexstr_to_uint("0x7");
    if (result != 0x7) {
        printf("Test point 9 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 10: Mixed-case hexadecimal string
    result = hexstr_to_uint("0xAbC123");
    if (result != 0xABC123) {
        printf("Test point 10 failure. Actual result = %u\n", result);
        return 0;
    }

    return 1;
}

/**
 * @brief Tests the twiggle_except_bit function.
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_twiggle_except_bit() {
    uint32_t result;

    // Test point 1: Invalid bit index (negative)
    result = twiggle_except_bit(0xFFFFFFFF, -1, SET);
    if (result != ERROR) {
        printf("Test point 1 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 2: Invalid bit index (greater than 31)
    result = twiggle_except_bit(0xFFFFFFFF, 32, CLEAR);
    if (result != ERROR) {
        printf("Test point 2 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 3: Clear operation on all bits except bit 0
    result = twiggle_except_bit(0xFFFFFFFF, 0, CLEAR);
    if (result != 0x00000001) {
        printf("Test point 3 failure. Actual result = %08X\n", result);
        return 0;
    }

    // Test point 4: Set operation on all bits except bit 15
    result = twiggle_except_bit(0x00000000, 15, SET);
    if (result != 0xFFFF7FFF) {
        printf("Test point 4 failure. Actual result = %08X\n", result);
        return 0;
    }

    // Test point 5: Toggle operation on all bits except bit 31
    result = twiggle_except_bit(0x80000000, 31, TOGGLE);
    if (result != 0xFFFFFFFF) {
        printf("Test point 5 failure. Actual result = %08X\n", result);
        return 0;
    }

    // Test point 6: Clear operation on all bits except bit 31
    result = twiggle_except_bit(0xFFFFFFFF, 31, CLEAR);
    if (result != 0x80000000) {
        printf("Test point 6 failure. Actual result = %08X\n", result);
        return 0;
    }

    // Test point 7: Set operation on all bits except bit 0
    result = twiggle_except_bit(0x00000000, 0, SET);
    if (result != 0xFFFFFFFE) {
        printf("Test point 7 failure. Actual result = %08X\n", result);
        return 0;
    }

    // Test point 8: Toggle operation on all bits except bit 0
    result = twiggle_except_bit(0x00000001, 0, TOGGLE);
    if (result != 0xFFFFFFFF) {
        printf("Test point 8 failure. Actual result = %08X\n", result);
        return 0;
    }

    // Test point 9: Toggle operation on a mixed value, excluding bit 15
    result = twiggle_except_bit(0x12345678, 15, TOGGLE);
    if (result != 0xEDCB2987) {
        printf("Test point 9 failure. Actual result = %08X\n", result);
        return 0;
    }

    // Test point 10: Set operation on an arbitrary value, excluding bit 8
    result = twiggle_except_bit(0x00FF00FF, 8, SET);
    if (result != 0xFFFFFEFF) {
        printf("Test point 10 failure. Actual result = %08X\n", result);
        return 0;
    }

    return 1;
}


/**
 * @brief Tests the grab_four_bits function.
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_grab_four_bits() {
    uint32_t result;

    // Test point 1: Invalid starting bit position (negative)
    result = grab_four_bits(0xFFFFFFFF, -1);
    if (result != ERROR) {
        printf("Test point 1 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 2: Invalid starting bit position (32)
    result = grab_four_bits(0xFFFFFFFF, 32);
    if (result != ERROR) {
        printf("Test point 2 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 3: Valid extraction from the lowest bits
    result = grab_four_bits(0x12345678, 0);
    if (result != 0x8) {
        printf("Test point 3 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 4: Valid extraction from a high bit position
    result = grab_four_bits(0xFEDCBA98, 28);
    if (result != 0xF) {
        printf("Test point 4 failure. Actual result = %u\n", result);
        return 0;
    }

    // Test point 5: Valid extraction from the highest possible bit position
    result = grab_four_bits(0x12345678, 28);
    if (result != 0x1) {
        printf("Test point 5 failure. Actual result = %u\n", result);
        return 0;
    }

    return 1;
}


/**
 * @brief Tests the hexdump function.
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_hexdump() {
    char buffer[1024];
    char *result;
    uint8_t memory[16] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    // Mask to get the lower 32 bits of the pointer
    uintptr_t masked_ptr = (uintptr_t)memory & 0xFFFFFFFF;

    // Test point 1: Valid memory dump of 16 bytes
    char expected_result[1024];
    snprintf(expected_result, sizeof(expected_result),
             "0x%lX 12 34 56 78 9A BC DE F0 \n0x%lX 11 22 33 44 55 66 77 88 ", masked_ptr, masked_ptr + 8);
    result = hexdump(buffer, sizeof(buffer), memory, 16);
    if (strcmp(result, expected_result) != 0) {
        printf("Test point 1 failure. Actual result = %s\n Expected result = %s\n", result, expected_result);
        return 0;
    }

    // Test point 2: Dump with zero bytes
    result = hexdump(buffer, sizeof(buffer), memory, 0);
    if (strcmp(result, "") != 0) {
        printf("Test point 2 failure. Actual result = %s\n", result);
        return 0;
    }

    // Test point 3: Dump with buffer size too small (no memory content)
    result = hexdump(buffer, 0, memory, 16);
    if (strcmp(result, "") != 0) {
        printf("Test point 3 failure. Actual result = %s\n", result);
        return 0;
    }

    // Test point 4: Valid memory dump of 8 bytes
    snprintf(expected_result, sizeof(expected_result),
             "0x%lX 12 34 56 78 9A BC DE F0 ", masked_ptr);
    result = hexdump(buffer, sizeof(buffer), memory, 8);
    if (strcmp(result, expected_result) != 0) {
        printf("Test point 4 failure. Actual result = %s\n", result);
        return 0;
    }

    // Test point 5: Valid memory dump of 4 bytes
    snprintf(expected_result, sizeof(expected_result),
             "0x%lX 12 34 56 78 ", masked_ptr);
    result = hexdump(buffer, sizeof(buffer), memory, 4);
    if (strcmp(result, expected_result) != 0) {
        printf("Test point 10 failure. Actual result = %s\n", result);
        return 0;
    }

    return 1;
}
