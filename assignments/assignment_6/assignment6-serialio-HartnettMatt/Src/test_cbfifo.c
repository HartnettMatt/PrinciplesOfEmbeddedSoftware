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
 * @file    test_cbfifo.c
 * @brief Matt Hartnett's work for ECEN5813 HW2.
 * This is the test file for the circular buffer based FIFO.
 *
 * @author  Matt Hartnett
 * @date    9/16/2024
 *
 */

#include "test_cbfifo.h"
#include "cbfifo.h"
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Test data array
static uint8_t data[BUFFER_SIZE];
cbfifo cb = {
    .write = 0,
    .read = 0,
    .is_full = 0};

/**
 * @brief Byte to int compare function
 *
 * @return 1 if byte and int are equal, 0 if not equal
 */
int byte_comp(uint8_t *arr, int val) {
    int cmp = 0;
    for (size_t i = 0; i < sizeof(int); i++) {
        cmp = cmp | (arr[i] << (32 - (4 * i)));
    }
    return cmp == val; // Byte array is equal to value
}


/**
 * @brief uint8_t array compare function
 *
 * @return 1 if arrays are equal, 0 if not equal
 */
int array_equal(uint8_t *arr1, uint8_t *arr2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (arr1[i] != arr2[i]) {
            return 0; // Arrays are not equal
        }
    }
    return 1; // Arrays are equal
}


/**
 * @brief Tests the cbfifo_enqueue function
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_cbfifo_enqueue() {
    int failure = 0;
    size_t result;
    for (int i = 0; i < sizeof(data); i++) {
        data[i] = (uint8_t)i;
    }
    uint8_t peek = 0;


    // Test point 1: Send a NULL buffer
    result = cbfifo_enqueue(&cb, NULL, 4);
    if (result != -1) {
        printf("Test point 1 failure. Running fifo state dump:\n\r");
        cbfifo_state_dump(&cb);
        failure++;
    }

    // Test point 2: Send a valid buffer but 0 nbyte
    result = cbfifo_enqueue(&cb, &data[0], 0);
    if (result != 0) {
        printf("Test point 2 failure. Running fifo state dump:\n\r");
        cbfifo_state_dump(&cb);
        failure++;
    }

    // Test point 3: Send a valid buffer and enqueue 1 byte
    result = cbfifo_enqueue(&cb, &data[0], 1);
    cbfifo_peek(&cb, &peek);
    if (result != 1 ||
        cbfifo_length(&cb) != 1 ||
        peek != data[0]) {
        printf("Test point 3 failure. Running fifo state/data dump:\n\r");
        cbfifo_state_dump(&cb);
        cbfifo_data_dump(&cb);
        printf("Peek: %d \n\r", peek);
        failure++;
    }

    // Test point 4: Send a valid buffer and enqueue another byte
    result = cbfifo_enqueue(&cb, &data[1], 1);
    cbfifo_peek(&cb, &peek);
    if (result != 1 ||
        cbfifo_length(&cb) != 2 ||
        peek != data[0]) {
        printf("Test point 4 failure. Running fifo state/data dump:\n\r");
        cbfifo_state_dump(&cb);
        cbfifo_data_dump(&cb);
        printf("Peek: %d \n\r", peek);
        failure++;
    }

    // Test point 5: Enqueue 256 bytes (should only enqueue BUFFER_SIZE-2 due to consumed bytes)
    result = cbfifo_enqueue(&cb, &data[2], 256);
    cbfifo_peek(&cb, &peek);
    if (result != (BUFFER_SIZE - 2) ||
        cbfifo_length(&cb) != BUFFER_SIZE ||
        peek != data[0]) {
        printf("Test point 5 failure. Running fifo state/data dump:\n\r");
        cbfifo_state_dump(&cb);
        cbfifo_data_dump(&cb);
        printf("Peek: %d \n\r", peek);
        printf("Result: %u \n\r", result);
        failure++;
    }

    // Test point 6: Enqueue 1 byte after full (should error out)
    result = cbfifo_enqueue(&cb, &data[0], 1);
    cbfifo_peek(&cb, &peek);
    if (result != -1 ||
        cbfifo_length(&cb) != BUFFER_SIZE ||
        peek != data[0]) {
        printf("Test point 6 failure. Running fifo state/data dump:\n\r");
        cbfifo_state_dump(&cb);
        cbfifo_data_dump(&cb);
        printf("Peek: 0x%02X \n\r", peek);
        failure++;
    }

    // Not much left to do without testing dequeue, more thorough testing to follow...

    if (failure) {
        printf("Total cbfifo_enqueue failures: %d\n\r", failure);
        return 0;
    }

    return 1;
}

/**
 * @brief Tests the cbfifo_dequeue function
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_cbfifo_dequeue() {
    int failure = 0;
    size_t result;
    uint8_t dq_data[BUFFER_SIZE];
    uint8_t zero[BUFFER_SIZE];
    memset(zero, 0, sizeof(zero));
    //    uint8_t peek = 0;

    // Test point 1: Dequeue with NULL buffer
    result = cbfifo_dequeue(&cb, NULL, 4);
    if (result != -1) {
        printf("Test point 1 failure. Running fifo state dump:\n\r");
        cbfifo_state_dump(&cb);
        failure++;
    }

    // Test point 2: Dequeue a single byte
    result = cbfifo_dequeue(&cb, dq_data, 1);
    if (result != 1 ||
        dq_data[0] != data[0] ||
        cbfifo_length(&cb) != (BUFFER_SIZE - 1)) {
        printf("Test point 2 failure. Running fifo state/data dump:\n\r");
        cbfifo_state_dump(&cb);
        cbfifo_data_dump(&cb);
        printf("Dequeued Data: 0x%02X\n\r", dq_data[0]);
        printf("Expected Data: 0x%02X\n\r", data[0]);
        printf("Result: %d\n\r", (int)result);
        failure++;
    }

    // Test point 3: Request more DQ than length
    result = cbfifo_dequeue(&cb, &dq_data[1], BUFFER_SIZE);
    if (result != (BUFFER_SIZE - 1) ||
        cbfifo_length(&cb) != 0 ||
        !array_equal(dq_data, data, sizeof(data))) {
        printf("Test point 3 failure. Running fifo state/data dump:\n\r");
        cbfifo_state_dump(&cb);
        cbfifo_data_dump(&cb);
        printf("Dequeued Data: ");
        for (int i = 0; i < sizeof(dq_data); i++) {
            printf("0x%02X, ", dq_data[i]);
        }
        printf("\n\r");
        printf("Expected Data: ");
        for (int i = 0; i < sizeof(data); i++) {
            printf("0x%02X, ", data[i]);
        }
        printf("\n\r");
        printf("Result: %d\n\r", (int)result);
        failure++;
    }

    // Test point 4: Dequeue with an empty FIFO
    result = cbfifo_dequeue(&cb, &dq_data[0], 4);
    if (result != 0) {
        printf("Test point 4 failure. Running fifo state dump:\n\r");
        cbfifo_state_dump(&cb);
        failure++;
    }

    // Test point 5: Full loop enqueue + dequeue (test a wraparound of the read/write pointer)
    result = cbfifo_enqueue(&cb, &zero[0], 8); // Give some data that's going to be dequeued then overwritten
    result = cbfifo_enqueue(&cb, &data[0], 8);
    if (result != 8 ||
        cbfifo_length(&cb) != 16) {
        printf("Test point 5.1 failure. Running fifo state dump:\n\r");
        cbfifo_state_dump(&cb);
        printf("Result: %d\n\r", (int)result);
        failure++;
    }
    result = cbfifo_dequeue(&cb, &dq_data[0], 8);
    if (result != 8 ||
        cbfifo_length(&cb) != 8) {
        printf("Test point 5.2 failure. Running fifo state dump:\n\r");
        cbfifo_state_dump(&cb);
        printf("Result: %d\n\r", (int)result);
        failure++;
    }
    // Wrap around and fill
    result = cbfifo_enqueue(&cb, &data[8], (BUFFER_SIZE));
    if (result != (BUFFER_SIZE-8) ||
        cbfifo_length(&cb) != BUFFER_SIZE) {
        printf("Test point 5.3 failure. Running fifo state dump:\n\r");
        cbfifo_state_dump(&cb);
        printf("Result: %d\n\r", (int)result);
        failure++;
    }
    result = cbfifo_dequeue(&cb, &dq_data[0], BUFFER_SIZE);
    if (result != BUFFER_SIZE ||
        cbfifo_length(&cb) != 0 ||
        !array_equal(dq_data, data, sizeof(data))) {
        printf("Test point 5.4 failure. Running fifo state/data dump:\n\r");
        cbfifo_state_dump(&cb);
        cbfifo_data_dump(&cb);
        printf("Dequeued Data: ");
        for (int i = 0; i < sizeof(dq_data); i++) {
            printf("0x%02X, ", dq_data[i]);
        }
        printf("\n\r");
        printf("Expected Data: ");
        for (int i = 0; i < sizeof(data); i++) {
            printf("0x%02X, ", data[i]);
        }
        printf("\n\r");
        printf("Result: %d\n\r", (int)result);
        failure++;
    }

    if (failure) {
        printf("Total cbfifo_dequeue failures: %d\n\r", failure);
        return 0;
    }
    return 1;
}


uint8_t test_cbfifo() {
    int result;
    int failure = 0;

    printf("Testing cbfifo_enqueue: ");
    result = test_cbfifo_enqueue();
    if (!result) {
        failure++;
    } else {
        printf("passed \n\r");
    }

    printf("Testing cbfifo_dequeue: ");
    result = test_cbfifo_dequeue();
    if (!result) {
        failure++;
    } else {
        printf("passed \n\r");
    }

    if (failure) {
        printf("Total cbfifo failures: %d\n\r", failure);
        return 0;
    }
    return 1;
}
