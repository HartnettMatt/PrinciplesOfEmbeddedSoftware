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
 * @file    test_llfifo.c
 * @brief Matt Hartnett's work for ECEN5813 HW2.
 * This is the test file for the linked list based FIFO.
 *
 * @author  Matt Hartnett
 * @date    9/16/2024
 *
 */

#include "test_llfifo.h"
#include "llfifo.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>


/**
 * @brief Int array compare function
 *
 * @return 1 if arrays are equal, 0 if not equal
 */
int arrays_equal(int *arr1, int *arr2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (arr1[i] != arr2[i]) {
            return 0; // Arrays are not equal
        }
    }
    return 1; // Arrays are equal
}


/**
 * @brief Tests the llfifo_create function
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_llfifo_create() {
    llfifo_t *result;
    int failure = 0;

    // Test point 1: Max capacity of 0 (should return null)
    result = llfifo_create(0, 0);
    if (result != NULL) {
        printf("Test point 1 failure. Running fifo state dump:\n");
        llfifo_state_dump(result);
        failure++;
    }
    llfifo_destroy(result);

    // Test point 2: Negative capacity (should return null)
    result = llfifo_create(-1, 0);
    if (result != NULL) {
        printf("Test point 2 failure. Running fifo state dump:\n");
        llfifo_state_dump(result);
        failure++;
    }
    llfifo_destroy(result);

    // Test point 3: Capacity greater than max capacity
    result = llfifo_create(2, 1);
    if (result != NULL) {
        printf("Test point 3 failure. Running fifo state dump:\n");
        llfifo_state_dump(result);
        failure++;
    }
    llfifo_destroy(result);

    // Test point 4: Capcity of 0, max capacity of 10
    result = llfifo_create(0, 10);
    // Failure conditions:
    if (result == NULL ||
        llfifo_length(result) != 0 ||        // Checks that the length is 0
        llfifo_capacity(result) != 0 ||      // Checks capacity
        llfifo_max_capacity(result) != 10 || // Checks max capacity
        llfifo_peek(result) != NULL          // This checks to make sure there isn't any data
        ) {

        printf("Test point 4 failure. Running fifo state dump:\n");
        llfifo_state_dump(result);
        failure++;
    }
    llfifo_destroy(result);

    // Test point 5: Capcity of 5, max capacity of 10
    result = llfifo_create(5, 10);
    // Failure conditions:
    if (result == NULL ||
        llfifo_length(result) != 0 ||        // Checks that the length is 0
        llfifo_capacity(result) != 5 ||      // Checks capacity
        llfifo_max_capacity(result) != 10 || // Checks max capacity
        llfifo_peek(result) != NULL          // This checks to make sure there isn't any data
        ) {

        printf("Test point 5 failure. Running fifo state dump:\n");
        llfifo_state_dump(result);
        failure++;
    }
    llfifo_destroy(result);


    if (failure) {
        printf("Total llfifo_create failures: %d\n", failure);
        return 0;
    }
    return 1;
}

/**
 * @brief Tests the llfifo_enqueue function
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_llfifo_enqueue() {
    int failure = 0;
    int result;
    int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    llfifo_t *fifo;


    // Test point 1: Max capacity of 0, enqueue data (functionally a NULL pointer test)
    fifo = llfifo_create(0, 0);
    result = llfifo_enqueue(fifo, &(data[0]));
    if (result != -1) {
        printf("Test point 1 failure. Running fifo state dump:\n");
        llfifo_state_dump(fifo);
        failure++;
    }
    llfifo_destroy(fifo);

    // Test point 2: Max capacity of 10, enqueue NULL
    fifo = llfifo_create(10, 0);
    result = llfifo_enqueue(fifo, NULL);
    if (result != -1) {
        printf("Test point 2 failure. Running fifo state dump:\n");
        llfifo_state_dump(fifo);
        failure++;
    }
    llfifo_destroy(fifo);

    // Test point 3: Max capacity of 9, enqueue data and peek
    fifo = llfifo_create(0, 9);
    result = llfifo_enqueue(fifo, &(data[0]));
    // Failure conditions:
    if (result != 1 ||
        *(int *)llfifo_peek(fifo) != data[0] ||
        llfifo_capacity(fifo) != 1 ||
        llfifo_length(fifo) != 1) {
        printf("Test point 3 failure. Running fifo state dump:\n");
        llfifo_state_dump(fifo);
        failure++;
    }

    // Test point 4: Enqueue a new data value and peek (should be old value)
    result = llfifo_enqueue(fifo, &(data[1]));
    // Failure conditions:
    if (result != 2 ||
        *(int *)llfifo_peek(fifo) != data[0] ||
        llfifo_capacity(fifo) != 2 ||
        llfifo_length(fifo) != 2) {
        printf("Test point 4 failure. Running fifo state/data dump:\n");
        llfifo_state_dump(fifo);
        llfifo_data_dump(fifo);
        failure++;
    }

    // Test point 5: Enqueue 8 new data values and peek (should be value of 1 from overwrite of 0)
    for (int i = 2; i < 10; i++) {
        result = llfifo_enqueue(fifo, &(data[i]));
    }
    // Failure conditions:
    if (result != 9 ||
        *(int *)llfifo_peek(fifo) != data[1] ||
        llfifo_capacity(fifo) != 9 ||
        llfifo_length(fifo) != 9) {
        printf("Test point 5 failure. Running fifo state/data dump:\n");
        llfifo_state_dump(fifo);
        llfifo_data_dump(fifo);
        printf("Peek: %d\n", *(int *)llfifo_peek(fifo));
        printf("Result: %d\n", result);
        failure++;
    }
    llfifo_destroy(fifo);

    // Test point 6: enqueue a bunch of different data types
    fifo = llfifo_create(0, 10);
    int data0 = 0;
    uint8_t data1 = 1;
    char data2 = 'c';
    char *data3 = malloc(sizeof(char));
    *data3 = 'd';
    long data4 = 4;
    long *data5 = malloc(sizeof(long));
    ;
    *data5 = 5;
    result = llfifo_enqueue(fifo, &(data0));
    result = llfifo_enqueue(fifo, &(data1));
    result = llfifo_enqueue(fifo, &(data2));
    result = llfifo_enqueue(fifo, &(data3));
    result = llfifo_enqueue(fifo, &(data4));
    result = llfifo_enqueue(fifo, &(data5));
    // Failure conditions:
    if (result != 6 ||
        *(int *)llfifo_peek(fifo) != data0 ||
        llfifo_capacity(fifo) != 6 ||
        llfifo_length(fifo) != 6) {
        printf("Test point 6 failure. Running fifo state/data dump:\n");
        llfifo_state_dump(fifo);
        llfifo_data_dump(fifo);
        printf("Peek: %d\n", *(int *)llfifo_peek(fifo));
        printf("Result: %d\n", result);
        failure++;
    }
    llfifo_destroy(fifo);
    free(data3);
    free(data5);


    if (failure) {
        printf("Total llfifo_enqueue failures: %d\n", failure);
        return 0;
    }
    return 1;
}

/**
 * @brief Tests the llfifo_dequeue function
 *
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_llfifo_dequeue() {
    int failure = 0;
    void *result;
    int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    llfifo_t *fifo;

    // Test point 1: Null pointer dequeue
    result = llfifo_dequeue(NULL);
    if (result != NULL) {
        printf("Test point 1 failure. Result %p:\n", result);
        failure++;
    }

    // Test point 2: Dequeue empty fifo
    fifo = llfifo_create(10, 0);
    result = llfifo_dequeue(fifo);
    if (result != NULL) {
        printf("Test point 2 failure. Result %p:\n", result);
        failure++;
    }
    llfifo_destroy(fifo);

    // Test point 3: Max capacity of 1, enqueue data and dequeue
    fifo = llfifo_create(0, 1);
    llfifo_enqueue(fifo, &(data[0]));
    result = llfifo_dequeue(fifo);
    // Failure conditions:
    if (result == NULL ||
        *(int *)result != data[0] ||
        llfifo_capacity(fifo) != 1 ||
        llfifo_length(fifo) != 0) {
        printf("Test point 3 failure. Running fifo state dump:\n");
        llfifo_state_dump(fifo);
        failure++;
    }
    llfifo_destroy(fifo);


    // Test point 4: Fill empty fifo with full data set, dequeue full data and compare
    fifo = llfifo_create(0, 10);
    int dq_data[10];
    for (int i = 0; i < 10; i++) {
        llfifo_enqueue(fifo, &(data[i]));
    }

    for (int i = 0; i < 10; i++) {
        dq_data[i] = *(int *)llfifo_dequeue(fifo);
    }
    // Failure conditions:
    if (llfifo_capacity(fifo) != 10 ||
        llfifo_length(fifo) != 0 ||
        !arrays_equal(data, dq_data, 10)) {
        printf("Test point 4 failure. Running fifo state/data dump:\n");
        llfifo_state_dump(fifo);
        llfifo_data_dump(fifo);
        failure++;
    }
    llfifo_destroy(fifo);

    // Test point 5: Fill empty fifo with full data set, dequeue until emtpy
    fifo = llfifo_create(0, 10);
    for (int i = 0; i < 10; i++) {
        llfifo_enqueue(fifo, &(data[i]));
    }
    int i = 0;
    while (llfifo_length(fifo) != 0) {
        dq_data[i] = *(int *)llfifo_dequeue(fifo);
        i++;
    }
    // Failure conditions:
    if (llfifo_capacity(fifo) != 10 ||
        llfifo_length(fifo) != 0 ||
        !arrays_equal(data, dq_data, 10)) {
        printf("Test point 5 failure. Running fifo state/data dump:\n");
        llfifo_state_dump(fifo);
        llfifo_data_dump(fifo);
        failure++;
    }
    llfifo_destroy(fifo);

    // Test point 6: Fill empty fifo with data larger than max capacity, dequeue until empty
    fifo = llfifo_create(0, 5);
    for (int i = 0; i < 10; i++) {
        llfifo_enqueue(fifo, &(data[i]));
    }

    for (int i = 0; i < 5; i++) {
        dq_data[i] = *(int *)llfifo_dequeue(fifo);
    }
    int expected_result[5] = {5, 6, 7, 8, 9};
    // Failure conditions:
    if (llfifo_capacity(fifo) != 5 ||
        llfifo_length(fifo) != 0 ||
        !arrays_equal(expected_result, dq_data, 5)) {
        printf("Test point 6 failure. Running fifo state/data dump:\n");
        llfifo_state_dump(fifo);
        llfifo_data_dump(fifo);
        printf("Returned data: \n");
        for (int i = 0; i < 5; i++) {
            printf("%d, ", dq_data[i]);
        }
        printf("\n");
        failure++;
    }
    llfifo_destroy(fifo);


    // Test point 7: enqueue a bunch of different data types and dequeue
    fifo = llfifo_create(0, 10);
    int data0 = 0;
    uint8_t data1 = 1;
    char data2 = 'c';
    char *data3 = malloc(sizeof(char));
    *data3 = 'd';
    long data4 = 4;
    long *data5 = malloc(sizeof(long));

    *data5 = 5;
    llfifo_enqueue(fifo, &(data0));
    llfifo_enqueue(fifo, &(data1));
    llfifo_enqueue(fifo, &(data2));
    llfifo_enqueue(fifo, (data3));
    llfifo_enqueue(fifo, &(data4));
    llfifo_enqueue(fifo, (data5));
    int dq_data0 = *(int *)llfifo_dequeue(fifo);
    uint8_t dq_data1 = *(uint8_t *)llfifo_dequeue(fifo);
    char dq_data2 = *(char *)llfifo_dequeue(fifo);
    char *dq_data3 = (char *)llfifo_dequeue(fifo);
    long dq_data4 = *(long *)llfifo_dequeue(fifo);
    long *dq_data5 = (long *)llfifo_dequeue(fifo);
    // Failure conditions:
    if (dq_data0 != data0 ||
        dq_data1 != data1 ||
        dq_data2 != data2 ||
        dq_data3 != data3 ||
        dq_data4 != data4 ||
        dq_data5 != data5 ||
        llfifo_capacity(fifo) != 6 ||
        llfifo_length(fifo) != 0) {
        printf("Test point 7 failure. Running fifo state/data dump:\n");
        llfifo_state_dump(fifo);
        llfifo_data_dump(fifo);
        printf("Data 0: %d\n", data0);
        printf("Data 1: %d\n", data1);
        printf("Data 2: %d\n", data2);
        printf("Data 3: %p\n", data3);
        printf("Data 4: %lu\n", data4);
        printf("Data 5: %p\n", data5);
        printf("DQ Data 0: %d\n", dq_data0);
        printf("DQ Data 1: %d\n", dq_data1);
        printf("DQ Data 2: %d\n", dq_data2);
        printf("DQ Data 3: %p\n", dq_data3);
        printf("DQ Data 4: %lu\n", dq_data4);
        printf("DQ Data 5: %p\n", dq_data5);
        failure++;
    }
    llfifo_destroy(fifo);
    free(data3);
    free(data5);


    if (failure) {
        printf("Total llfifo_enqueue failures: %d\n", failure);
        return 0;
    }
    return 1;
}

uint8_t test_llfifo() {
    int result;
    int failure = 0;

    printf("Testing llfifo_create: ");
    result = test_llfifo_create();
    if (!result) {
        failure++;
    } else {
        printf("passed \n");
    }

    printf("Testing llfifo_enqueue: ");
    result = test_llfifo_enqueue();
    if (!result) {
        failure++;
    } else {
        printf("passed \n");
    }

    printf("Testing llfifo_dequeue: ");
    result = test_llfifo_dequeue();
    if (!result) {
        failure++;
    } else {
        printf("passed \n");
    }

    if (failure) {
        printf("Total llfifo failures: %d\n", failure);
        return 0;
    }
    return 1;
}
