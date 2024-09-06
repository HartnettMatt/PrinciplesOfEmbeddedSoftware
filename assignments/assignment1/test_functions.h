/** File Name: test_functions.h
 *  Author: Matt Hartnett
 *  Tools: gcc
 *  Dependancies: stdint
 */

#ifndef TEST_FUNCTIONS_H
#define TEST_FUNCTIONS_H

#include <stdint.h>

/**
 * @brief Tests the rotate_right function.
 * 
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_rotate_right(void);

/**
 * @brief Tests the binstr_to_uint function.
 * 
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_binstr_to_uint(void);

/**
 * @brief Tests the int_to_binstr function.
 * 
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_int_to_binstr(void);

/**
 * @brief Tests the hexstr_to_uint function.
 * 
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_hexstr_to_uint(void);

/**
 * @brief Tests the twiggle_except_bit function.
 * 
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_twiggle_except_bit(void);

/**
 * @brief Tests the grab_four_bits function.
 * 
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_grab_four_bits(void);

/**
 * @brief Tests the hexdump function.
 * 
 * @return 0 if the test passes, otherwise non-zero.
 */
uint8_t test_hexdump(void);

#endif // TEST_FUNCTIONS_H
