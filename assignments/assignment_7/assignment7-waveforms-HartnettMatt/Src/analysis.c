/*
 * analysis.c: Perform analysis on a set of samples and return the output.
 *
 *  Created on: Nov 25, 2024
 *      Author: mhartnett
 */

#include "analysis.h"
#include "autocorrelate.h"
#include "analog_in.h" // Necessary for ADC sample rate
#include <stdio.h>

void analyze(int16_t *buffer, uint16_t buffer_size, int *min_value, int *max_value, int *average, int *period, int *frequency) {
    int min = buffer[0];
    int max = buffer[0];
    int sum = 0;
    for (int i = 0; i < buffer_size; i++) {
        if (buffer[i] < min) {
            min = buffer[i];
        }
        if (buffer[i] > max) {
            max = buffer[i];
        }
        sum = sum + buffer[i];
    }
    autocorrelate_sample_format_t format = kAC_12bps_signed;
    int per = autocorrelate_detect_period(buffer, buffer_size, format);


    *average = sum >> 10; // Only works if buffer size is 1024!
    *min_value = min;
    *max_value = max;
    *period = per;
    if (per > 0) {
        *frequency = ADC_SAMPLE_RATE / per;
    } else {
        *frequency = 0; // Handle the case where the period is invalid
    }
}

void analyze_and_print(int16_t *buffer, uint16_t buffer_size) {
    int min_value = 0;
    int max_value = 0;
    int average = 0;
    int period = 0;
    int frequency = 0;
    analyze(buffer, buffer_size, &min_value, &max_value, &average, &period, &frequency);
    printf("min=%d max=%d avg=%d period=%d samples frequency=%d Hz \r\n", min_value, max_value, average, period, frequency);
}
