/*
 * PBKDF1.c
 *
 * Application entry point, and test timing.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "pc_profiler.h"


#include "pbkdf1.h"
#include "pbkdf1_test.h"
#include "ticktime.h"

#include "static_profiler.h"


/*
 * Times a single call to the pbkdf1 function, and prints
 * the resulting duration.
 *
 * Parameters:
 *     print_time - Boolean flag to indicate if the time duration should be printed.
 */
static void time_pbkdf1(bool print_time)
{
  const char *pass = "Boulder";
  const char *salt = "Buffaloes";
  int iterations = 4096;
  size_t dk_len = 20;
  int passlen, saltlen;
  uint8_t act_result[512];
  uint8_t exp_result[512];

  const char *exp_result_hex = "AD9C66A545F813558394C66D9302C9EAA59429DD";
    
  ticktime_t duration = 0;

  assert(dk_len <= sizeof(act_result));

  hexstr_to_bytes(exp_result, exp_result_hex, dk_len);passlen = strlen(pass);
  saltlen = strlen(salt);

  reset_timer();
  error_t err = pbkdf1((const uint8_t *)pass, passlen, (const uint8_t *)salt, saltlen,
      iterations, act_result, dk_len);
  duration = get_timer();

  if ((err == NO_ERROR) && cmp_bin(act_result, exp_result, dk_len)) {
	  if (print_time) {
		  printf("%s: %u iterations took %u msec\r\n", __FUNCTION__,
				  iterations, (unsigned int)duration);
	  } else {
		  printf("%s: %u iterations complete\r\n", __FUNCTION__,
					  iterations);
	  }
  } else {
	  if (print_time) {
              printf("FAILURE on timed test duration=%u msec\r\n", (unsigned int)duration);
	  } else {
              printf("FAILURE on tests.\r\n");
	  }
  }
}


/*
 * Run all the validity checks; exit on failure
 */
static void run_tests()
{
  bool success = true;

  success &= test_isha();
  success &= test_pbkdf1();

  if (success)
    return;

  printf("TEST FAILURES EXIST ... exiting\r\n");

  exit(-1);
}


/*
 * Application entry point.
 */
int main(void) {

  /* Init board hardware. */

  init_ticktime();

  printf("Running validity tests...\r\n");
  run_tests();
  printf("All tests passed!\r\n");

  //Time test section 1 for reporting and comparing time.
  printf("Running timing test...Report this time.\r\n");
  time_pbkdf1(true);
  printf("Done with timing test...\r\n");

  //Time test section 2 for profiling with static profiling.
  printf("Running call count test with static profiling....\r\n");
  static_profile_on();
  time_pbkdf1(false);
  static_profile_off();
  print_static_profiler_summary();
  printf("Done with call count test with static profiling....\r\n");

  //Time test section 3 for profiling with PC.
  printf("Running call count test with PC profiling....\r\n");
  pc_profile_on();
  time_pbkdf1(false);
  pc_profile_off();
  print_pc_profiler_summary();
  printf("Done with call count test with PC profiling....\r\n");

  return 0 ;
}


