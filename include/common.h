#ifndef COMMON_H
#define COMMON_H

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PATH_MAX 4096

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
#define BIT_SET(a, n, on)                                                      \
  {                                                                            \
    if (on)                                                                    \
      a |= (1 << n);                                                           \
    else                                                                       \
      a &= ~(1 << n);                                                          \
  }
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

#define NO_IMPL                                                                \
  {                                                                            \
    fprintf(stderr, "NOT YET IMPLEMENTED\n");                                  \
    exit(-5);                                                                  \
  }

struct key_value {
  unsigned char key;
  unsigned int value;
};

struct hex_string {
  unsigned char hex;
  char *string;
};

struct chars_string {
  char key[3];
  char *string;
};

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

u32 get_ticks(void);
void delay(u32 ms);

#endif
