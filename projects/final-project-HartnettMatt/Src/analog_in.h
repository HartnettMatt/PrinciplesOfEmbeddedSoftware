/*
 * analog_in.c: A ADC module that utilizes the DMA
 *
 *  Created on: Nov 25, 2024
 *      Author: mhartnett
 */
#ifndef ANALOG_IN_H_
#define ANALOG_IN_H_
#include <stdint.h>

// Configuration constants:
#define ADC_SAMPLE_RATE 96000 // Desired sample rate in Hz
#define ADC_RESOLUTION 12     // Number of bits per sample

#define ADC_BUFFER_SIZE 1024

// Timer constants:
#define F_TIM7_CLOCK 48000000
#define TIM7_PRESCALER (F_TIM7_CLOCK / ADC_SAMPLE_RATE) / 2
/**
* Collects a buffer of ADC samples.
*
* This function enables the ADC and continuously reads samples until the buffer
* is full. It uses the global `adc_samples` array to store the collected data.
* The function blocks until all samples are collected.
*
* @return A pointer to the array of collected ADC samples.
*/
int16_t *analog_in_sample(void);

/**
* Initializes the ADC and Timer 7 for analog input sampling.
*
* Configures the ADC to use pin PA4 (A04) as an analog input,
* and sets up Timer 7 to periodically trigger ADC sampling. This function
* enables the necessary clocks, configures the GPIO pin as an analog input,
* and ensures the ADC is ready to start conversions.
*
* Steps:
* - Enable peripheral clocks for the ADC and GPIOA.
* - Configure PA4 as an analog input.
* - Enable and configure the HSI14 oscillator for the ADC.
* - Set the ADC clock source, sample time, and other settings.
* - Initialize Timer 7 for periodic interrupts.
*/
void analog_in_init(void);


#endif /* ANALOG_IN_H_ */