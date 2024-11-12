/*
 * pbkdf1_test.c
 *
 * Test functions for pbkdf1
 * 
 */

#include "pbkdf1_test.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "isha.h"
#include "pbkdf1.h"


/* 
 * Compares two byte strings; returns true if they match, false
 * otherwise
 * 
 * Parameters:
 *   b1, b2    The strings to be compared
 *   len       The length of the strings
 */
bool cmp_bin(const uint8_t *b1, const uint8_t *b2, size_t len)
{
  for (size_t i=0; i<len; i++)
    if (b1[i] != b2[i])
      return false;
  return true;
}

/*
 * Returns the integer corresponding to the given hex character
 */
static unsigned char hexdigit_to_int(char c)
{
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  if (c >= '0' && c <= '9')
    return c - '0';
  assert(0);

  return 0; //To silence the compiler warning.
}


/*
 * Convert a hex string into a binary value
 *
 * Parameters:
 *   out         Output binary value
 *   in_str      Input hex string
 *   binary_len  The length of the binary output, in bytes. 
 *                 in_str must be 2x this length
 * 
 * Returns:
 *   The converted string, in out
 */
void hexstr_to_bytes(uint8_t *out, const char *in_str, size_t binary_len)
{
  for (size_t i=0; i<binary_len; i++) 
    out[i] = hexdigit_to_int(in_str[i*2]) << 4 | hexdigit_to_int(in_str[i*2+1]);
}

/*
 * Convert a byte array into a hex string.
 *
 * Parameters:
 *     out_str      Output hex string
 *     in           Input binary array
 *     binary_len   Length of binary input
 *
 */
void bytes_to_hexstr(char *out_str, const uint8_t *in, size_t binary_len)
{
   int i;

   char *ptr = out_str;
   for (i=0; i < binary_len; i++)
   {
	   ptr += sprintf(ptr, "%02X", in[i]);
   }
}

#define min(a,b)  ((a)<(b)?(a):(b))

/*
 * Tests the ISHA functions. Returns true if all tests pass, false
 * otherwise. Diagnostic information is printed via PRINTF.
 */
bool test_isha()
{
  typedef struct {
    const char *msg;
    const char *hexdigest;
  } test_matrix_t;

  test_matrix_t tests[] =
    { {"abc", "7F2DFE0BE05CC41AD34DA2C06A71BFBBFA99516D"},
      {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
       "EE436697BC91866B1493D43C840DDEBBE89E2DA9"},
      {"Nobody inspects the spammish repetition",
       "326EAB504ADE84651A2F0529158B7D99EAA5EDB3"},
      {"Now his life is full of wonder/But his heart still knows some fear/Of a simple thing, he cannot comprehend/Why they try to tear the mountains down/To bring in a couple more/More people, more scars upon the land", "E9CEFF83AB2F8AD812537F3B7A8D805CB5F03143"}
    };

  const int num_tests = sizeof(tests) / sizeof(test_matrix_t);
  int tests_passed = 0;
  uint8_t exp_digest[ISHA_DIGESTLEN];
  uint8_t act_digest[ISHA_DIGESTLEN];
  int msglen;
  ISHAContext ctx;
      
  // First time through: Single call to ISHAInput per test
  for (int i=0; i<num_tests; i++) {
    msglen = strlen(tests[i].msg);
    hexstr_to_bytes(exp_digest, tests[i].hexdigest, ISHA_DIGESTLEN);

    ISHAReset(&ctx);
    ISHAInput(&ctx, (const unsigned char *)tests[i].msg, msglen);
    ISHAResult(&ctx, act_digest);

    if (cmp_bin(act_digest, exp_digest, ISHA_DIGESTLEN)) {
      printf("%s test %d: success\r\n", __FUNCTION__, i);
      tests_passed++;
    } else {
      printf("%s test %d: FAILURE\r\n", __FUNCTION__, i);
    }
  }

  // Second time through: Deliver data in 10-byte chunks to ISHAInput
  for (int i=0; i<num_tests; i++) {
    const unsigned char *data = (const unsigned char *)tests[i].msg;
    msglen = strlen(tests[i].msg);
    hexstr_to_bytes(exp_digest, tests[i].hexdigest, ISHA_DIGESTLEN);

    ISHAReset(&ctx);
    while (msglen > 0) {
      int bytes_this_pass = min(10, msglen);
      ISHAInput(&ctx, data, bytes_this_pass);
      msglen -= bytes_this_pass;
      data += bytes_this_pass;
    }

    ISHAResult(&ctx, act_digest);

    if (cmp_bin(act_digest, exp_digest, ISHA_DIGESTLEN)) {
      printf("%s test %d: success\r\n", __FUNCTION__, i + num_tests);
      tests_passed++;
    } else {
      printf("%s test %d: FAILURE\r\n", __FUNCTION__, i + num_tests);
    }
  }

  return (num_tests*2 == tests_passed);
}


/*
 * Tests the pbkdf1 function. Returns true if all tests pass, false
 * otherwise. Diagnostic information is printed via PRINTF.
 */
bool test_pbkdf1()
{
  typedef struct {
    const char *pass;
    const char *salt;
    int iterations;
    size_t dk_len;
    const char *hex_result;  // expected result, as a hex string
    bool status;
  } test_matrix_t;

  test_matrix_t tests[] =
    { {"password", "mysalt", 1, 20, "C559D909AEC289FA804BE6B877587EEF88A91C67", true},
      {"password", "mysalt", 1, 21, "", false},
      {"password", "mysalt", 1, 32, "", false},
      {"password", "mysalt", 1, 12, "C559D909AEC289FA804BE6B877587EEF88A91C67", true},
      {"password", "mysalt", 1, 200, "", false},
      {"password", "mysalt", 2, 20, "A5C2B1FCF4295609AA9D9C57F48B6CBC22F213E4", true},
      {"password", "mysalt", 2, 2, "A5C2B1FCF4295609AA9D9C57F48B6CBC22F213E4", true},
      {"password", "mysalt", 3, 20, "3A0F0A597D5208BE10AB0E3466F8FD028EC67FA1", true},
      {"password", "mysalt", 100, 20, "C05FAB102E26CCCAA38F5698F0A96F5BBDB827D4", true},
      {"password", "mysalt", 100, 9, "C05FAB102E26CCCAA38F5698F0A96F5BBDB827D4", true},
      {"Boulder", "Buffaloes", 100, 64, "", false}
    };
  const int num_tests = sizeof(tests) / sizeof(test_matrix_t);
  int tests_passed = 0;
  uint8_t exp_result[512];
  uint8_t act_result[512];
  int passlen, saltlen;
      
  for (int i=0; i<num_tests; i++) {

    passlen = strlen(tests[i].pass);
    saltlen = strlen(tests[i].salt);

    assert(tests[i].dk_len <= sizeof(exp_result));

    error_t ret = pbkdf1((const uint8_t *)tests[i].pass, passlen,
        (const uint8_t *)tests[i].salt, saltlen, tests[i].iterations, act_result, tests[i].dk_len);

    if (tests[i].status == true) {
        hexstr_to_bytes(exp_result, tests[i].hex_result, tests[i].dk_len);
    }

    if (ret != NO_ERROR && tests[i].status == false) {
        printf("%s test %d: success\r\n", __FUNCTION__, i);
        tests_passed++;
    }
    else if (ret == NO_ERROR && tests[i].status == true && cmp_bin(act_result, exp_result, tests[i].dk_len)) {
      printf("%s test %d: success\r\n", __FUNCTION__, i);
      tests_passed++;
    } else {
      printf("%s test %d: FAILURE\r\n", __FUNCTION__, i);
    }
  }

  return (num_tests == tests_passed);
}

