/*
 * analog_out.c: An DAC module that utilizes the DMA
 *
 *  Created on: Nov 25, 2024
 *      Author: mhartnett
 */

#include "analog_out.h"
#include "utilities.h"
#include <stm32f091xc.h>

int blk_size;

void set_blk_size(int block_size) {
    blk_size = block_size;
}

// This init function was taken from Alex Dean's github: https://github.com/alexander-g-dean/ESF/blob/master/ST/Code/common-driver/dac.c
void dac_init(void) {
    // Enable clocks for DAC and PA4
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Init PA4 as analog by setting both MODER bits
    MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER4, ESF_GPIO_MODER_ANALOG);

    // Init DAC1, connecting it to PA4
    DAC1->CR = DAC_CR_EN1;
}

/**
 * Initializes TIM6 for periodic DMA requests.
 *
 * This function enables the TIM6 clock, sets the auto-reload value (ARR)
 * and prescaler (PSC) for the desired DAC frequency.
 */
void tim6_init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->ARR = 1;
    TIM6->PSC = TIM6_PRESCALER - 1;
    // Enable DMA request
    TIM6->DIER = TIM_DIER_UDE;
    // Enable counting
    TIM6->CR1 |= TIM_CR1_CEN;
}

/**
 * DMA1 Channel 2 and 3 interrupt handler.
 *
 * This handler checks for an interrupt flag, resets the DMA block size to the required value,
 * and clears the interrupt flag to ensure proper DMA operation.
 */
void DMA1_Channel2_3_IRQHandler(void) {
    if (DMA2->ISR) {
        DMA2_Channel3->CNDTR = blk_size; // Reset to whatever block size is required
    }
    DMA2->IFCR |= 1; // Clear flag
}

void analog_out_init(uint16_t *buffer, uint16_t buffer_size) {
    // Enable clocks for DAC, TIMER6, and DMA
    RCC->APB1ENR |= RCC_APB1ENR_DACEN | RCC_APB1ENR_TIM6EN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // Configure DAC
    dac_init();

    // Configure timer
    tim6_init();

    // Configure DMA for DAC
    // Enable DMA2 clock
    RCC->AHBENR |= RCC_AHBENR_DMA2EN;
    DMA2_Channel3->CCR = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_CIRC;
    MODIFY_FIELD(DMA2_Channel3->CCR, DMA_CCR_MSIZE, 1);
    MODIFY_FIELD(DMA2_Channel3->CCR, DMA_CCR_PSIZE, 1);
    MODIFY_FIELD(DMA2_Channel3->CCR, DMA_CCR_PL, 3);

    // Configure DMA IRQ
    NVIC_SetPriority(DMA1_Ch4_7_DMA2_Ch3_5_IRQn, 3);
    NVIC_ClearPendingIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
    NVIC_EnableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);

    MODIFY_FIELD(DMA2->CSELR, DMA_CSELR_C3S, 1);
    DMA2_Channel3->CNDTR = buffer_size;            // Number of data items to transfer
    DMA2_Channel3->CMAR = (uint32_t)buffer;        // Memory address
    DMA2_Channel3->CPAR = (uint32_t)&DAC->DHR12R1; // DAC data register
}

void analog_out_start(void) {
    // Ensure DMA and DAC are enabled
    DMA2_Channel3->CCR |= DMA_CCR_EN;
    TIM6->CR1 |= TIM_CR1_CEN;
}


void analog_out_stop(void) {
    // Disable DMA and Timer
    DAC->DHR12R1 = 0;
    DMA1_Channel1->CCR &= ~DMA_CCR_EN;
    TIM6->CR1 &= ~TIM_CR1_CEN;
}

int tone_to_samples(uint32_t frequency, uint32_t step_size, uint32_t block_size, int16_t *buffer, uint32_t buffer_size) {

    if (block_size > buffer_size) {
        return -1; // Buffer too small
    }

    int32_t current_phase = 0;

    for (int32_t i = 0; i < block_size; ++i) {
        //        int32_t raw_sample = fp_sin(current_phase);
        int32_t raw_sample = 1;
        buffer[i] = (int16_t)(raw_sample + 1);
        current_phase = (current_phase + step_size);
    }

    return 0;
}