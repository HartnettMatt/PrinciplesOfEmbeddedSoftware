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
 * @file    process_cmd.h
 * @brief Matt Hartnett's Command Processor header file.
 *
 * @author  Matt Hartnett
 * @date    11/11/2024
 *
 */

#ifndef _PROCESS_CMD_H
#define _PROCESS_CMD_H

#include <stddef.h>
#include <stdint.h>

#define MAX_LEN 640 // Max allowed length of hexdump

/*
 * Helper function to parse hexadecimal.
 * Note: remove 0x before calling
 *
 * Parameters:
 * str: Hex string to process
 *
 * Returns: Integer value of hex string
 */
unsigned int parse_hex(const char *str);

/*
 * Helper function to check if a string is a valid hexadecimal
 *
 * Parameters:
 * str: Hex string to process
 *
 * Returns: 0 if invalid, 1 if valid
 */
int is_valid_hex(const char *str);

/*
 * Helper function to parse and validate the length argument
 *
 * Parameters:
 * str: Hex string to process
 * valid: Pointer to a valid bit to indicate if the argument is correct or not
 *
 * Returns: Integer value of length argument
 */
int parse_length(const char *str, int *valid);

/*
 * Command processor engine
 *
 * Parameters:
 * input: Command to process
 * valid: Number of bytes (characters) in command
 *
 * Returns: None
 */
void process_command(char *input, size_t nbytes);


#endif // _PROCESS_CMD_H