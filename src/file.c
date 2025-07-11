#include "file.h"
#include <string.h>

unsigned char *read_file(const char *file_path, size_t *file_size) {
  FILE *file = fopen(file_path, "rb");
  if (!file) {
    fprintf(stderr, "Failed to open file %s:%s\n", file_path, strerror(errno));
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) == -1) {
    fprintf(stderr, "Failed to set file position indicator: %s\n",
            strerror(errno));
    fclose(file);
    return NULL;
  }

  int print_rom_size = ftell(file);
  if (print_rom_size == -1) {
    fprintf(stderr, "Failed to get file position indicator: %s\n",
            strerror(errno));
    if (fclose(file) == -1) {
      fprintf(stderr, "Failed to close file %s:%s\n", file_path,
              strerror(errno));
      return NULL;
    }
  }

  *file_size = (size_t)print_rom_size;

  rewind(file);

  unsigned char *buffer = (unsigned char *)malloc(*file_size);
  if (!buffer) {
    fprintf(stderr, "Failed to allocate memory for file buffer: %s\n",
            strerror(errno));

    if (fclose(file) == -1) {
      fprintf(stderr, "Failed to close file %s:%s\n", file_path,
              strerror(errno));
    }
    return NULL;
  }

  const size_t bytes_read = fread(buffer, 1, *file_size, file);

  if (bytes_read != *file_size) {
    fprintf(stderr, "Error reading file into buffer.");
    free(buffer);

    if (fclose(file) == -1) {
      fprintf(stderr, "Failed to close file %s:%s\n", file_path,
              strerror(errno));
    }

    return NULL;
  }

  if (fclose(file) == -1) {
    fprintf(stderr, "Failed to close file %s:%s\n", file_path, strerror(errno));
    return NULL;
  }

  return buffer;
}
