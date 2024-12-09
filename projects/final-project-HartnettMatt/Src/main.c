/**
 ******************************************************************************
 * @file           : main.c
 * @author         : mhartnett
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "analog_in.h"
#include "test_sine.h"
#include "analog_out.h"
#include "analysis.h"
#include <stdio.h>
#include <stm32f091xc.h>

#define F_SYS_CLK (48000000L)

// Tone buffer
int16_t a4_samples[BUFFER_SIZE];
int16_t d5_samples[BUFFER_SIZE];
int16_t e5_samples[BUFFER_SIZE];
int16_t a5_samples[BUFFER_SIZE];

// Print flag
static int flag = 0;
static int tone_index = 0;
static int time_count = 0;

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
    if (time_count >= 20) { // Switch tone every 2 seconds
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
        case 2:
            DMA2_Channel3->CMAR = (uint32_t)e5_samples;
            DMA2_Channel3->CNDTR = E5_BLOCK;
            set_blk_size(E5_BLOCK);
            break;
        case 3:
            DMA2_Channel3->CMAR = (uint32_t)a5_samples;
            DMA2_Channel3->CNDTR = A5_BLOCK;
            set_blk_size(A5_BLOCK);
            break;
        }

        // Restart the DMA
        DMA2_Channel3->CCR |= DMA_CCR_EN;  // Enable DMA
        flag = 0;
    }
}

int main(void) {
    /* You can use the Command Shell Console in STM32CubeIDE or an
	 * external program such as Putty or TeraTerm to view printf output.
	 * The settings are: Baud: 9600, Parity: None, Stop Bits: 1 with
	 * the ST-Link COM port setting.
	 * Clocks: Processor = 48 Mhz. AHB = 48 MHz. APB = 24 MHz.
	 */

    // Generate all of my tone buffers first
    tone_to_samples(A4_FREQ, A4_STEP, A4_BLOCK, a4_samples, BUFFER_SIZE);
    tone_to_samples(D5_FREQ, D5_STEP, D5_BLOCK, d5_samples, BUFFER_SIZE);
    tone_to_samples(E5_FREQ, E5_STEP, E5_BLOCK, e5_samples, BUFFER_SIZE);
    tone_to_samples(A5_FREQ, A5_STEP, A5_BLOCK, a5_samples, BUFFER_SIZE);

    analog_out_init(a4_samples, A4_BLOCK);
    analog_in_init();
    set_blk_size(A4_BLOCK);
    Init_SysTick();
    analog_out_start();
    int16_t *adc_values;

    // Use the main loop to handle printing and analysis so it doesn't bog down the timing of the actual tone generation
    while (1) {
        if(flag == 0 && time_count >= 10){
            switch (tone_index) {
            case 0:
                printf("Generated %d samples at %d Hz; computed period=%d samples\r\n", (A4_BLOCK_CNT*A4_BLOCK), A4_FREQ, A4_BLOCK);
                break;
            case 1:
                printf("Generated %d samples at %d Hz; computed period=%d samples\r\n", (D5_BLOCK_CNT*D5_BLOCK), D5_FREQ, A4_BLOCK);
                break;
            case 2:
                printf("Generated %d samples at %d Hz; computed period=%d samples\r\n", (E5_BLOCK_CNT*E5_BLOCK), E5_FREQ, A4_BLOCK);
                break;
            case 3:
                printf("Generated %d samples at %d Hz; computed period=%d samples\r\n", (A5_BLOCK_CNT*A5_BLOCK), A5_FREQ, A4_BLOCK);
                break;
            }

            adc_values = analog_in_sample();
            analyze_and_print(adc_values, ADC_BUFFER_SIZE);
            flag = 1;
        }
    }

    return 0;
}
