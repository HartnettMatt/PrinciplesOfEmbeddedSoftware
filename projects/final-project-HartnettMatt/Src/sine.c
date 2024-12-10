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
 * @file    sine.c
 * @brief Matt Hartnett's sine computation file.
 *
 * @author  Matt Hartnett
 * @date    12/05/2024
 *
 */
#include "sine.h"
#include <stdint.h>
#include <math.h>

int compute_sine_wave(int frequency, uint16_t *buffer, int bufferSize, uint16_t sampling_frequency, uint16_t resolution) {
    if (frequency <= 0 || buffer == NULL || bufferSize <= 0 || sampling_frequency <= 0 || resolution <= 0) {
        return 0; // Invalid input
    }

    int samplesPerPeriod = sampling_frequency / frequency;
    int max_size = pow(2, (resolution - 1)) - 10;
    if (samplesPerPeriod > bufferSize) {
        samplesPerPeriod = bufferSize; // Limit to buffer size
    }

    // Precompute step size using fixed-point arithmetic
    uint32_t phaseIncrement = (1UL << 24) / samplesPerPeriod; // Fixed-point phase increment
    uint32_t phase = 0;                                       // Fixed-point phase accumulator

    for (int i = 0; i < samplesPerPeriod; i++) {
        // Compute sine using integer approximation
        float angle = (float)(phase)*2.0f * (float)M_PI / (1UL << 24);
        int sineValue = (int16_t)(max_size * sinf(angle)); // Scale to max resolution
        buffer[i] = (uint16_t)(sineValue + max_size);      // Scale to unsigned
        phase += phaseIncrement;
    }

    return samplesPerPeriod; // Return number of valid data points
}