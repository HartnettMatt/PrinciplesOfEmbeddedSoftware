/*
 * timer.c
 *
 *  Created on: Sep 29, 2024
 *      Author: Matt Hartnett
 */

#include "log.h"
#include "timer.h"
#include "button.h"

int delay(int delay_msec, int button_interrupt_en) {
	LOG("START TIMER %d\r\n", delay_msec);
	if (button_interrupt_en) {
		while (delay_msec > 0) {
			int ctr = BUTTON_POLL_RATE * MSEC_TO_CTR;
			while (ctr-- != 0) {
				__asm volatile("NOP");
			}
			if(read_button()){
				// If the button has been pressed, then cancel the timer
				return 1;
			}
			delay_msec = delay_msec - BUTTON_POLL_RATE;
		}
		return 0;
	} else {
		int ctr = delay_msec * MSEC_TO_CTR;
		while (ctr-- != 0) {
			__asm volatile("NOP");
		}
		return 0;
	}

}

