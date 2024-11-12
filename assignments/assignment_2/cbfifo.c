/*******************************************************************************
 * Copyright (C) 2023 by Matt Hartnett
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. Matt Hartnett and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/

/**
 * @file    cbfifo.c
 * @brief The implmentation file for the circular
 *  buffer FIFO.
 *
 * @author  Matt Hartnett
 * @date    09/11/2024
 *
 */
#include "cbfifo.h"
#include <stddef.h>
#include <stdio.h>
#define BUFFER_SIZE 256


static uint8_t fifo[BUFFER_SIZE]; // The circular buffer array
static uint8_t write = 0;         // Head index
static uint8_t read = 0;          // Tail index
static int is_full = 0;


size_t cbfifo_enqueue(void *buf, size_t nbyte) {
    // Error conditions
    if (buf == NULL || is_full) {
        return (size_t)-1;
    }

    size_t available_space = BUFFER_SIZE - cbfifo_length();
    size_t to_enqueue = nbyte;

    // Cap the enqueue length to the max available space
    if (to_enqueue > available_space) {
        to_enqueue = available_space;
    }

    // Loop through the buffer and add to FIFO at the head ptr
    for (size_t i = 0; i < to_enqueue; i++) {
        fifo[write] = ((uint8_t *)buf)[i];
        write = (write + 1) & (BUFFER_SIZE - 1); // Wrap around using bitwise AND
    }

    // If we had to cap, then the FIFO is full at this point
    if (to_enqueue > 0 && read == write) {
        is_full = 1;
    }
    return to_enqueue;
}

size_t cbfifo_dequeue(void *buf, size_t nbyte) {
    if (buf == NULL || nbyte == 0) {
        return (size_t)-1; // Return -1 if buffer is NULL
    }
    size_t available_data = cbfifo_length();
    size_t to_dequeue = nbyte;

    if (to_dequeue > available_data) {
        to_dequeue = available_data;
    }

    for (size_t i = 0; i < to_dequeue; i++) {
        ((uint8_t *)buf)[i] = fifo[read];
        read = (read + 1) & (BUFFER_SIZE - 1); // Wrap around using bitwise AND
    }

    // If any bytes were removed, then the FIFO isn't full anymore
    if (to_dequeue > 0) {
        is_full = 0;
    }

    return to_dequeue;
}

size_t cbfifo_peek(uint8_t *byte) {
    if (cbfifo_length() == 0) {
        // Buffer is empty
        return 0;
    }

    *byte = fifo[read];
    return cbfifo_length();
}

size_t cbfifo_length() {
    // If the FIFO is full, the return should be 256, but if not the formula will calculate it
    return is_full ? 256 : (size_t)((uint8_t)(write - read));
}

size_t cbfifo_capacity() {
    // Capacity is statically set to BUFFER_SIZE
    return BUFFER_SIZE;
}

void cbfifo_state_dump() {
    printf("Write pointer: %d \n", write);
    printf("Read pointer: %d \n", read);
    printf("Length: %d \n", (int)cbfifo_length());
    printf("Is Full: %d \n", is_full);
}

void cbfifo_data_dump() {
    if((int)cbfifo_length() == 0){
        printf("FIFO is empty.");
    }
    for (int i = 0; i < (int)cbfifo_length(); i++) {
        printf("0x%02X, ", (uint8_t)fifo[i]);
    }
    printf("\n");
}