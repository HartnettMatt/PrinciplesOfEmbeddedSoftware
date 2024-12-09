/*
 * analog_in.c: A ADC module that utilizes the DMA
 *
 *  Created on: Nov 25, 2024
 *      Author: mhartnett
 */

#include "analog_in.h"
#include <stdint.h>
#include "utilities.h"
#include <stm32f091xc.h>

static int adc_en;
static int sample_count;

int16_t adc_samples[ADC_BUFFER_SIZE];

/**
 * Initializes TIM7 to generate interrupts every 100 ms.
 *
 * This function configures TIM7 to use a 48 MHz system clock divided down
 * to 1 kHz (1 ms tick) using the prescaler. The auto-reload value is set
 * to generate an update event every 100 ticks (100 ms). The function also
 * enables the TIM7 interrupt in the NVIC.
 *
 */
void tim7_init(void) {
    // Enable clock for TIM7
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;

    // Configure the timer
    TIM7->PSC = TIM7_PRESCALER - 1; // Prescaler value: (48MHz / 1000) = 48kHz
    TIM7->ARR = 1;                  // Auto-reload value: 48kHz * 2 = 96kHz
    TIM7->DIER |= TIM_DIER_UIE;     // Enable update interrupt
    TIM7->CR1 |= TIM_CR1_CEN;       // Enable the timer

    // Enable the TIM7 interrupt in the NVIC
    NVIC_SetPriority(TIM7_IRQn, 1); // Set interrupt priority (lower numbers are higher priority)
    NVIC_EnableIRQ(TIM7_IRQn);      // Enable TIM7 interrupt
}

/**
 * TIM7 interrupt handler.
 *
 * This function is called when TIM7 generates an update event interrupt.
 * It clears the interrupt flag and provides a location to add custom
 * interrupt-handling code.
 *
 * Example applications include toggling an LED, updating a counter,
 * or triggering periodic tasks.
 *
 */
void TIM7_IRQHandler(void) {
    if (TIM7->SR & TIM_SR_UIF) { // Check if update interrupt flag is set
        TIM7->SR &= ~TIM_SR_UIF; // Clear the interrupt flag
        if(adc_en && sample_count < ADC_BUFFER_SIZE){
            // Get value from ADC
            ADC1->CR |= ADC_CR_ADSTART;
            while(!(ADC1->ISR & ADC_ISR_EOC));
            // Store value in ADC buffer
            adc_samples[sample_count] = ADC1->DR;
            sample_count++;
        }
    }
}

int16_t* analog_in_sample(void){
    sample_count = 0;
    adc_en = 1;
    while(sample_count < ADC_BUFFER_SIZE); // Spin until all samples are collected
    adc_en = 0;
    return adc_samples;
}

void analog_in_init(void){
    tim7_init();
    adc_en = 0;
    sample_count = 0;

    // This section of code was copied from Alex Dean's Github and modified by Matt Hartnett
    // Enable peripheral clock of ADC
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
	// Enable peripheral clock of GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // Configure PA4 as analog input to ADC
    MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER4, ESF_GPIO_MODER_ANALOG);

	// Oscillator: Enable and Select HSI14 (from STM32F0 Reference Manual, A.7.4)
	RCC->CR2 |= RCC_CR2_HSI14ON;	/* (2) Start HSI14 RC oscillator */
	while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0) {	/* (3) Wait HSI14 is ready */
		/* For robust implementation, add here time-out management */
	}
	/* Select HSI14 with CKMODE=00 */
	MODIFY_FIELD(ADC1->CFGR2, ADC_CFGR2_CKMODE, 0);
	// Init ADCl
	MODIFY_FIELD(ADC1->SMPR, ADC_SMPR_SMP, 0);	// SMP = 000 for minimum sample time

	/* CFGR1: The default configuration (CFGR1 = 0) matches what we want:
	   some features are disabled (analog watchdog, discontinous conversion
	   mode, auto-off mode, wait conversion mode, continuous conversion mode,
	   hardware trigger) and other features are selected: software trigger,
	   right-aligned data, 12-bit resolution. */
	ADC1->CFGR1 = 0;

	ADC1->CHSELR = ADC_CHSELR_CHSEL4;	// Select ADC input channel 4

	// Enable ADC (from STM32F0 Reference Manual, A.7.2
	if ((ADC1->ISR & ADC_ISR_ADRDY) != 0) {	/* (1) Ensure that ADRDY = 0 */
		ADC1->ISR |= ADC_ISR_ADRDY;	/* (2) Clear ADRDY */
	}
	ADC1->CR |= ADC_CR_ADEN;			/* (3) Enable the ADC */
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) {	/* (4) Wait until ADC ready */
		/* For robust implementation, add here time-out management */
	}
}