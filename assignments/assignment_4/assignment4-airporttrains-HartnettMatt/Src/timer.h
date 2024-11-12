/*
 * timer.h
 *
 *  Created on: Oct 14, 2024
 *      Author: Matt Hartnett
 */

#ifndef TIMER_H_
#define TIMER_H_

#define F_SYS_CLK (48000000L)

/*
 * Initalize the systick to generate interrupts at 100ms
 *
 * Parameters: none
 *
 * Returns: none
 */
void Init_SysTick(void);

/*
 * Initalize the systick to generate interrupts at 100ms
 *
 * Parameters: none
 *
 * Returns: none
 */
void SysTick_Handler(void);

/*
 * Configure the clock to run at 48 MHz
 *
 * Parameters: none
 *
 * Returns: none
 */

#endif /* TIMER_H_ */
