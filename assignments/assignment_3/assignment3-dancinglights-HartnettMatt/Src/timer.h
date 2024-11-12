/*
 * timer.h
 *
 *  Created on: Sep 29, 2024
 *      Author: Matt Hartnett
 */

#ifndef TIMER_H_
#define TIMER_H_

#define MSEC_TO_CTR 3200 // Conversion between nops and milliseconds
#define BUTTON_POLL_RATE 100 // Button poll rate in milliseconds

/*
 * Delay using a processor cycle loop
 *
 * Parameters:
 *   delay_msec = Number of milliseconds to delay by
 *   button_interrupt_en = 1 if the delay can be interrupted, 0 if not
 *
 * Returns: 0 if the timer finishes, 1 if the button interrupted the delay
 */
int delay(int delay_msec, int button_interrupt_en);

#endif /* TIMER_H_ */
