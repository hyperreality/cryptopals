#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int frequencyScore(const unsigned char *inp, const size_t inp_len) {
  int freqs[] = {
      8167,  // a
      1492,  // b
      2782,  // c
      4253,  // d
      12702, // e
      2228,  // f
      2015,  // g
      6094,  // h
      6966,  // i
      153,   // j
      772,   // k
      4025,  // l
      2406,  // m
      6749,  // n
      7507,  // o
      1929,  // p
      95,    // q
      5987,  // r
      6327,  // s
      9056,  // t
      2758,  // u
      978,   // v
      2360,  // w
      150,   // x
      1974,  // y
      74,    // z
  };

  int score = 0;

  for (size_t i = 0; i < inp_len; i++) {
    if (inp[i] == 32) {
      score += 10000;
    } else if (inp[i] >= 65 && inp[i] <= 90) {
      score += freqs[inp[i] - 65];
    } else if (inp[i] >= 97 && inp[i] <= 122) {
      score += freqs[inp[i] - 97];
    } else if (inp[i] >= 128) {
      score -= 5000;
    } else {
      score -= 1000;
    }
  }

  return score;
}

double indexCoincidence(const char *inp) {
  int N = 0;
  double sum = 0.0;
  double total = 0.0;

  int values[26] = {0};

  // frequency of each letter in s
  int ch;
  for (size_t i = 0; i < strlen(inp); i++) {
    ch = inp[i];
    if (ch >= 65 && ch <= 90) {
      values[ch - 65]++;
      N++;
    } else if (ch >= 97 && ch <= 122) {
      values[ch - 97]++;
      N++;
    }
  }

  // sum of each frequency
  for (size_t i = 0; i < 26; i++) {
    ch = values[i];
    sum += ch * (ch - 1);
  }

  total = sum / (N * (N - 1));

  return total;
}

