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
 * @file    assignment1_f24.c
 * @brief Matt Hartnett's work for ECEN5813 HW1.
 * This homework assignment focuses on bitwise functions
 * in C.
 * @author  Matt Hartnett
 * @date    9/3/2024
 *
 */
#include "assignment1_f24.h"
#include <stdint.h>


/**
 * @brief Rotates the bits of a number to the right.
 *
 * @param num The number whose bits are to be rotated.
 * @param nbits The number of bits to rotate.
 * @return The resulting number after rotation.
 */
uint8_t rotate_right(uint8_t num, uint8_t nbits) {
    // Any bitshift >= 8 is circular, so eliminate any unnecessary shifts
    nbits = nbits & (ROTATE_BITSIZE - 1);
    uint8_t temp = num;
    num = num >> nbits;
    num = num | (temp << (ROTATE_BITSIZE - nbits));
    return num;
}

/**
 * @brief Converts a binary string to an unsigned integer.
 *
 * @param str The binary string to convert.
 * @return The unsigned integer representation of the binary string.
 */
uint32_t binstr_to_uint(const char *str) {
    const char *crawler = str;
    // Check that both the pointer is not null and the character is not null, and the string starts with 0b
    if (crawler == NULL || *crawler == '\0' || *crawler != '0' || *(crawler + 1) != 'b') {
        return ERROR;
    }

    crawler = crawler + 2; // Jump to the first character of the binary string
    uint32_t num = 0;      // Return value
    uint8_t i = 0;         // Counter to check that input size is >= 32 bits

    // Loop through the string and add 1s and 0s to num appropriately
    while (*crawler != '\0') {
        // Check for over sized input
        if (i >= BINSTR_BITSIZE) {
            return ERROR;
        }

        // Build output num
        num = num << 1;
        if (*crawler == '1') {
            // Set the least significant bit to 1
            num = num | 1;
        } else if (*crawler != '0') {
            // Non binary character (skip if '0', since 0 is automatically added)
            return ERROR;
        }

        // Iterate through string and counter
        crawler++;
        i++;
    }
    return num;
}

/**
 * @brief Converts an integer to a binary string.
 *
 * @param str The buffer to store the binary string.
 * @param size The size of the buffer.
 * @param num The integer to convert.
 * @param nbits The number of bits to use in the conversion.
 * @return Size of string on success, 0xFFFFFFFF on failure.
 */
int32_t int_to_binstr(char *str, size_t size, int32_t num, uint8_t nbits) {
    // Check for nullptr
    if (str == NULL) {
        return ERROR;
    }

    // Calculate the minimum number of bits required to represent num
    uint8_t required_bits = 0;
    int32_t temp = num;
    // Two's complement require 32 bits for negative numbers
    if (num < 0) {
        required_bits = INTTOBINSTR_BITSIZE;
    }
    // num = 0 edgecase (requires 1 bit to represent)
    else if (num == 0) {
        required_bits = 1;
    }
    // Count the number of bits
    else {
        while (temp > 0) {
            temp = temp >> 1;
            required_bits++;
        }
    }

    // Check if nbits is big enough to store num
    if (required_bits > nbits) {
        *str = '\0';
        return ERROR;
    }

    // Check if str buffer has enough space for nbits + 2
    if (size < (nbits + 2)) {
        *str = '\0';
        return ERROR;
    }

    char *crawler = str;
    // Append '0b' to the string
    *crawler = '0';
    crawler++;
    *crawler = 'b';
    crawler++;

    // Loop through the bits of num, add them as chars to the string
    for (uint8_t i = nbits; i > 0; i--) {
        // If 1, add 1, if 0, add 0
        *crawler = ((num >> (i - 1)) & 1) ? '1' : '0';
        crawler++;
    }

    // Terminate string
    *crawler = '\0';
    return nbits + 2;
}

/**
 * @brief Converts a hexadecimal string to an unsigned integer.
 *
 * @param str The hexadecimal string to convert.
 * @return The unsigned integer representation of the hexadecimal string.
 */
uint32_t hexstr_to_uint(const char *str) {
    const char *crawler = str;
    // Check that both the pointer is not null and the character is not null, and the string starts with 0x or 0X
    if (crawler == NULL || *crawler == '\0' || *crawler != '0' || !(*(crawler + 1) == 'x' || *(crawler + 1) == 'X')) {
        return ERROR;
    }

    crawler = crawler + 2; // Jump to the first character of the hex string
    uint32_t num = 0;      // Return value
    uint8_t i = 0;         // Counter to check that input size is >= 32 bits

    // Loop through the string and add 1s and 0s to num appropriately
    while (*crawler != '\0') {
        // Check for over sized input
        if (i >= HEXSTR_BITSIZE) {
            return ERROR;
        }

        // Build output num
        num = num << 4;
        switch (*crawler) {
        case '0':
            num = num | 0x0;
            break;
        case '1':
            num = num | 0x1;
            break;
        case '2':
            num = num | 0x2;
            break;
        case '3':
            num = num | 0x3;
            break;
        case '4':
            num = num | 0x4;
            break;
        case '5':
            num = num | 0x5;
            break;
        case '6':
            num = num | 0x6;
            break;
        case '7':
            num = num | 0x7;
            break;
        case '8':
            num = num | 0x8;
            break;
        case '9':
            num = num | 0x9;
            break;
        case 'A':
        case 'a':
            num = num | 0xA;
            break;
        case 'B':
        case 'b':
            num = num | 0xB;
            break;
        case 'C':
        case 'c':
            num = num | 0xC;
            break;
        case 'D':
        case 'd':
            num = num | 0xD;
            break;
        case 'E':
        case 'e':
            num = num | 0xE;
            break;
        case 'F':
        case 'f':
            num = num | 0xF;
            break;
        default:
            return ERROR;
            break;
        }

        // Iterate through string and counter
        crawler++;
        // Increment by a nibble for each hex digit
        i += 4;
    }
    return num;
}

/**
 * @brief Performs a bitwise operation (clear, set, or toggle) on all bits except the specified one.
 *
 * @param input The input number.
 * @param bit The bit to exclude from the operation.
 * @param operation The operation to perform (CLEAR, SET, TOGGLE).
 * @return The resulting number after the operation.
 */
uint32_t twiggle_except_bit(uint32_t input, int bit, operation_t operation) {
    // Make sure bit is in range
    if (bit < 0 || bit > 31) {
        return ERROR;
    }

    // Create a mask to preserve the specified bit
    uint32_t mask = ~(1 << bit);
    uint32_t ret = 0;

    if (operation == CLEAR) {
        // Clear all bits
        ret = 0;
    } else if (operation == SET) {
        // Set all bits
        ret = UINT32_MAX;
    } else if (operation == TOGGLE) {
        // Toggle all bits
        ret = ~input;
    }
    ret = ret & mask; // Clear the specified bit in the ret
    // Restore the preserved bit
    ret |= (input & ~mask);

    return ret;
}

/**
 * @brief Extracts four bits starting from a specified bit position.
 *
 * @param input The input number.
 * @param start_bit The starting bit position.
 * @return The four extracted bits as a number.
 */
uint32_t grab_four_bits(uint32_t input, int start_bit) {
    // Make sure start_bit is in range
    if (start_bit < 0 || start_bit > 31) {
        return ERROR;
    }

    return ((input >> start_bit) & 0xF);
}

/**
 * @brief Creates a hexadecimal dump of a memory location.
 *
 * @param str The buffer to store the hexadecimal dump.
 * @param size The size of the buffer.
 * @param loc The memory location to dump.
 * @param nbytes The number of bytes to dump.
 * @return A pointer to the buffer containing the hexadecimal dump.
 */
char *hexdump(char *str, size_t size, const void *loc, size_t nbytes) {
    if (nbytes > NBYTES_MAXSIZE) {
        *str = '\0';
        return str;
    }
    if (size == 0) {
        return str;
    }

    // Define the characters used for hexadecimal representation.
    const char hex_chars[] = "0123456789ABCDEF";
    // Create a pointer to the memory location.
    const unsigned char *mem = (const unsigned char *)loc;

    // Index for memory bytes
    uint32_t i = 0;
    // Index for output string
    uint32_t o = 0;

    while (i < nbytes) {
        if ((i & 0b111) == 0) { // If i is divisible by 8
            // Create a readable integer type for the memory location
            uintptr_t address = (uintptr_t)(loc + i);
            // Add memory address location to str
            if (i != 0) {
                // Add newline if this isn't the first line
                str[o] = '\n';
                o++;
            }
            str[o] = '0';
            o++;
            str[o] = 'x';
            o++;
            // Add address itself
            for (int j = BUS_SIZE - 4; j >= 0; j -= 4) {
                str[o] = hex_chars[((address >> j) & 0xF)];
                o++;
            }
            str[o] = ' ';
            o++;
        }

        // Add a single byte to str
        unsigned char byte = mem[i]; // Get the byte from memory
        // Convert the high nibble (4 bits) to a hex character
        unsigned char high_nibble = (byte >> 4) & 0x0F;
        str[o] = hex_chars[high_nibble];
        o++;
        // Convert the low nibble (4 bits) to a hex character
        unsigned char low_nibble = byte & 0x0F;
        str[o] = hex_chars[low_nibble];
        o++;
        str[o] = ' ';
        o++;

        // Increment to the next memory address
        i++;
    }

    str[o] = '\0';
    return str;
}
