#include <stdint.h>
#include <string.h>

#define N 624
#define TEMPER_MASK_1 0x9d2c5680
#define TEMPER_MASK_2 0xefc60000

typedef struct {
  uint32_t mt[N];
  uint32_t index;
} mt19937_state;

mt19937_state mt19937_initialise(uint32_t seed) {
  mt19937_state s;
  memset(&s, 0, sizeof s);

  s.mt[0] = seed;
  for (size_t i = 1; i < N; i++)
    s.mt[i] = 0x6c078965 * (s.mt[i - 1] ^ (s.mt[i - 1] >> 30)) + i;
  return s;
}

void mt19937_regenerate(mt19937_state *s) {
  for (size_t i = 0; i < N; i++) {
    uint32_t y = s->mt[i] & 0x80000000;
    y += s->mt[(i + 1) % N] & 0x7fffffff;

    uint32_t z = s->mt[(i + 397) % N];
    s->mt[i] = z ^ (y >> 1);

    if (y % 2)
      s->mt[i] ^= 0x9908b0df;
  }
}

uint32_t mt19937_temper(uint32_t y) {
  y ^= y >> 11;
  y ^= (y << 7) & TEMPER_MASK_1;
  y ^= (y << 15) & TEMPER_MASK_2;
  y ^= y >> 18;
  return y;
}

uint32_t mt19937_extract(mt19937_state *s) {
  if (s->index == 0)
    mt19937_regenerate(s);

  uint32_t y = s->mt[s->index];

  s->index++;
  s->index %= N;
  return mt19937_temper(y);
}

#undef N
