#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

unsigned char *read_file(const char *file_path, size_t *file_size);

#endif
