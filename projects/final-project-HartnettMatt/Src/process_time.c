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
 * @file    process_time.c
 * @brief Matt Hartnett's Time Processor implementation file.
 *
 * @author  Matt Hartnett
 * @date    11/04/2024
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process_time.h"
#include "uled.h"
int process_time(char *input, size_t nbytes) {
    int hours = -1, minutes = -1, total_seconds = 0;
    char *endptr;

    // Skip leading whitespace
    while (*input && isspace((unsigned char)*input)) {
        input++;
    }

    // Check if the input is empty
    if (*input == '\0') {
        printf("Error: Empty input\r\n");
        return -1;
    }

    // Parse hours and minutes
    if (isdigit((unsigned char)*input)) {
        int time_value = strtol(input, &endptr, 10);

        // Check if the input length indicates a combined hours-minutes format
        int length = endptr - input;
        if (length == 4) { // Format like 0800
            hours = time_value / 100;
            minutes = time_value % 100;
            input = endptr;
        } else if (length == 2 || *endptr == ':') { // Format like 08:00 or 08
            hours = time_value;
            input = endptr;

            if (*input == ':') {
                input++;
                if (isdigit((unsigned char)*input)) {
                    minutes = strtol(input, &endptr, 10);
                    input = endptr;
                } else {
                    printf("Error: Invalid minutes format\r\n");
                    return -1;
                }
            } else {
                minutes = 0; // Default minutes to 0 if not provided
            }
        } else {
            printf("Error: Invalid time format\r\n");
            return -1;
        }
    } else {
        printf("Error: Invalid hours format\r\n");
        return -1;
    }

    // Validate hours and minutes
    if (minutes < 0 || minutes >= 60) {
        printf("Error: Invalid minutes\r\n");
        return -1;
    }
    if (hours < 0 || hours > 23) {
        printf("Error: Invalid hours\r\n");
        return -1;
    }

    // Skip whitespace
    while (*input && isspace((unsigned char)*input)) {
        input++;
    }

    // Parse optional AM/PM
    if (*input != '\0') {
        if (strncasecmp(input, "AM", 2) == 0) {
            if (hours < 1 || hours > 12) {
                printf("Error: Invalid hours for 12-hour format\r\n");
                return -1;
            }
            if (hours == 12) hours = 0; // Convert 12 AM to 0 hours
        } else if (strncasecmp(input, "PM", 2) == 0) {
            if (hours < 1 || hours > 12) {
                printf("Error: Invalid hours for 12-hour format\r\n");
                return -1;
            }
            if (hours != 12) hours += 12; // Convert PM hours to 24-hour format
        } else {
            printf("Error: Invalid AM/PM format\r\n");
            return -1;
        }
    }

    // Calculate total seconds since midnight
    total_seconds = hours * 3600 + minutes * 60;

    return total_seconds;
}