#include "hexdump.h"
#include <stddef.h>
#include <stdint.h>

char *hexdump(char *str, size_t size, const void *loc, size_t nbytes) {
    if (nbytes > NBYTES_MAXSIZE) {
        *str = '\0';
        return str;
    }
    if (size == 0) {
        return str;
    }

    // Define the characters used for hexadecimal representation.
    const char hex_chars[] = "0123456789ABCDEF";
    // Create a pointer to the memory location.
    const unsigned char *mem = (const unsigned char *)loc;

    // Index for memory bytes
    uint32_t i = 0;
    // Index for output string
    uint32_t o = 0;

    while (i < nbytes) {
        if ((i & 0b111) == 0) { // If i is divisible by 8
            // Create a readable integer type for the memory location
            uintptr_t address = (uintptr_t)(loc + i);
            // Add memory address location to str
            if (i != 0) {
                // Add newline if this isn't the first line
                // Added carriage return to look nicer in serial output for serialio
                str[o] = '\r';
                o++;
                str[o] = '\n';
                o++;
            }
            str[o] = '0';
            o++;
            str[o] = 'x';
            o++;
            // Add address itself
            for (int j = BUS_SIZE - 4; j >= 0; j -= 4) {
                str[o] = hex_chars[((address >> j) & 0xF)];
                o++;
            }
            str[o] = ' ';
            o++;
        }

        // Add a single byte to str
        unsigned char byte = mem[i]; // Get the byte from memory
        // Convert the high nibble (4 bits) to a hex character
        unsigned char high_nibble = (byte >> 4) & 0x0F;
        str[o] = hex_chars[high_nibble];
        o++;
        // Convert the low nibble (4 bits) to a hex character
        unsigned char low_nibble = byte & 0x0F;
        str[o] = hex_chars[low_nibble];
        o++;
        str[o] = ' ';
        o++;

        // Increment to the next memory address
        i++;
    }

    str[o] = '\0';
    return str;
}