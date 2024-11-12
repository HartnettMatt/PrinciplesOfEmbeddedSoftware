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
 * @file    process_cmd.c
 * @brief Matt Hartnett's Command Processor implementation file.
 *
 * @author  Matt Hartnett
 * @date    11/11/2024
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hexdump.h"
#include "process_cmd.h"
#include "uled.h"

unsigned int parse_hex(const char *str) {
    unsigned int value = 0;
    sscanf(str, "%x", &value);
    return value;
}

int is_valid_hex(const char *str) {
    while (*str) {
        if (!isxdigit((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

int parse_length(const char *str, int *valid) {
    *valid = 1;

    // Check if the length is in hexadecimal format
    if (strncmp(str, "0x", 2) == 0) {
        // Skip "0x" prefix
        str += 2;

        // Validate the rest of the string as hexadecimal
        const char *hex_str = str;
        while (*hex_str) {
            if (!isxdigit((unsigned char)*hex_str)) {
                *valid = 0;
                return 0;
            }
            hex_str++;
        }
        // Convert the hexadecimal string to an integer
        return parse_hex(str);
    }

    // Otherwise, validate as a decimal number
    const char *dec_str = str;
    while (*dec_str) {
        if (!isdigit((unsigned char)*dec_str)) {
            *valid = 0;
            return 0;
        }
        dec_str++;
    }
    // Convert the decimal string to an integer
    return atoi(str);
}

void process_command(char *input, size_t nbytes) {
    // Convert the input string to uppercase and remove extra spaces
    char output[100];
    int out_idx = 0;
    int in_word = 0;

    // Skip leading whitespace
    while (*input && isspace((unsigned char)*input)) {
        input++;
    }

    // Check if the command is empty and return if so
    if (*input == '\0') {
        return;
    }

    // Check for "echo" command
    if (strncasecmp(input, "echo", 4) == 0) {
        input += 4;

        // Skip whitespace after "echo"
        while (*input && isspace((unsigned char)*input)) {
            input++;
        }

        // Process the rest of the input as the string to echo
        while (*input) {
            if (isspace((unsigned char)*input)) {
                if (in_word) {
                    // Add a single space between words
                    output[out_idx++] = ' ';
                    in_word = 0;
                }
            } else {
                // Convert to uppercase and add to output
                output[out_idx++] = toupper((unsigned char)*input);
                in_word = 1;
            }
            input++;
        }

        // Remove any trailing space
        if (out_idx > 0 && output[out_idx - 1] == ' ') {
            out_idx--;
        }

        // Null-terminate the output string
        output[out_idx] = '\0';

        // Send the processed string back to the terminal
        printf(output);
        printf("\r\n");

        // Check for LED command
    } else if (strncasecmp(input, "led", 3) == 0) {
        input += 3;
        // Skip whitespace after "led"
        while (*input && isspace((unsigned char)*input)) {
            input++;
        }
        if (strncasecmp(input, "off", 3) == 0) {
            uled_off();
        } else if (strncasecmp(input, "on", 2) == 0) {
            uled_on();
        } else {
            // Send an error message
            printf("Unknown LED command\r\n");
        }

        // Check for "hexdump" command
    } else if (strncasecmp(input, "hexdump", 7) == 0) {
        input += 7;

        // Skip whitespace after "hexdump"
        while (*input && isspace((unsigned char)*input)) {
            input++;
        }

        // Extract the Start argument (hexadecimal address)
        char *start_str = input;
        while (*input && !isspace((unsigned char)*input)) {
            input++;
        }

        // Null-terminate Start argument and move to the Len argument
        if (*input)
            *input++ = '\0';

        // Skip whitespace to reach Len argument
        while (*input && isspace((unsigned char)*input)) {
            input++;
        }

        // Extract the Len argument
        char *len_str = input;
        while (*input && !isspace((unsigned char)*input)) {
            input++;
        }

        // Null-terminate Len argument
        if (*input)
            *input = '\0';

        // Validate Start argument as a hexadecimal address
        if (!is_valid_hex(start_str)) {
            printf("Error: invalid start address\r\n");
            return;
        }
        void *start_addr = (void *)(uintptr_t)parse_hex(start_str);

        // Validate and parse Len argument
        int len_valid = 0;
        int len = parse_length(len_str, &len_valid);
        if (!len_valid || len < 0 || len > MAX_LEN) {
            printf("Error: invalid length\r\n");
            return;
        }

        // Buffer to hold the hexdump output
        char hexdump_output[512];

        // Call the hexdump function and print the output
        hexdump(hexdump_output, sizeof(hexdump_output), start_addr, (size_t)len);
        printf(hexdump_output);
        printf("\r\n");

        // ADD NEW COMMANDS HERE IF YOU'D LIKE
        // Add a new else if here and compare the input to your desired command
        // Increment input by the number of your characters in your command, then start programming!
    } else {
        // If command is not recognized, send an error message
        printf("Unknown command: ");
        printf(input);
        printf("\n\r");
    }
}