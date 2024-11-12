/*
 * fsm.h
 *
 * This header file is for the finite state machine (FSM) that
 * "controls" the airport train. The design for the FSM can be found
 * at the project root directory as "State Machine.pdf"
 *
 *  Created on: Oct 14, 2024
 *      Author: Matt Hartnett
 */

#ifndef FSM_H_
#define FSM_H_

// State timing definitions based on release vs debug
#include <stdint.h>
#ifdef DEBUG
// DEBUG:
#define STATE_T_DURATION 3
#define STATE_A_DURATION 2
#define STATE_B_DURATION 2
#define STATE_S_DURATION 2
#define STATE_TRANSITION_DURATION 4
#else
// RELEASE:
#define STATE_T_DURATION 5
#define STATE_A_DURATION 4
#define STATE_B_DURATION 4
#define STATE_S_DURATION 4
#define STATE_TRANSITION_DURATION 4
#endif

#define SYSTICK_INTERRUPT_FREQ 10
// Define brightness levels
#define BRIGHTNESS_T 0xFF
#define BRIGHTNESS_A 0xAF
#define BRIGHTNESS_B 0x5F
#define BRIGHTNESS_S 0x0F

typedef enum {
 STATE_T,
 STATE_A,
 STATE_B,
 STATE_S,
 STATE_E,
 STATE_TRANSITION
} TrainState;

typedef struct {
 uint8_t brightness;
 uint32_t duration;
 TrainState next_state;
 const char *name;
} StateInfo;

/*
 * Increments the FSM timer counter. Intended to be called by the
 * systick ISR every 100ms.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   none
 */
void fsm_increment(void);

/*
 * Infinite loop that operates the FSM.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   none
 */
void fsm_run(void);

#endif /* FSM_H_ */
