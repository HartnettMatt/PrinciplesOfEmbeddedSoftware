/*
 * led.h
 *
 *  Created on: Sep 29, 2024
 *      Author: Matt Hartnett and Alex Dean
 */

#ifndef ULED_H_
#define ULED_H_

// These defines were written by Alex Dean:
// https://github.com/alexander-g-dean/ESF/blob/master/ST/Code/ch2/Basic-LED/main.c
#define LD2_OFF_MSK	(GPIO_BSRR_BR_5)
#define LD2_ON_MSK	(GPIO_BSRR_BS_5)
// End of Alex Dean

/*
 * Initializes the user LED (LD2)
 *
 * Parameters:
 *   none
 *
 * Returns: 0 if the LED is initialized correctly, 1 if not
 */
int uled_init(void);

/*
 * Turns the user LED on
 *
 * Parameters:
 *   none
 *
 * Returns: 0 if the LED is turned on correctly, 1 if not
 */
int uled_on(void);

/*
 * Turns the user LED off
 *
 * Parameters:
 *   none
 *
 * Returns: 0 if the LED is turned off correctly, 1 if not
 */
int uled_off(void);

#endif /* ULED_H_ */
