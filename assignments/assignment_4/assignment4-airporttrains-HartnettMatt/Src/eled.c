/*
 * led.c
 *
 *  Created on: Oct 14, 2024
 *      Author: Matt Hartnett and Alex Dean
 */

#include "log.h"
#include <stm32f091xc.h>
#include "eled.h"
#include "utilities.h"

int eled_init(void) {

    // This section of code was taken from Alex Dean's Github
    // https://github.com/alexander-g-dean/ESF/blob/master/ST/Code/ch7/PWM/main.c
    // Configure PA7 with alternate function 1: TIM3_CH2
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER7, ESF_GPIO_MODER_ALT_FUNC);
    MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFRL7, 1);

    // Configure TIM3 counter and prescaler
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = PWM_PRESCALER - 1;
    TIM3->ARR = PWM_MAX_DUTY_VALUE;
    TIM3->CR1 = 0; // count up

    // Configure TIM3 channel 2
    TIM3->CCR2 = 1;                               // Short on-time by default
    TIM3->CCER |= TIM_CCER_CC2P;                  // active low polarity
    MODIFY_FIELD(TIM3->CCMR1, TIM_CCMR1_OC2M, 6); // Select PWM mode
    TIM3->CCMR1 |= TIM_CCMR1_OC2PE;               // Enable preload register
    TIM3->EGR |= TIM_EGR_UG;                      // Generate update
    TIM3->CCER |= TIM_CCER_CC2E;                  // Enable channel output on OC2
    TIM3->BDTR |= TIM_BDTR_MOE;                   // Enable main output

    TIM3->CR1 |= TIM_CR1_CEN; // Enable timer
    // End of Alex Dean Code

    return 0;
}

void eled_set_brightness(uint8_t brightness) {
    uint32_t bright = brightness * PWM_MAX_DUTY_VALUE;
    bright = bright >> 8; // Roughly equivalent to dividing by 256 (max of bright ~~ PWM_MAX_DUTY_VALUE)
    TIM3->CCR2 = bright;
}

