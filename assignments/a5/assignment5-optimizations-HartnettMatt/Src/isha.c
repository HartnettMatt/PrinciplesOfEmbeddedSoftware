/*
 * isha.c
 *
 * A completely insecure and bad hashing algorithm, based loosely on
 * SHA-1 (which is itself no longer considered a good hashing
 * algorithm)
 *
 * Based on code for sha1 processing from Paul E. Jones, available at
 * https://www.packetizer.com/security/sha1/
 * Optimized by Matt Hartnett
 */

#include "stdbool.h"
#include "isha.h"
#include "static_profiler.h"

// Do not modify these declarations
uint32_t ISHAProcessMessageBlockEnd, ISHAPadMessageEnd,
    ISHAResetEnd, ISHAResultEnd, ISHAInputEnd;
#define record_pc(x) asm("mov %0, pc" \
                         : "=r"(x))
//----------------------------------------------------------------------

/*
 * circular shift macro - modified to remove unnecessary shifting
 */
#define ISHACircularShift(bits, word) \
    ((((word) << (bits))) | ((word) >> (32 - (bits))))


/*  
 * Processes the next 512 bits of the message stored in the MBlock
 * array.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */
static void ISHAProcessMessageBlock(ISHAContext *ctx) {
    uint32_t temp;
    int t;
    uint32_t W[16];
    uint32_t A, B, C, D, E;

    A = ctx->MD[0];
    B = ctx->MD[1];
    C = ctx->MD[2];
    D = ctx->MD[3];
    E = ctx->MD[4];

    // Load message block into W with byte swapping
    // Un-optimized style:
    //    W[t] = ((uint32_t)ctx->MBlock[t * 4]) << 24;
    register const uint32_t *MBlock32 = (const uint32_t *) ctx->MBlock;
    for (t = 0; t < 16; t++) {
        // This function is a far more efficient way to do the endian swapping
        W[t] = __builtin_bswap32(MBlock32[t]);
    }

    // Remove unnecessary masking:
    //    temp = ISHACircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t];
    //    temp &= 0xFFFFFFFF;
    for (t = 0; t < 16; t++) {
            temp = ISHACircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t];
            E = ISHACircularShift(25, D);
            D = ISHACircularShift(15, C);
            C = ISHACircularShift(30, B);
            B = ISHACircularShift(10, A);
            A = ISHACircularShift(5, temp);
    }

    // Remove unnecessary masking
    // Previous example:
    //  ctx->MD[0] = (ctx->MD[0] + A) & 0xFFFFFFFF;
      ctx->MD[0] = (ctx->MD[0] + A);
      ctx->MD[1] = (ctx->MD[1] + B);
      ctx->MD[2] = (ctx->MD[2] + C);
      ctx->MD[3] = (ctx->MD[3] + D);
      ctx->MD[4] = (ctx->MD[4] + E);

    ctx->MB_Idx = 0;

    // Do not modify this line
    record_pc(ISHAProcessMessageBlockEnd);

    // Static profiler moved to the end for better branching (repeated for all functions):
    #ifdef DEBUG
        if (!static_profiling_on) {
            return;
        }
        ISHAProcessMessageBlockCount++;
    #endif
}


/*  
 * The message must be padded to an even 512 bits.  The first padding
 * bit must be a '1'.  The last 64 bits represent the length of the
 * original message.  All bits in between should be 0. This function
 * will pad the message according to those rules by filling the MBlock
 * array accordingly. It will also call ISHAProcessMessageBlock()
 * appropriately. When it returns, it can be assumed that the message
 * digest has been computed.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */

/*
 * Optimized code:
 * Primary change is the introduction of the pointer p, saves a lot of unnecessary multiplication
 */

static void ISHAPadMessage(ISHAContext *ctx) {
    /*
   *  Check to see if the current message block is too small to hold
   *  the initial padding bits and length.  If so, we will pad the
   *  block, process it, and then continue padding into a second
   *  block.
   */
    uint32_t MB_Idx = ctx->MB_Idx;
    // Created one pointer register to make pointer arthimetic possible and save time
    register uint8_t *p = &ctx->MBlock[MB_Idx];
    if (MB_Idx > 55) {
        *p++ = 0x80;
        MB_Idx++;

        while (MB_Idx < 64) {
            *p++ = 0;
            MB_Idx++;
        }

        ISHAProcessMessageBlock(ctx);

        // Reset index and pointer after processing the block
        // ctx->MB_Idx is reset to 0 in ProcessMessage, removed redunant assignments
        MB_Idx = 0;
        p = ctx->MBlock;

        while (ctx->MB_Idx < 56) {
            ctx->MBlock[ctx->MB_Idx++] = 0;
        }
    } else {
        *p++ = 0x80;
        MB_Idx++;

        while (MB_Idx < 56) {
            *p++ = 0;
            MB_Idx++;
        }
    }
    p = &ctx->MBlock[56];
    uint32_t len_high = ctx->Length_High;
    uint32_t len_low = ctx->Length_Low;

    *p++ = (len_high >> 24) & 0xFF;
    *p++ = (len_high >> 16) & 0xFF;
    *p++ = (len_high >> 8) & 0xFF;
    *p++ = len_high & 0xFF;
    *p++ = (len_low >> 24) & 0xFF;
    *p++ = (len_low >> 16) & 0xFF;
    *p++ = (len_low >> 8) & 0xFF;
    *p++ = len_low & 0xFF;

    ctx->MB_Idx = 64;
    ISHAProcessMessageBlock(ctx);

    // Do not modify this line
    asm("mov %0, pc"
        : "=r"(ISHAPadMessageEnd));

// Static profiler:
#ifdef DEBUG
    if (!static_profiling_on) {
        return;
    }
    ISHAPadMessageCount++;
#endif
}

// Not much optimizing to be done here
void ISHAReset(ISHAContext *ctx) {
    ctx->Length_Low = 0;
    ctx->Length_High = 0;
    ctx->MB_Idx = 0;

    ctx->MD[0] = 0x3829B301;
    ctx->MD[1] = 0xCDADAB89;
    ctx->MD[2] = 0x54BADCFE;
    ctx->MD[3] = 0x1FFF5476;
    ctx->MD[4] = 0xCC394260;

    ctx->Computed = 0;
    ctx->Corrupted = 0;

    // Do not modify this line
    record_pc(ISHAResetEnd);

// Static profiler:
#ifdef DEBUG
    if (!static_profiling_on) {
        return;
    }
    ISHAResetCount++;
#endif
}


void ISHAResult(ISHAContext *ctx, uint8_t *digest_out) {

    if (ctx->Corrupted) {
        return;
    }

    if (!ctx->Computed) {
        ISHAPadMessage(ctx);
        ctx->Computed = 1;
    }

    uint32_t *MD_ptr = ctx->MD;
    uint8_t *digest_ptr = digest_out;
    register uint32_t word;

    // Unrolled loop for i = 0 to 4
    // i = 0
    word = MD_ptr[0];
    *digest_ptr++ = (word >> 24);
    *digest_ptr++ = (word >> 16);
    *digest_ptr++ = (word >> 8);
    *digest_ptr++ = word;

    // i = 1
    word = MD_ptr[1];
    *digest_ptr++ = (word >> 24);
    *digest_ptr++ = (word >> 16);
    *digest_ptr++ = (word >> 8);
    *digest_ptr++ = word;

    // i = 2
    word = MD_ptr[2];
    *digest_ptr++ = (word >> 24);
    *digest_ptr++ = (word >> 16);
    *digest_ptr++ = (word >> 8);
    *digest_ptr++ = word;

    // i = 3
    word = MD_ptr[3];
    *digest_ptr++ = (word >> 24);
    *digest_ptr++ = (word >> 16);
    *digest_ptr++ = (word >> 8);
    *digest_ptr++ = word;

    // i = 4
    word = MD_ptr[4];
    *digest_ptr++ = (word >> 24);
    *digest_ptr++ = (word >> 16);
    *digest_ptr++ = (word >> 8);
    *digest_ptr++ = word;

    // Do not modify this line
    record_pc(ISHAResultEnd);

// Static profiler:
#ifdef DEBUG
    if (!static_profiling_on) {
        return;
    }
    ISHAResultCount++;
#endif
}

void ISHAInput(ISHAContext *ctx, const uint8_t *message_array, size_t length) {

    if (!length) {
        return;
    }

    if (ctx->Computed || ctx->Corrupted) {
        ctx->Corrupted = 1;
        return;
    }

    while (length-- && !ctx->Corrupted) {
        ctx->MBlock[ctx->MB_Idx++] = *message_array; // Deleted unnecessary mask

        ctx->Length_Low += 8;
        // Deleted some unnecessary masks here and there
        if (ctx->Length_Low == 0) {
            ctx->Length_High++;
            // there
            if (ctx->Length_High == 0) {
                /* Message is too long */
                ctx->Corrupted = 1;
            }
        }

        if (ctx->MB_Idx == 64) {
            ISHAProcessMessageBlock(ctx);
        }

        message_array++;
    }

    record_pc(ISHAInputEnd);
// Static profiler:
#ifdef DEBUG
    if (!static_profiling_on) {
        return;
    }
    ISHAInputCount++;
#endif
}


// Do not modify anything below this line

static bool cmp_bin(const uint8_t *b1, const uint8_t *b2, size_t len) {
    for (size_t i = 0; i < len; i++)
        if (b1[i] != b2[i])
            return false;
    return true;
}

void GetFunctionAddress(const char *func_name, uint32_t *start, uint32_t *end) {
    if (cmp_bin((const uint8_t *)func_name, (const uint8_t *)"ISHAProcessMessageBlock", 23)) {
        *start = (uint32_t)ISHAProcessMessageBlock;
        *end = ISHAProcessMessageBlockEnd;
    } else if (cmp_bin((const uint8_t *)func_name, (const uint8_t *)"ISHAPadMessage", 14)) {
        *start = (uint32_t)ISHAPadMessage;
        *end = ISHAPadMessageEnd;
    } else if (cmp_bin((const uint8_t *)func_name, (const uint8_t *)"ISHAReset", 9)) {
        *start = (uint32_t)ISHAReset;
        *end = ISHAResetEnd;
    } else if (cmp_bin((const uint8_t *)func_name, (const uint8_t *)"ISHAResult", 10)) {
        *start = (uint32_t)ISHAResult;
        *end = ISHAResultEnd;
    } else if (cmp_bin((const uint8_t *)func_name, (const uint8_t *)"ISHAInput", 9)) {
        *start = (uint32_t)ISHAInput;
        *end = ISHAInputEnd;
    }

    *end += 100;
}
