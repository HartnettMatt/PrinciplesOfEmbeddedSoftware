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
 * @brief Matt Hartnett's main file for ECEN5813 Assignment 6: Serial IO
 * This program acts as a very simple command line that takes in a few different
 * commands and responds accordingly. The UART was implemented as part of this project
 * in USART.h/.c. Command processing happens in process_cmd.h/.c. Some helper code
 * was taken from previous assignments as projects, such as cbfifo, uled, and hexdump.
 *
 * @author  Matt Hartnett
 * @date    11/11/2024
 *
 */

#include "uled.h"
#include "usart.h"
#include "test_cbfifo.h"
#include <stdio.h>

#define PROMPT "$$ "
#define BUFFER_SIZE 128

#include "usart.h"
#include "process_cmd.h"

int main(void) {
    /*
	 * Clocks: Processor = 48 Mhz. AHB = 48 MHz. APB = 24 MHz.
	 *
	 */
    char buf[BUFFER_SIZE]; // Buffer to store received string
    int idx = 0;   // Index for buffer

    // Initialize USART
    usart_init();
    // Initialize LED
    uled_init();

    // Optional cbfifo test - use only in debugging
    // test_cbfifo();

    printf("\r\n$$ Welcome to SerialIO!\r\n");

    while (1) {
        // Print the prompt "$$ "
        printf("$$ ");

        // Reset buffer index
        idx = 0;

        // Loop until newline is received
        while (1) {
            char ch = __io_getchar(); // Get a character

            if (ch == '\n' || ch == '\r') {
                // End of input string (newline or carriage return)
                buf[idx] = '\0';           // Null-terminate the string
                usart_transmit("\r\n", 2); // Send newline for the terminal
                break;
            } else if (ch == '\b' || ch == 127) { // Backspace or Delete key
                if (idx > 0) {
                    // Remove the last character in buffer
                    idx--;
                    usart_transmit("\b \b", 3); // Erase character from terminal
                }
            } else if (idx < sizeof(buf) - 1) {
                // Store character in buffer and echo it back
                buf[idx++] = ch;
                usart_transmit(&ch, 1); // Echo back the character
            }
        }
        // Process command:
        process_command(buf, idx);
    }
}
