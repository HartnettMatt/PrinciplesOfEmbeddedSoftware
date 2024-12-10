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
#include "process_cmd.h"

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
static int time_count = 0;
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
 *
 * This handler increments a time counter and switches tones every 2 seconds.
 * When the time threshold is reached, it disables the DMA, updates the DMA
 * buffer with the next tone's sample data, resets the counter, and restarts
 * the DMA. The tones cycle through a predefined set of four options.
 */
void SysTick_Handler(void) {

    time_count++;
    if (time_count >= 20) {                // Switch tone every 2 seconds
        DMA2_Channel3->CCR &= ~DMA_CCR_EN; // Disable DMA
        time_count = 0;
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

int main(void) {
    /*
	 * Clocks: Processor = 48 Mhz. AHB = 48 MHz. APB = 24 MHz.
	 *
	 */
//    char buf[TXT_BUFFER_SIZE]; // Buffer to store received string
//    int idx = 0;   // Index for buffer

    // Generate all of my tone buffers first
    tone_to_samples(A4_FREQ, A4_STEP, A4_BLOCK, a4_samples, BUFFER_SIZE);
    tone_to_samples(D5_FREQ, D5_STEP, D5_BLOCK, d5_samples, BUFFER_SIZE);

    // Initialize peripherals
    usart_init();
    uled_init();
    analog_out_init(a4_samples, A4_BLOCK);
    set_blk_size(A4_BLOCK);
    // TODO: Do I need to use analog input or could I use digital input? (be careful with voltage levels, don't want to damage a pin)
    // TODO: UART init (maybe, depends on if we can read with default UART)

    // TODO: Ask user for time and alarm, set system parameters

    printf("What time is it?\r\n");
    printf("Goodnight!");
    Init_SysTick();
    // TODO: Figure out sleep and such

    while (1) {
        // TODO: Add logic for checking time and resetting and such
    }
}
