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
#include "sine.h"
#include "button.h"

// TODO: Solve tearing on analog output

#define F_SYS_CLK (48000000L)
#define TXT_BUFFER_SIZE 128
#define X10_SECONDS_IN_DAY 864000
#define C5_FREQ 523 // Frequency of alarm - C5 is best note to wake a person up


// Tone buffer
uint16_t c4_samples[SINE_BUFFER_SIZE];

// Keep track of time
static int current_time = 0;
static int alarm_time = 0;
static int alarm_flag = 0;
static int tone_counter = 0;

/**
 * Initializes the SysTick timer to generate periodic interrupts.
 *
 * This function configures the SysTick timer to produce an interrupt
 * every 100ms. The timer is initialized with a specific reload value
 * and interrupt priority, and then enabled.
 * Systick occurs every 100ms to maintain responsivness when turning off the alarm
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
 *
 * This handler increments a time counter and switches tones every 2 seconds.
 * When the time threshold is reached, it disables the DMA, updates the DMA
 * buffer with the next tone's sample data, resets the counter, and restarts
 * the DMA. The tones cycle through a predefined set of four options.
 */
void SysTick_Handler(void) {
    if (current_time < X10_SECONDS_IN_DAY) {
        current_time++;
    } else {
        current_time = 0;
    }
    if (current_time == alarm_time) { // Start alarm
        alarm_flag = 1;
    } else if (alarm_flag == 2) {
        if(tone_counter < 20){
            tone_counter++;
        } else {
            tone_counter = 0;
        }
    } else {
        tone_counter = 0; // Make sure this is reset
    }
}
// Returns length of string
// TODO: Write comment for this function
int user_input(char *str, size_t strlen) {
    int str_idx = 0; // Index for string buffer
    while (1) {
        char ch = __io_getchar(); // Get a character
        if (ch == '\n' || ch == '\r') {
            // End of input string (newline or carriage return)
            str[str_idx] = '\0';       // Null-terminate the string
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

    // Generate tone buffer first - only do once, a lot of math involved
    int sample_cnt = compute_sine_wave(C5_FREQ, c4_samples, SINE_BUFFER_SIZE, DAC_SAMPLE_RATE, DAC_RESOLUTION);

    // Initialize most peripherals
    usart_init();
    uled_init();
    button_init();
    analog_out_init(c4_samples, sample_cnt);
    set_blk_size(sample_cnt);
    // TODO: Do I need to use analog input or could I use digital input? (be careful with voltage levels, don't want to damage a pin)
    // Ask user for system parameters:
    char str_buf[TXT_BUFFER_SIZE]; // Recieve string buffer
    int str_size;
    int time = -1;
    // Loop until a valid input is recieved:
    while (time == -1) {
        printf("What time is it?\r\n");
        str_size = user_input(str_buf, TXT_BUFFER_SIZE);
        time = process_time(str_buf, str_size);
    }
    // Needs to be multiplied by 10 due to systick occuring every 100ms
    // Systick occurs every 100ms to maintain responsivness when turning off the alarm
    current_time = 10 * time;
    time = -1;
    while (time == -1) {
        printf("When do you want to wake up?\r\n");
        str_size = user_input(str_buf, TXT_BUFFER_SIZE);
        time = process_time(str_buf, str_size);
    }
    alarm_time = 10 * time;
    printf("Goodnight! See you at %s\r\n", str_buf);

    Init_SysTick();
    // TODO: Figure out sleep and such

    while (1) {
        // TODO: Add logic for checking time and resetting and such
        if (alarm_flag == 1) {
            printf("Good morning!");
            // Start the alarm
            analog_out_start();
            alarm_flag = 2;
        } else if (alarm_flag == 2) {
            // Alarm turn off condition
            if(read_button() == 1){
                analog_out_stop();
                alarm_flag = 0;
            } else {
                if(tone_counter < 10){
                    analog_out_start();
                } else {
                    analog_out_stop();
                }
            }
        }
    }
}
