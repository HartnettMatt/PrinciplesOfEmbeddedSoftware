/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Matt Hartnett
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdio.h>
#include "fsm.h"
#include "button.h"
#include "eled.h"
#include "timer.h"
#include "uled.h"

int main(void)
{
	/* You can use the Command Shell Console in STM32CubeIDE or an
	 * external program such as Putty or TeraTerm to view printf output.
	 * The settings are: Baud: 9600, Parity: None, Stop Bits: 1 with
	 * the ST-Link COM port setting.
	 * Clocks: Processor = 48 Mhz. AHB = 48 MHz. APB = 24 MHz.
	 */

    // Init various devices:
    uled_init();
    eled_init();
    button_init();
    Init_SysTick(); // This one starts the 100ms interrupts

    // Infinte loop is contained in the fsm_run routine
    fsm_run();
}
