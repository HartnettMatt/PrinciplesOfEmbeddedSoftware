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
 * @file    test_cbfifo.h
 * @brief Matt Hartnett's work for ECEN5813 HW2.
 * This is the test header file for the circular buffer based FIFO.
 *
 * @author  Matt Hartnett
 * @date    9/16/2024
 *
 */

#ifndef TEST_CBFIFO_H
#define TEST_CBFIFO_H

#include <stdint.h>
#include <stddef.h>

uint8_t test_cbfifo(void);
uint8_t test_cbfifo_enqueue(void);
uint8_t test_cbfifo_dequeue(void);

#endif // TEST_CBFIFO_H

