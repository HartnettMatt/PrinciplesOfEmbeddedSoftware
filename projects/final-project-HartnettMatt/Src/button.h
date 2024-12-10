/*
 * button.h
 *
 *  Created on: Sep 29, 2024
 *      Author: Matt Hartnett and Alex Dean
 */

#ifndef BUTTON_H_
#define BUTTON_H_


#define B1_IN_MSK		(GPIO_IDR_13)
#define B1_PULL_VALUE 1

/*
 * Initializes the user button (B1)
 *
 * Parameters:
 *   none
 *
 * Returns: 0 if the button is initialized correctly, 1 if not
 */
int button_init(void);

/*
 * Reads the state of the button
 *
 * Parameters:
 *   none
 *
 * Returns: 1 if the button is pressed, 0 if not
 */
int read_button(void);

#endif /* BUTTON_H_ */
