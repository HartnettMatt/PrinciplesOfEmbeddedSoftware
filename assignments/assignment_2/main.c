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
 * @file    main.c
 * @brief The main file for assignment 2 of ECEN5813.
 * This file calls the relevant tests for two FIFO implementations
 * that were completed as part of this proejct. The first FIFO is
 * a circular buffer, the second is a linked list. *
 *
 * @author  Matt Hartnett
 * @date    09/11/2024
 *
 */

#include "test_llfifo.h"
#include "llfifo.h"
#include "test_cbfifo.h"
#include "cbfifo.h"
#include <stdio.h>

int main(int argc, char **argv) {
 printf("Testing linked list FIFO...\n");
 int test = test_llfifo();
 if (test == 0) {
  printf("Linked list test(s) failed.\n");
 } else {
  printf("Linked list tests succeeded.\n");
 }
 printf("Testing circular buffer FIFO...\n");
 test = test_cbfifo();
 if (test == 0) {
  printf("Circular buffer test(s) failed.\n");
 } else {
  printf("Circular buffer tests succeeded.\n");
 }
 return 0;
}
