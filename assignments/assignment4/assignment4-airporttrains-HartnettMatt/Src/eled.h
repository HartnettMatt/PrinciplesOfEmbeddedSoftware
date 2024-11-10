/*
 * led.h
 *
 *  Created on: Sep 29, 2024
 *      Author: Matt Hartnett and Alex Dean
 */

#include "stdint.h"

#ifndef ELED_H_
#define ELED_H_

// These defines were written by Alex Dean:
// https://github.com/alexander-g-dean/ESF/blob/master/ST/Code/ch2/Basic-LED/main.c
#define PWM_PRESCALER (2)
#define PWM_FREQUENCY (500)
#define PWM_MAX_DUTY_VALUE ( (F_TIM_CLOCK / (PWM_FREQUENCY * PWM_PRESCALER)) - 1)
#define F_TIM_CLOCK (48UL*1000UL*1000UL)	// 48 MHz
#define DIM_DELAY (100)
// End of Alex Dean

/*
 * Initializes the user LED (LD2)
 *
 * Parameters:
 *   none
 *
 * Returns: 0 if the LED is initialized correctly, 1 if not
 */
int eled_init(void);

/*
 * Sets the user LED to a brightness
 *
 * Parameters:
 *   brightness - 8 bit value representing the LED brightness
 *
 * Returns: none
 */
void eled_set_brightness(uint8_t brightness);


#endif /* ELED_H_ */
