/*
 * analog_out.h: An DAC module that utilizes the DMA
 *
 *  Created on: Nov 25, 2024
 *      Author: mhartnett
 */

#ifndef ANALOG_OUT_H_
#define ANALOG_OUT_H_
#include <stdint.h>

// Configuration constants:
#define DAC_SAMPLE_RATE 48000    // Desired sample rate in Hz
#define DAC_RESOLUTION 12        // Number of bits per sample

// Other constants:
#define ESF_GPIO_MODER_ANALOG 	(3)
#define MAX_DAC_CODE 4095
#define SINE_BUFFER_SIZE 128

// Timer constants:
#define F_TIM6_CLOCK 48000000
#define TIM6_PRESCALER (F_TIM6_CLOCK/DAC_SAMPLE_RATE)/2 // 500

// Muiscal Notes:
/**
 * Set block size for DMA reset
 *
 */
void set_blk_size(int block_size);


/**
 * Initializes the DAC
 * Configures the DAC to operate and sets the output to pin PC5
 *
 */
void dac_init(void);

/**
 * Initializes the analog playback system.
 * Configures the DAC, TIMER6, and DMA to play the provided buffer repeatedly.
 *
 * @param buffer Pointer to the analog sample buffer (16-bit samples)
 * @param buffer_size Size of the analog sample buffer
 */
void analog_out_init(uint16_t *buffer, uint16_t buffer_size);

/**
 * Starts analog playback.
 */
void analog_out_start(void);

/**
 * Stops analog playback.
 */
void analog_out_stop(void);

/**
 * Generates a 1 period block of tone samples based on the given frequency and step size.
 *
 * This function fills the provided buffer with `block_size` samples of a tone.
 * The tone is generated using a sine wave function and scaled to fit within
 * the range of int16_t. The phase of the sine wave is updated incrementally
 * using the specified `step_size`.
 *
 * @param frequency The frequency of the tone in Hertz (not directly used in this function).
 * @param step_size The step increment for the phase accumulator, determining the tone's frequency.
 * @param block_size The number of samples to generate in the buffer.
 * @param buffer Pointer to the int16_t buffer where samples will be stored.
 * @param buffer_size The size of the provided buffer (must be >= block_size).
 *
 * @return 0 on success, or -1 if the provided buffer is too small to hold the requested samples.
 *
 */
int tone_to_samples(uint32_t frequency, uint32_t step_size, uint32_t block_size, int16_t *buffer, uint32_t buffer_size);

#endif /* ANALOG_OUT_H_ */