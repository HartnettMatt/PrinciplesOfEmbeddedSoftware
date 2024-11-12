/*
 * fsm.c
 *
 * The implementation file of the finite state machine.
 *
 *  Created on: Oct 14, 2024
 *      Author: Matt Hartnett
 */

#include "fsm.h"
#include "log.h"
#include "button.h"
#include "uled.h"
#include "eled.h"
volatile uint32_t interrupt_counter = 0;

void fsm_increment(void) {
    interrupt_counter++;
}

void fsm_run() {
    uint32_t prev_interrupt_counter = 0;
    uint32_t state_counter = 0;
    uint8_t current_brightness = 0;
    uint32_t blink_point = 0;
    uint32_t uled_blink = 0;
    uint8_t blink_sequence[] = {1, 0, 1, 0, 1, 0, 0, 0, 0, 0};
    const uint32_t blink_sequence_length = sizeof(blink_sequence) / sizeof(blink_sequence[0]);
    TrainState next_state = STATE_T, prev_state = STATE_T, current_state = STATE_T;

    // Pre-compute durations
    const uint32_t DURATION_T = STATE_T_DURATION * SYSTICK_INTERRUPT_FREQ;
    const uint32_t DURATION_A = STATE_A_DURATION * SYSTICK_INTERRUPT_FREQ;
    const uint32_t DURATION_B = STATE_B_DURATION * SYSTICK_INTERRUPT_FREQ;
    const uint32_t DURATION_S = STATE_S_DURATION * SYSTICK_INTERRUPT_FREQ;
    const uint32_t DURATION_TRANSITION = STATE_TRANSITION_DURATION * SYSTICK_INTERRUPT_FREQ;

    // Store state info
    const StateInfo state_info[] = {
            [STATE_T] = {BRIGHTNESS_T, DURATION_T, STATE_A, "TERMINAL T"},
            [STATE_A] = {BRIGHTNESS_A, DURATION_A, STATE_B, "CONCOURSE A"},
            [STATE_B] = {BRIGHTNESS_B, DURATION_B, STATE_S, "CONCOURSE B"},
            [STATE_S] = {BRIGHTNESS_S, DURATION_S, STATE_T, "SHED S"},
            [STATE_E] = {0, 0, STATE_E, "EMERGENCY"},
            [STATE_TRANSITION] = {0, DURATION_TRANSITION, STATE_T, "TRANSITION"},
    };

    LOG("MAIN LOOP STARTING...\r\n")
    while (1) {
        if (interrupt_counter != prev_interrupt_counter) {
            prev_interrupt_counter = interrupt_counter;

            // Check for emergency stop
            if (read_button() && current_state != STATE_E) {
                prev_state = current_state;
                current_state = STATE_E;
                uled_blink = 0;
                LOG("EMERGENCY STOP ACTIVATED. SYSTIME: %lu\n\r", interrupt_counter * 100);
            }

            if (current_state == STATE_E) {
                if (!read_button()) {
                    current_state = prev_state;
                    LOG("EMERGENCY STOP RELEASED. SYSTIME: %lu, RESUMING....\n\r", interrupt_counter * 100);
                    uled_off();
                } else {
                    if(blink_sequence[uled_blink]){
                        uled_on();
                    } else {
                        uled_off();
                    }
                    if (uled_blink < blink_sequence_length - 1) {
                        uled_blink++;
                    } else {
                        uled_blink = 0;
                    }
                }
            } else {
                // First iteration through a state:
                if (state_counter == 1) {
                    LOG("ENTERING %s. SYSTIME: %lu\n\r", state_info[current_state].name, interrupt_counter * 100);
                    if (current_state != STATE_TRANSITION) {
                        next_state = state_info[current_state].next_state;
                        current_brightness = state_info[current_state].brightness;
                    }
                }

                // Exit the current state and enter state transition:
                if (state_counter >= state_info[current_state].duration) {
                    if (current_state != STATE_TRANSITION) {
                        current_state = STATE_TRANSITION;
                    } else {
                        current_state = next_state;
                    }
                    state_counter = 0;
                }

                // When in transition state, slowly change the light brightness
                if (current_state == STATE_TRANSITION) {
                    blink_point = 0;
                    // Adjust brightness during transition
                    current_brightness = (next_state == STATE_T) ? (current_brightness + 6) : (current_brightness - 2);
                }

                // Set brightness and handle blinking regardless of state
                eled_set_brightness(current_brightness * blink_sequence[blink_point]);
                state_counter++;

                // Iterate through blinks if not in transition
                if (current_state != STATE_TRANSITION) {
                    if (blink_point < blink_sequence_length - 1) {
                        blink_point++;
                    } else {
                        blink_point = 0;
                    }
                }
            }
        }
    }
}
