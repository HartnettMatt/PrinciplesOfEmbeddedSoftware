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
 * @brief Matt Hartnett's USART implemententation.
 *
 * @author  Matt Hartnett
 * @date    11/10/2024
 *
 */

#include "usart.h"
#include "cbfifo.h"
#include "field_access.h"
#include "stm32f091xc.h"
#include <stdint.h>
#include <stdio.h>

cbfifo rx_cb = {
    .write = 0,
    .read = 0,
    .is_full = 0};
cbfifo tx_cb = {
    .write = 0,
    .read = 0,
    .is_full = 0};

int usart_init() {

    // This code was taken from Alex Dean's Github: https://github.com/alexander-g-dean/ESF.git
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // GPIO A pin 2 and 3 in alternate function 1 (USART2)
    // Set mode field to 2 for alternate function
    MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER2, ESF_GPIO_MODER_ALT_FUNC);
    MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER3, ESF_GPIO_MODER_ALT_FUNC);
    // Select USART2 (AF = 1) as alternate function
    MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL2, 1);
    MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL3, 1);
    // End of Alex Dean Code


    // Set baud rate
    USART2->BRR = USART_BRR;
    // set M0 and M1 fields based on data size
    MODIFY_FIELD(USART2->CR1, USART_CR1_M, USART_M_VAL);
    // Set parity generation and checking
    MODIFY_FIELD(USART2->CR1, USART_CR1_PCE, USART_PCE_VAL);
    MODIFY_FIELD(USART2->CR1, USART_CR1_PS, USART_PS_VAL);
    // Number of stop bits
    MODIFY_FIELD(USART2->CR2, USART_CR2_STOP, STOP_BITS);

    // Enable transmitter and receiver and USART
    MODIFY_FIELD(USART2->CR1, USART_CR1_TE, 1);
    MODIFY_FIELD(USART2->CR1, USART_CR1_RE, 1);
    MODIFY_FIELD(USART2->CR1, USART_CR1_UE, 1);

    // Enable RX interrupts
    MODIFY_FIELD(USART2->CR1, USART_CR1_RXNEIE, 1);

    // Enable USART2 interrupts in NVIC
    NVIC_SetPriority(USART2_IRQn, 2);
    NVIC_ClearPendingIRQ(USART2_IRQn);
    NVIC_EnableIRQ(USART2_IRQn);

    setbuf(stdout, NULL); // Necessary to remove printf buffering that causes issues

    return 0;
}

// This code was copied from Alex Dean and modified by Matt Hartnett
void USART2_IRQHandler(void) {
    uint8_t ch;
    if (USART2->ISR & USART_ISR_RXNE) { // Receive buffer not empty
        // Reading from RDR will clear RXNE
        ch = USART2->RDR;
        if (cbfifo_length(&rx_cb) != cbfifo_capacity(&rx_cb)) {
            cbfifo_enqueue(&rx_cb, &ch, sizeof(ch));
        }
    }
    if (USART2->ISR & USART_ISR_TXE) { // Transmit buffer empty
        // Writing to TDR will clear TXE
        if (cbfifo_length(&tx_cb) != 0) { // More data to send
            cbfifo_dequeue(&tx_cb, &ch, sizeof(ch));
            USART2->TDR = ch;
        } else { // Disable transmitter interrupt
            MODIFY_FIELD(USART2->CR1, USART_CR1_TXEIE, 0);
        }
    }
}


void usart_transmit(char *buf, int nbyte) {
    while (nbyte > 0) {
        while (cbfifo_length(&tx_cb) == cbfifo_capacity(&tx_cb))
            ; // Wait until TX CB is not full
        cbfifo_enqueue(&tx_cb, buf, 1);
        buf++;
        nbyte--;
    }
    MODIFY_FIELD(USART2->CR1, USART_CR1_TXEIE, 1); // Enable TXE interrupt
}

void usart_receive_blocking(char *buf, int num_chars) {
    // This function gets num_chars, blocking if they aren't all ready yet
    while (num_chars > 0) {
        while (cbfifo_length(&rx_cb) == 0) // Block until some data is received
            ;
        cbfifo_dequeue(&rx_cb, buf, 1);
        num_chars--; // Got another character
        buf++;       // Advance buffer pointer to space for next character
    }
}

uint8_t usart_receive(void) {
    while ((USART2->ISR & USART_ISR_RXNE) == 0)
        ;
    return USART2->RDR;
}

int __io_putchar(int ch) {
    char buf = (char)ch;  // Convert int to char for transmission
    usart_transmit(&buf, 1);  // Send one character
    return 1;  // Indicate success
}

int __io_getchar(void) {
    char buf;
    usart_receive_blocking(&buf, 1);  // Receive one character in blocking mode
    return (int)buf;  // Return the received character
}