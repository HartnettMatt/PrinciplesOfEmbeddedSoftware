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
 * @file    usart.h
 * @brief Matt Hartnett's USART header file.
 *
 * @author  Matt Hartnett
 * @date    11/10/2024
 *
 */

#ifndef _USART_H
#define _USART_H

#include <stdint.h>

// User UART settings
#define BAUD_RATE 19200
#define DATA_SIZE 9 // Include any parity bits, so 8 data bits + 1 parity bit = 9 data_size
#define PARTIY 2 // None = 0 Even = 1, Odd = 2
#define STOP_BITS 0 // 0 = 1 stop bit, 1 = 0.5 stop bits, 2 = 2 stop bits, 3 = 1.5 stop bits

// Other/derived settings
#define ESF_GPIO_MODER_ALT_FUNC (2) // Taken from Alex Dean's github
#if DATA_SIZE == 7
 #define USART_M_VAL 2
#elif DATA_SIZE == 8
 #define USART_M_VAL 0
#elif DATA_SIZE == 9
 #define USART_M_VAL 1
#endif
#if PARTIY == 0
 #define USART_PCE_VAL 0
 #define USART_PS_VAL 0
#elif PARTIY == 1
 #define USART_PCE_VAL 1
 #define USART_PS_VAL 0
#elif PARTIY == 2
 #define USART_PCE_VAL 1
 #define USART_PS_VAL 1
#endif
#define BUS_CLK 24000000
#define USART_BRR (BUS_CLK / BAUD_RATE) // Uses 16x oversampling

/*
 * Initilizes the USART1 peripherial based on the settings defined above
 *
 * Parameters: None
 *
 * Returns:
 *   0 for success
 *   1 for failure
 */
int usart_init();

/*
 * Interrupt handler for USART. Either load received character into rx_cb or transmit
 * character out of tx_cb based on interrupt value.
 *
 * Parameters: None
 *
 * Returns: None
 *
 */
void USART2_IRQHandler(void);

/*
 * Transmit a string over USART.
 *
 * Parameters:
 * buf: String to transmit
 * nbyte: Number of bytes (characters) to transmit
 *
 * Returns: None
 *
 */
void usart_transmit(char * buf, int nbyte);

/*
 * Blocking USART receive. Will not allow for continued
 * execution until a character is received. Useful for debugging.
 *
 * Parameters:
 * buf: Received string memory buffer
 * nbyte: Number of bytes (characters) to receive before unblocking
 *
 * Returns: None
 *
 */
void usart_receive_blocking(char * buf, int num_chars);

/*
 * Simple, single character USART receive. Useful for debugging.
 *
 * Parameters: None
 *
 * Returns: Received character as integer.
 *
 */
uint8_t usart_receive(void);

/*
 * Overridden putchar with Matt's USART transmit.
 *
 * Parameters:
 * ch: Character to transmit
 *
 * Returns:
 * 1 on success
 * 0 on failure
 *
 */
int __io_putchar(int ch);

/*
 * Overridden getchar with Matt's USART receive.
 *
 * Parameters: None
 *
 * Returns: Received character as integer.
 *
 */
int __io_getchar(void);

#endif // _USART_H