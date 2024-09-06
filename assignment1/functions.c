/** DEPRECATED FILE, DO NOT USE
 * File Name: functions.c
 *  Author: Matt Hartnett
 *  Tools: gcc
 *  Dependancies: stdint, functions.h
 */

#include "functions.h"
#include <stdint.h>
/** TODO:
 *  Replace error results with macros
 */

uint8_t rotate_right(uint8_t num, uint8_t nbits) {
  // Any bitshift >= 8 is circular, so eliminate any unnecessary shifts
  nbits = nbits & (ROTATE_BITSIZE - 1);
  uint8_t temp = num;
  num = num >> nbits;
  num = num | (temp << (ROTATE_BITSIZE - nbits));
  return num;
}

uint32_t binstr_to_uint(const char *str) {
  const char *crawler = str;
  // Check that both the pointer is not null and the character is not null, and the string starts with 0b
  if (crawler == NULL || *crawler == '\0' || *crawler != '0' || *(crawler + 1) != 'b') {
    return ERROR;
  }

  crawler = crawler + 2; // Jump to the first character of the binary string
  uint32_t num = 0;      // Return value
  uint8_t i = 0;         // Counter to check that input size is >= 32 bits

  // Loop through the string and add 1s and 0s to num appropriately
  while (*crawler != '\0') {
    // Check for over sized input
    if (i >= BINSTR_BITSIZE) {
      return ERROR;
    }

    // Build output num
    num = num << 1;
    if (*crawler == '1') {
      // Set the least significant bit to 1
      num = num | 1;
    } else if (*crawler != '0') {
      // Non binary character (skip if '0', since 0 is automatically added)
      return ERROR;
    }

    // Iterate through string and counter
    crawler++;
    i++;
  }
  return num;
}

int32_t int_to_binstr(char *str, size_t size, int32_t num, uint8_t nbits) {
  // Calculate the minimum number of bits required to represent num
  uint8_t required_bits = 0;
  int32_t temp = num;
  // Two's complement require 32 bits for negative numbers
  if (num < 0) {
    required_bits = INTTOBINSTR_BITSIZE;
  }
  // num = 0 edgecase (requires 1 bit to represent)
  else if (num == 0) {
    required_bits = 1;
  }
  // Count the number of bits
  else {
    while (temp > 0) {
      temp = temp >> 1;
      required_bits++;
    }
  }

  // Check if nbits is big enough to store num
  if (required_bits > nbits) {
    *str = '\0';
    return ERROR;
  }

  // Check if str buffer has enough space for nbits + 2
  if (size < (nbits + 2)) {
    *str = '\0';
    return ERROR;
  }

  char *crawler = str;
  // Append '0b' to the string
  *crawler = '0';
  crawler++;
  *crawler = 'b';
  crawler++;

  // Loop through the bits of num, add them as chars to the string
  for (uint8_t i = nbits; i > 0; i--) {
    // If 1, add 1, if 0, add 0
    *crawler = ((num >> (i - 1)) & 1) ? '1' : '0';
    crawler++;
  }

  // Terminate string
  *crawler = '\0';
  return nbits + 2;
}

uint32_t hexstr_to_uint(const char *str) {
  const char *crawler = str;
  // Check that both the pointer is not null and the character is not null, and the string starts with 0x or 0X
  if (crawler == NULL || *crawler == '\0' || *crawler != '0' || ~(*(crawler + 1) == 'x' || *(crawler + 1) == 'X')) {
    return ERROR;
  }

  crawler = crawler + 2; // Jump to the first character of the hex string
  
  return 0;
}

uint32_t twiggle_except_bit(uint32_t input, int bit, operation_t operation) {
  return 0;
}

uint32_t grab_four_bits(uint32_t input, int start_bit) {
  return 0;
}

char *hexdump(char *str, size_t size, const void *loc, size_t nbytes) {
  return 0;
}