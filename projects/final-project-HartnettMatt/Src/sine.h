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
 * @file    sine.h
 * @brief Matt Hartnett's sine computation header file.
 *
 * @author  Matt Hartnett
 * @date    12/05/2024
 *
 */

#ifndef _SINE_H
#define _SINE_H

#include <stdint.h>

/**
 * @brief Computes one period of a sine wave and stores it in the provided buffer.
 *
 * This function is quite computationally expensive - do not use frequently.
 *
 * @param frequency The frequency of the sine wave in Hz.
 * @param buffer Pointer to an array of int16_t to store the sine wave values.
 * @param bufferSize The size of the buffer in number of samples.
 * @param sampling_frequency The sampling frequency of the DAC
 * @param resolution The resolution of the DAC in number of bits
 * @return int The number of valid data points stored in the buffer.
 */
int compute_sine_wave(int frequency, uint16_t *buffer, int bufferSize, uint16_t sampling_frequency, uint16_t resolution);

#endif // _SINE_H