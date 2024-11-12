/*
 * eled.h
 *
 *  Created on: Sep 29, 2024
 *      Author: Matt Hartnett
 */

#ifndef ELED_H_
#define ELED_H_

#define PB3_OFF_MSK	(GPIO_BSRR_BS_3)
#define PB3_ON_MSK	(GPIO_BSRR_BR_3)


/*
 * Initializes the external LED (EB3)
 *
 * Parameters:
 *   none
 *
 * Returns: 0 if the LED is initialized correctly, 1 if not
 */
int eled_init(void);

/*
 * Turns the external LED on
 *
 * Parameters:
 *   none
 *
 * Returns: 0 if the LED is turned on correctly, 1 if not
 */
int eled_on(void);

/*
 * Turns the external LED off
 *
 * Parameters:
 *   none
 *
 * Returns: 0 if the LED is turned off correctly, 1 if not
 */
int eled_off(void);

#endif /* ELED_H_ */
