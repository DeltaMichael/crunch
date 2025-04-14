#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

void fwrite_u32(FILE* file, uint32_t data);
void fwrite_u16(FILE* file, uint16_t data);
bool is_little_endian();

#endif

