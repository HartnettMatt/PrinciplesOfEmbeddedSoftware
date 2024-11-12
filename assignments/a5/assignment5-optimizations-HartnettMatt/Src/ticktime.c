/**
 * @file    ticktime.c
 * @brief   Code to map SysTick timer into msec counter
 * @author  Howdy Pierce, howdy.pierce@colorado.edu, Matt Hartnett
 */

#include "stm32f091xc.h"
#include "ticktime.h"
#include "pc_profiler.h"
#include <stdio.h>


/*
 * Stack pointer macro
 */
#define record_sp(x) asm("mov %0, sp" \
                         : "=r"(x))


static volatile ticktime_t g_now = 0;
static ticktime_t g_timer = 0;

void init_ticktime(void) {
  // set control & status register to use 48 MHz/8 = 6 MHz
  // clock. Then interrupt every .2 msec, e.g., 5000x per sec
  SysTick->LOAD = (48000000 / 8) / 5000 - 1;
  NVIC_SetPriority(SysTick_IRQn, 3);
  NVIC_ClearPendingIRQ(SysTick_IRQn);
  NVIC_EnableIRQ(SysTick_IRQn);
  SysTick->VAL = 0;
  SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk);
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

  g_now = 0;
  g_timer = 0;
}

void SysTick_Handler(void) {
  g_now++;
#ifdef DEBUG
  uint32_t stack_frame;
  uint32_t pc;
  if (pc_profiling_on) {
    record_sp(stack_frame);
    // Program Counter (PC) is at offset 14 in the stack frame
    pc = ((uint32_t *)stack_frame)[10];
    pc_profile_check(&pc);
  }
#endif //DEBUG
}


ticktime_t now(void) {
  return g_now;
}

ticktime_t now_ms(void) {
  return g_now * 1000 / 5000;
}

void reset_timer(void) {
  g_timer = g_now;
}


ticktime_t get_timer(void) {
  return (g_now - g_timer) * 1000 / 5000;
}
