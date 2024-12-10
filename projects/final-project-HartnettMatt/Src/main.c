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
 * @brief Matt Hartnett's main file for ECEN5813 Final Project
 * TODO: Finish this
 *
 * @author  Matt Hartnett
 * @date    12/3/2024
 *
 */

#include <stm32f091xc.h>
#include <stdio.h>
#include "uled.h"
#include "analog_out.h"
#include "usart.h"
#include "usart.h"
#include "process_time.h"

// TODO: Solve tearing on analog output

#define F_SYS_CLK (48000000L)
#define PROMPT "$$ "
#define TXT_BUFFER_SIZE 128
#define TWO_PI 6 // TODO: Fix this


// Tone buffer
// TODO: Check if these are the tones we want to generate
int16_t a4_samples[BUFFER_SIZE];
int16_t d5_samples[BUFFER_SIZE];

// Keep track of time
static int current_time = 0;
static int alarm_time = 0;
static int tone_index = 0;

// TODO: Change to 1 second
/**
 * Initializes the SysTick timer to generate periodic interrupts.
 *
 * This function configures the SysTick timer to produce an interrupt
 * every 100ms. The timer is initialized with a specific reload value
 * and interrupt priority, and then enabled.
 */
void Init_SysTick(void) {
    // SysTick is defined in core_cm0.h
    // Set reload to get 100ms interrupts
    SysTick->LOAD = (F_SYS_CLK / 80) - 1;

    // Set interrupt priority
    NVIC_SetPriority(SysTick_IRQn, 3);
    // Force load of reload value
    SysTick->VAL = 0;
    // Enable interrupt, enable SysTick timer
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}


// TODO Completely re-do this guy to do as little as possible
/**
 * SysTick interrupt handler for tone switching.
 *udo
 * This handler increments a time counter and switches tones every 2 seconds.
 * When the time threshold is reached, it disables the DMA, updates the DMA
 * buffer with the next tone's sample data, resets the counter, and restarts
 * the DMA. The tones cycle through a predefined set of four options.
 */
void SysTick_Handler(void) {

    current_time++;
    if (current_time >= 20) {                // Switch tone every 2 seconds
        DMA2_Channel3->CCR &= ~DMA_CCR_EN; // Disable DMA
        current_time = 0;
        tone_index = (tone_index + 1) % 4; // Cycle through 4 tones

        // Change the DMA buffer to the new tone
        switch (tone_index) {
        case 0:
            DMA2_Channel3->CMAR = (uint32_t)a4_samples;
            DMA2_Channel3->CNDTR = A4_BLOCK;
            set_blk_size(A4_BLOCK);
            break;
        case 1:
            DMA2_Channel3->CMAR = (uint32_t)d5_samples;
            DMA2_Channel3->CNDTR = D5_BLOCK;
            set_blk_size(D5_BLOCK);
            break;
        }

        // Restart the DMA
        DMA2_Channel3->CCR |= DMA_CCR_EN; // Enable DMA
    }
}
// Returns length of string
// TODO: Write comment for this function
// TODO: Add buffer overflow protection
int user_input(char *str, size_t strlen) {
    int str_idx = 0;      // Index for string buffer
    while (1) {
        char ch = __io_getchar(); // Get a character
        if (ch == '\n' || ch == '\r') {
            // End of input string (newline or carriage return)
            str[str_idx] = '\0';   // Null-terminate the string
            usart_transmit("\r\n", 2); // Send newline for the terminal
            return str_idx;
        } else if (ch == '\b' || ch == 127) { // Backspace or Delete key
            if (str_idx > 0) {
                // Remove the last character in buffer
                str_idx--;
                usart_transmit("\b \b", 3); // Erase character from terminal
            }
        } else if (str_idx < strlen - 1) {
            // Store character in buffer and echo it back
            str[str_idx++] = ch;
            usart_transmit(&ch, 1); // Echo back the character
        }
    }
    return -1; // Error condition
}

int main(void) {
    /*
	 * Clocks: Processor = 48 Mhz. AHB = 48 MHz. APB = 24 MHz.
	 *
	 */

    // Generate all of my tone buffers first
    tone_to_samples(A4_FREQ, A4_STEP, A4_BLOCK, a4_samples, BUFFER_SIZE);
    tone_to_samples(D5_FREQ, D5_STEP, D5_BLOCK, d5_samples, BUFFER_SIZE);

    // Initialize peripherals
    usart_init();
    uled_init();
    analog_out_init(a4_samples, A4_BLOCK);
    set_blk_size(A4_BLOCK);
    // TODO: Do I need to use analog input or could I use digital input? (be careful with voltage levels, don't want to damage a pin)
    // Ask user for system parameters:
    char str_buf[TXT_BUFFER_SIZE]; // Recieve string buffer
    int str_size;
    int time = -1;
    // Loop until a valid input is recieved:
    while(time == -1){
        printf("What time is it?\r\n");
        str_size = user_input(str_buf, TXT_BUFFER_SIZE);
        time = process_time(str_buf, str_size);
    }
    current_time = time;
    time = -1;
    while(time == -1){
        printf("When do you want to wake up?\r\n");
        str_size = user_input(str_buf, TXT_BUFFER_SIZE);
        time = process_time(str_buf, str_size);
    }
    alarm_time = time;
    printf("Goodnight! See you at %s\r\n", str_buf);

    
//    Init_SysTick();
    // TODO: Figure out sleep and such

    while (1) {
        // TODO: Add logic for checking time and resetting and such

    }
}
