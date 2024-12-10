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
 * @file    process_time.h
 * @brief Matt Hartnett's Time Processor header file.
 *
 * @author  Matt Hartnett
 * @date    12/04/2024
 *
 */

#ifndef _PROCESS_TIME_H
#define _PROCESS_TIME_H

#include <stddef.h>
#include <stdint.h>

#define MAX_LEN 640 // Max allowed length of hexdump

/*
 * Command processor engine
 *
 * Parameters:
 * input: Command to process
 * valid: Number of bytes (characters) in command
 *
 * Returns: The number of seconds since midnight of the time in the string
 */
int process_time(char *input, size_t nbytes);


#endif // _PROCESS_TIME_H