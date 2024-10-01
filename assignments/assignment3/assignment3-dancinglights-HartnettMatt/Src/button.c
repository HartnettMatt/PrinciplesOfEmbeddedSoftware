/*
 * button.c
 *
 *  Created on: Sep 29, 2024
 *      Author: Matt Hartnett and Alex Dean
 */

#include <stdio.h>
#include <stm32f091xc.h>
#include "button.h"
#include "utilities.h"

int button_init(void) {

	// This section of code was taken from Alex Dean's Github
	// https://github.com/alexander-g-dean/ESF/blob/master/ST/Code/ch2/Basic-LED/main.c
	// Enable peripheral clock of GPIOC (for Switch B1)
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	// Configure PC13 in input mode (00=0)
	MODIFY_FIELD(GPIOC->MODER, GPIO_MODER_MODER13, ESF_GPIO_MODER_INPUT);
	// End of Alex Dean Code

	return 0;

}

int read_button(void){
	return !(GPIOC->IDR & B1_IN_MSK);
}
