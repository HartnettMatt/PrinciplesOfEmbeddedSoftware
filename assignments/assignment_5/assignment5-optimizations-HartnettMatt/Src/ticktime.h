/**
 * @file    ticktime.h
 * @brief   Map SysTick timer into msec counter
 * @author  Howdy Pierce, howdy.pierce@colorado.edu
 */

#ifndef _TICKTIME_H_
#define _TICKTIME_H_

typedef uint32_t ticktime_t;  // time since boot, in sixteenths of a second
                              // 32 bits holds > 3000 days of time

/*
 * Initializes the ticktime counters. Immediately after this function
 * returns, a call to now() will return 0, and a call to reset_timer()
 * will return 0.
 */
void init_ticktime(void);

/*
 * Returns time since init_ticktime() was called, in 20th of a
 * second
 */
ticktime_t now(void);

/*
 *
 */
ticktime_t now_ms(void);

/*
 * Resets the timer. Immediately after this function returns, a call
 * to get_timer() will return 0.
 */
void reset_timer(void);

/*
 * Returns time since the latter of init_ticktime() or reset_timer()
 * was called, in 20th of a second.
 */
ticktime_t get_timer(void);


#endif /* _TICKTIME_H_ */
