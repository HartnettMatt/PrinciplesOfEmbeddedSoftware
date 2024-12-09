/*
 * analysis.h: Perform analysis on a set of samples and return the output.
 *
 *  Created on: Nov 25, 2024
 *      Author: mhartnett
 */


#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include <stdint.h>
#include "autocorrelate.h"

/**
 * Analyzes an input buffer for statistical and signal properties.
 *
 * This function calculates the minimum, maximum, and average values of a buffer of samples.
 * It also determines the signal period and frequency using autocorrelation.
 *
 * @param buffer Pointer to the buffer containing input samples.
 * @param buffer_size Number of samples in the buffer.
 * @param min_value Pointer to store the minimum value in the buffer.
 * @param max_value Pointer to store the maximum value in the buffer.
 * @param average Pointer to store the average value of the buffer.
 * @param period Pointer to store the detected period of the signal.
 * @param frequency Pointer to store the calculated frequency of the signal.
 *
 * Note: The average calculation assumes a buffer size of 1024 for the right shift operation.
 */
void analyze(int16_t *buffer, uint16_t buffer_size, int* min_value, int* max_value, int* average, int* period, int* frequency);

/**
 * Analyzes a buffer and prints the computed signal properties.
 *
 * This function uses the `analyze` function to calculate the minimum, maximum,
 * average, period, and frequency of a signal stored in the input buffer. It then
 * prints these values in a formatted output.
 *
 * @param buffer Pointer to the buffer containing input samples.
 * @param buffer_size Number of samples in the buffer.
 */
void analyze_and_print(int16_t *buffer, uint16_t buffer_size);

#endif /* ANALYSIS_H_ */