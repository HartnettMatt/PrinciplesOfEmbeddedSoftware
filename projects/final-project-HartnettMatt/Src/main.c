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
 * This project is a motion detecting alarm clock. It queries the user
 * for the current time as well as the alarm time, then goes to sleep and waits.
 * Once the alarm time has occured, it begins beeping. If it detects motion, it stops
 * beeping and waits until tomorrow to go off again.
 *
 * @author  Matt Hartnett
 * @date    12/3/2024
 *
 */

#include <stm32f091xc.h>
#include <stdio.h>
#include "dig_in.h"
#include "analog_out.h"
#include "usart.h"
#include "usart.h"
#include "process_time.h"
#include "sine.h"
#include "button.h"
#include "sleep.h"
#include "systick.h"

#define TXT_BUFFER_SIZE 128
#define X10_SECONDS_IN_DAY 864000
#define C5_FREQ 523 // Frequency of alarm - C5 is best note to wake a person up
#define TONE_ON_TIME 8
#define TONE_OFF_TIME 5
#define TONE_PERIOD (TONE_ON_TIME + TONE_OFF_TIME)
#define MIN_TONE_COUNT 5 // How many tones go off before movement can turn off the alarm

typedef enum {
    ALARM_FLAG_IDLE,      // Alarm is idle - only on bootup when waiting for user input
    ALARM_FLAG_ARMED,     // Alarm is armed
    ALARM_FLAG_TRIGGERED, // Alarm has been triggered
    ALARM_FLAG_RUNNING    // Alarm is running
} alarm_flag_t;

// Tone buffer
uint16_t c4_samples[SINE_BUFFER_SIZE];
// Keep track of time
static int current_time = 0;
static int alarm_time = 0;
// Keep track of alarm state
static alarm_flag_t alarm_flag = ALARM_FLAG_IDLE;
// Keep track of tone timing
static int tone_timer = 0;
static int tone_count = 0;

/**
 * SysTick interrupt handler for tone switching.
 *
 * This handler mostly just increments the current time.
 * If current time is equal to alarm time, the alarm enters
 * the triggered state, and if it's in the running state,
 * it increments through the tone playback.
 */
void SysTick_Handler(void) {
    if (current_time < X10_SECONDS_IN_DAY) {
        current_time++;
    } else {
        current_time = 0;
    }

    if (current_time == alarm_time) { // Start alarm
        alarm_flag = ALARM_FLAG_TRIGGERED;
    } else if (alarm_flag == ALARM_FLAG_RUNNING) { // Manage alarm tone timing
        if (tone_timer < TONE_PERIOD) {
            tone_timer++;
        } else {
            tone_timer = 0;
            tone_count++;
        }
    } else {
        // Reset these if the alarm is off
        tone_timer = 0;
        tone_count = 0;
    }
}

/**
 * @brief Captures user input from the terminal and stores it in a buffer.
 *
 * Reads characters from the terminal, handles backspace for editing,
 * and terminates the input on newline or carriage return. The input is
 * stored in the provided buffer and null-terminated.
 *
 * @param[out] str    Buffer to store the input string.
 * @param[in]  strlen Size of the buffer, including space for the null terminator.
 *
 * @return Length of the captured string (excluding the null terminator),
 *         or -1 in case of an error.
 */
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

    // Variables used throughout main loop
    int sensor_val = 0;
    char str_buf[TXT_BUFFER_SIZE]; // Recieve string buffer
    int str_size;
    int user_time = -1;

    // Initialize peripherals
    usart_init();
    button_init();
    analog_out_init(c4_samples, sample_cnt);
    set_blk_size(sample_cnt);
    dig_in_init();
    sleep_init();
    systick_init();

    // Power On Self Test (POST)
    #ifdef DEBUG
    printf("RUNNING POWER ON SELF TEST...\r\n");
    usart_test();
    dig_in_test();
    printf("POWER ON SELF TEST COMPLETE\r\n");
    #endif // DEBUG

    // Primary state machine loop:
    while (1) {
        // Alarm state logic
        switch (alarm_flag) {
        case ALARM_FLAG_IDLE:
            // Ask user for system parameters:
            // Loop until a valid input is recieved:
            while (user_time == -1) {
                printf("What time is it?\r\n");
                str_size = user_input(str_buf, TXT_BUFFER_SIZE);
                if (str_size != -1) {
                    user_time = process_time(str_buf, str_size);
                }
            }
            // Needs to be multiplied by 10 due to systick occuring every 100ms
            current_time = 10 * user_time;
            user_time = -1;

            // Start counting time immediatly - don't want to be inaccurate if user is slow setting alarm
            systick_start();

            // Ask for alarm time
            while (user_time == -1) {
                printf("When do you want to wake up?\r\n");
                str_size = user_input(str_buf, TXT_BUFFER_SIZE);
                if (str_size != -1) {
                    user_time = process_time(str_buf, str_size);
                }
            }
            // Needs to be multiplied by 10 due to systick occuring every 100ms
            alarm_time = 10 * user_time;

            alarm_flag = ALARM_FLAG_ARMED;
            printf("Goodnight! See you at %s\r\n", str_buf);
            break;
        case ALARM_FLAG_ARMED:
            // Just go back to sleep - alarm check is handled in systick to ensure immediate switch to triggered
            break;
        case ALARM_FLAG_TRIGGERED:
            // Start the alarm
            printf("Good morning!\r\n");
            analog_out_start();
            alarm_flag = ALARM_FLAG_RUNNING;
            break;
        case ALARM_FLAG_RUNNING:
            // Alarm turn off check
            sensor_val = dig_in_read();
            if ((tone_count >= MIN_TONE_COUNT && sensor_val == 1) || read_button() == 1) {
                analog_out_stop();
                printf("Glad to see you awake!\r\n");
                alarm_flag = ALARM_FLAG_ARMED;
            } else {
                if (tone_timer < TONE_ON_TIME) {
                    analog_out_start();
                } else {
                    analog_out_stop();
                }
            }
            break;
        }
        enter_sleep(); // Go to sleep - only woken up by DMA or SysTick interrupts
    }
}
