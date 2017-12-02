#include <stdint.h>
#include <string.h>

#define mt19937_N 624
#define _TEMPER_MASK_1 0x9d2c5680
#define _TEMPER_MASK_2 0xefc60000

typedef struct {
  uint32_t mt[mt19937_N];
  uint32_t index;
} mt19937_state;

mt19937_state mt19937_initialise(uint32_t seed) {
  mt19937_state s;
  memset(&s, 0, sizeof s);

  s.mt[0] = seed;
  for (size_t i = 1; i < mt19937_N; i++)
    s.mt[i] = 0x6c078965 * (s.mt[i - 1] ^ (s.mt[i - 1] >> 30)) + i;
  return s;
}

void mt19937_regenerate(mt19937_state *s) {
  for (size_t i = 0; i < mt19937_N; i++) {
    uint32_t y = s->mt[i] & 0x80000000;
    y += s->mt[(i + 1) % mt19937_N] & 0x7fffffff;

    uint32_t z = s->mt[(i + 397) % mt19937_N];
    s->mt[i] = z ^ (y >> 1);

    if (y % 2)
      s->mt[i] ^= 0x9908b0df;
  }
}

uint32_t mt19937_temper(uint32_t y) {
  y ^= y >> 11;
  y ^= (y << 7) & _TEMPER_MASK_1;
  y ^= (y << 15) & _TEMPER_MASK_2;
  y ^= y >> 18;
  return y;
}

uint32_t mt19937_extract(mt19937_state *s) {
  if (s->index == 0)
    mt19937_regenerate(s);

  uint32_t y = s->mt[s->index];

  s->index++;
  s->index %= mt19937_N;
  return mt19937_temper(y);
}

/* uint32_t _undo_shift_2(uint32_t y) { */
/*   uint32_t t = y; */

/*   for (int i = 0; i < 5; i++) { */
/*     t <<= 7; */
/*     t = y ^ (t & _TEMPER_MASK_1); */
/*   } */

/*   return t; */
/* } */

/* uint32_t _undo_shift_1(uint32_t y) { */
/*   uint32_t t = y; */

/*   for (int i = 0; i < 2; i++) { */
/*     t >>= 7; */
/*     t ^= y; */
/*   } */

/*   return t; */
/* } */

/* uint32_t mt19937_untemper(uint32_t y) { */
/*   y ^= y >> 18; */
/*   y ^= ((y << 15) & _TEMPER_MASK_2); */

/*   y = _undo_shift_2(y); */
/*   y = _undo_shift_1(y); */

/*   return y; */
/* } */

uint32_t _unshift_right(uint32_t v, unsigned shift) {
  uint32_t z = 0;

  for (size_t i = 0; i * shift < 32; i++) {
    uint32_t mask = (0xffffffff << (32 - shift)) >> (shift * i);
    uint32_t part = v & mask;
    v ^= part >> shift;
    z |= part;
  }

  return z;
}

uint32_t _unshift_left_mask(uint32_t v, unsigned shift, uint32_t mask) {
  uint32_t z = 0;

  for (size_t i = 0; i * shift < 32; i++) {
    uint32_t pmask = (0xffffffff >> (32 - shift)) << (shift * i);
    uint32_t part = v & pmask;
    v ^= (part << shift) & mask;
    z |= part;
  }

  return z;
}

uint32_t mt19937_untemper(uint32_t y) {
  uint32_t r = y;

  r = _unshift_right(r, 18);
  r = _unshift_left_mask(r, 15, _TEMPER_MASK_2);
  r = _unshift_left_mask(r, 7, _TEMPER_MASK_1);
  r = _unshift_right(r, 11);

  return r;
}
