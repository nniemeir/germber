#ifndef MAIN_H
#define MAIN_H

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
