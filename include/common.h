#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

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

#endif
