/*
 * profiler.c
 *
 *  Created on: Aug 2, 2023
 *      Author: lpandit, Matt Hartnett
 */

#include "pc_profiler.h"
#include "static_profiler.h"
#include "isha.h"
#include <stdio.h>

bool pc_profiling_on = false;

/* Declarations of the function addresses*/
uint32_t ISHAProcessMessageBlockStart, ISHAPadMessageStart, ISHAResetStart, ISHAResultStart, ISHAInputStart;
uint32_t ISHAProcessMessageBlockFinish, ISHAPadMessageFinish, ISHAResetFinish, ISHAResultFinish, ISHAInputFinish;

/*
 *
 */
void pc_profile_check(uint32_t *pc) {
 if(*pc >= ISHAProcessMessageBlockStart && *pc <= ISHAProcessMessageBlockFinish) {
  ISHAProcessMessageBlockCount++;
 } else if(*pc >= ISHAPadMessageStart && *pc <= ISHAPadMessageFinish) {
  ISHAPadMessageCount++;
 } else if(*pc >= ISHAResetStart && *pc <= ISHAResetFinish) {
  ISHAResetCount++;
 } else if(*pc >= ISHAResultStart && *pc <= ISHAResultFinish) {
  ISHAResultCount++;
 } else if(*pc >= ISHAInputStart && *pc <= ISHAInputFinish) {
  ISHAInputCount++;
 }
}

/*
 * Turn the profiler on for call counting.
 */
void pc_profile_on(void) {
 // Init values
 ISHAProcessMessageBlockCount = 0;
 ISHAPadMessageCount = 0;
 ISHAResetCount = 0;
 ISHAInputCount = 0;
 ISHAResultCount = 0;
 pc_profiling_on = true;
 // Find start points of all functions
 uint32_t start, end;
 GetFunctionAddress("ISHAProcessMessageBlock", &start, &end);
 ISHAProcessMessageBlockStart = start;
 ISHAProcessMessageBlockFinish = end;
 GetFunctionAddress("ISHAPadMessage", &start, &end);
 ISHAPadMessageStart = start;
 ISHAPadMessageFinish = end;
 GetFunctionAddress("ISHAReset", &start, &end);
 ISHAResetStart = start;
 ISHAResetFinish = end;
 GetFunctionAddress("ISHAResult", &start, &end);
 ISHAResultStart = start;
 ISHAResultFinish = end;
 GetFunctionAddress("ISHAInput", &start, &end);
 ISHAInputStart = start;
 ISHAInputFinish = end;
}

/*
 * Turn the profiler off for call counting.
 */
void pc_profile_off(void) {
 pc_profiling_on = false;
}

/*
 * Print a summary of profiler calls for following functions.
 *
 */
void print_pc_profiler_summary(void) {
 printf("Function: ISHAProcessMessageBlock       Call count: %u \n\r", (unsigned int)ISHAProcessMessageBlockCount);
 printf("Function: ISHAPadMessage                Call count: %u \n\r", (unsigned int)ISHAPadMessageCount);
 printf("Function: ISHAResult                    Call count: %u \n\r", (unsigned int)ISHAResultCount);
 printf("Function: ISHAReset                     Call count: %u \n\r", (unsigned int)ISHAResetCount);
 printf("Function: ISHAInput                     Call count: %u \n\r", (unsigned int)ISHAInputCount);
}
