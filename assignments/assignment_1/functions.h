/** DEPRECATED FILE, DO NOT USE
 * File Name: functions.h
 *  Author: Matt Hartnett
 *  Tools: gcc
 *  Dependancies: stdint
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdint.h>
#include <stddef.h>


typedef enum {
    CLEAR,
    SET,
    TOGGLE
} operation_t;


uint8_t rotate_right(uint8_t num, uint8_t nbits);


uint32_t binstr_to_uint(const char *str);


int32_t int_to_binstr(char *str, size_t size, int32_t num, uint8_t nbits);


uint32_t hexstr_to_uint(const char *str);


uint32_t twiggle_except_bit(uint32_t input, int bit, operation_t operation);


uint32_t grab_four_bits(uint32_t input, int start_bit);


char *hexdump(char *str, size_t size, const void *loc, size_t nbytes);

#endif // FUNCTIONS_H
