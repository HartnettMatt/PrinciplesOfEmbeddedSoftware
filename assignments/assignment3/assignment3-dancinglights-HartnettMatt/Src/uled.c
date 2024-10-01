/*
 * led.c
 *
 *  Created on: Sep 29, 2024
 *      Author: Matt Hartnett and Alex Dean
 */

#include <stdio.h>
#include <stm32f091xc.h>
#include "uled.h"
#include "utilities.h"

int uled_init(void) {

	// This section of code was taken from Alex Dean's Github
	// https://github.com/alexander-g-dean/ESF/blob/master/ST/Code/ch2/Basic-LED/main.c
	// Enable peripheral clock of GPIOA (for LD2)
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// Configure PA5 in output mode (01=1)
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER5, ESF_GPIO_MODER_OUTPUT);
	// Turn off LD2
	GPIOA->BSRR |= LD2_OFF_MSK;
	// End of Alex Dean Code

	return 0;

}

int uled_on(void){
#ifdef DEBUG
	printf("ULED ON\r\n");
#endif // DEBUG
	GPIOA->BSRR |= LD2_ON_MSK;
	return 0;
}

int uled_off(void){
#ifdef DEBUG
	printf("ULED OFF\r\n");
#endif // DEBUG
	GPIOA->BSRR |= LD2_OFF_MSK;
	return 0;
}
