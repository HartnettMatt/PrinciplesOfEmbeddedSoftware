/**
 * @file    hexdump.h
 * @brief The header file for hexdump
 *
 *
 * @author Matt Hartnett
 * @date   11/10/2024
 *
 */

#ifndef HEXDUMP_H
#define HEXDUMP_H

#include <stddef.h>

#define NBYTES_MAXSIZE 65535
#define BUS_SIZE 32

/**
 * @brief Creates a hexadecimal dump of a memory location.
 *
 * @param str The buffer to store the hexadecimal dump.
 * @param size The size of the buffer.
 * @param loc The memory location to dump.
 * @param nbytes The number of bytes to dump.
 * @return A pointer to the buffer containing the hexadecimal dump.
 */
char *hexdump(char *str, size_t size, const void *loc, size_t nbytes);

#endif  /* HEXDUMP_H */