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


size_t cbfifo_enqueue(cbfifo *cb, void *buf, size_t nbyte) {
    // Error conditions
    if (buf == NULL || cb->is_full) {
        return (size_t)-1;
    }

    size_t available_space = BUFFER_SIZE - cbfifo_length(cb);
    size_t to_enqueue = nbyte;

    // Cap the enqueue length to the max available space
    if (to_enqueue > available_space) {
        to_enqueue = available_space;
    }

    // Loop through the buffer and add to FIFO at the head ptr
    for (size_t i = 0; i < to_enqueue; i++) {
        cb->fifo[cb->write] = ((uint8_t *)buf)[i];
        cb->write = (cb->write + 1) & (BUFFER_SIZE - 1); // Wrap around using bitwise AND
    }

    // If we had to cap, then the FIFO is full at this point
    if (to_enqueue > 0 && cb->read == cb->write) {
        cb->is_full = 1;
    }
    return to_enqueue;
}

size_t cbfifo_dequeue(cbfifo *cb, void *buf, size_t nbyte) {
    if (buf == NULL || nbyte == 0) {
        return (size_t)-1; // Return -1 if buffer is NULL
    }
    size_t available_data = cbfifo_length(cb);
    size_t to_dequeue = nbyte;

    if (to_dequeue > available_data) {
        to_dequeue = available_data;
    }

    for (size_t i = 0; i < to_dequeue; i++) {
        ((uint8_t *)buf)[i] = cb->fifo[cb->read];
        cb->read = (cb->read + 1) & (BUFFER_SIZE - 1); // Wrap around using bitwise AND
    }

    // If any bytes were removed, then the FIFO isn't full anymore
    if (to_dequeue > 0) {
        cb->is_full = 0;
    }

    return to_dequeue;
}

size_t cbfifo_peek(cbfifo *cb, uint8_t *byte) {
    if (cbfifo_length(cb) == 0) {
        // Buffer is empty
        return 0;
    }

    *byte = cb->fifo[cb->read];
    return cbfifo_length(cb);
}

size_t cbfifo_length(cbfifo *cb) {
    // More reliable for changes in BUFFER_SIZE
    if (cb->is_full) {
        // If the buffer is marked as full, return the maximum capacity
        return BUFFER_SIZE;
    } else if (cb->write >= cb->read) {
        // If write pointer is ahead or equal to read pointer, calculate length normally
        return cb->write - cb->read;
    } else {
        // If write pointer has wrapped around, account for the circular structure
        return BUFFER_SIZE - (cb->read - cb->write);
    }
}

size_t cbfifo_capacity(cbfifo *cb) {
    // Capacity is statically set to BUFFER_SIZE
    return BUFFER_SIZE;
}

void cbfifo_state_dump(cbfifo *cb) {
    printf("Write pointer: %d \n", cb->write);
    printf("Read pointer: %d \n", cb->read);
    printf("Length: %d \n", (int)cbfifo_length(cb));
    printf("Is Full: %d \n", cb->is_full);
}

void cbfifo_data_dump(cbfifo *cb) {
    if ((int)cbfifo_length(cb) == 0) {
        printf("FIFO is empty.");
    }
    for (int i = 0; i < (int)cbfifo_length(cb); i++) {
        printf("0x%02X, ", (uint8_t)cb->fifo[i]);
    }
    printf("\n");
}


void cbfifo_reset(cbfifo *cb) {
    cb->write = 0;
    cb->read = 0;
    cb->is_full = 0;
}