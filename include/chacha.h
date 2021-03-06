/* Public domain */

#ifndef CHACHA_H
#define CHACHA_H


#include "config.h"
#include "integer.h"
#include <stddef.h>

/* The ChaCha stream cipher, by djb.
   https://cr.yp.to/chacha.html

   The use of ChaCha as a PRNG is described here:
   https://docs.rs/rand_chacha/0.3.0/rand_chacha/struct.ChaCha8Rng.html#fn1

   Desirable properties (for setseed support):
   - Stateless
   - For a given key, there are 2^64 different pseudo-random streams (selected
     using an incrementing integer stream_id)
   - Seeking in a stream is constant time

   Taken together, this means that in NetHack we can:
   - create and store a single strong 32-byte key per game
   - on top of that, for each RNG we only have to store one 64-bit integer (the
     position in the random stream)
   - each RNG still needs a 64-bit identifier, but we can create this
     deterministically; this ID needn't be saved anywhere

   Other properties that are good to know:
   - Reasonably fast, though this implementation is not optimised; it runs at
     around 8 cycles/byte on modern machines.
   - Generates random data in blocks of 64 bytes at a time.
   - Cryptographically strong (although we use the weakened 8-round version, as
     a performance tradeoff)
 */
void chacha_8rounds_prng(uint32_t output[16], const uint8_t seed[32], uint64_t stream_id, uint64_t pos);

/* '=' is valid as a padding character */
#define is_valid_b64_char(c) (('A' <= c && c <= 'z') || ('0' <= c && c <= '9') || c == '+' || c == '/')
boolean is_valid_b64(char *src, size_t src_len);
size_t b64_encode(char *src, char *dest, size_t src_len);
size_t b64_decode(char *src, char *dest, size_t src_len);
#endif
