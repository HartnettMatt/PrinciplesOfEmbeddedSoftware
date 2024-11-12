/*
 * File gives the implementation of the static profiler functions.
 *
 *  Created on: Aug 2, 2023
 *      Author: lpandit, Matt Hartnett
 */

#include "static_profiler.h"
#include <stdio.h>

/* Declarations of the counter and profiling flag */
uint32_t ISHAProcessMessageBlockCount, ISHAPadMessageCount, ISHAResetCount, ISHAInputCount, ISHAResultCount;
bool static_profiling_on;

/*
 * Turns the static profiler on.
 */
void static_profile_on(void) {
 ISHAProcessMessageBlockCount = 0;
 ISHAPadMessageCount = 0;
 ISHAResetCount = 0;
 ISHAInputCount = 0;
 ISHAResultCount = 0;
 static_profiling_on = true;
}

/*
 * Turns the static profiler off.
 */
void static_profile_off(void) {
 static_profiling_on = false;
}

/*
 *  Prints the summary of the profiling.
 */
void print_static_profiler_summary(void) {
 printf("Function: ISHAProcessMessageBlock       Call count: %u \n\r", (unsigned int)ISHAProcessMessageBlockCount);
 printf("Function: ISHAPadMessage                Call count: %u \n\r", (unsigned int)ISHAPadMessageCount);
 printf("Function: ISHAResult                    Call count: %u \n\r", (unsigned int)ISHAResultCount);
 printf("Function: ISHAReset                     Call count: %u \n\r", (unsigned int)ISHAResetCount);
 printf("Function: ISHAInput                     Call count: %u \n\r", (unsigned int)ISHAInputCount);
}
