#include "util.h"

bool is_little_endian() {
	int n = 1;
	return *(char *)&n == 1;
}

void fwrite_u32(FILE* file, uint32_t data) {
	if(is_little_endian()) {
		printf("BEFORE: %08X\n", data);
		uint32_t out = 0;
		uint32_t mask = 0x000000FF;
		for(int i = 3; i >= 0; i--) {
			uint32_t mid = (data & mask);
			mid = mid << 8 * i;
			out = out | mid;
			data = data >> 8;
		}
		printf("AFTER: %08X\n", out);
		fwrite(&out, sizeof(uint32_t), 1, file);
	} else {
		fwrite(&data, sizeof(uint32_t), 1, file);
	}
}

void fwrite_u16(FILE* file, uint16_t data) {
	if(is_little_endian()) {
		printf("BEFORE: %04X\n", data);
		uint16_t out = 0;
		uint16_t mask = 0x00FF;
		for(int i = 1; i >= 0; i--) {
			uint16_t mid = (data & mask);
			mid = mid << 8 * i;
			out = out | mid;
			data = data >> 8;
		}
		printf("AFTER: %04X\n", out);
		fwrite(&out, sizeof(uint16_t), 1, file);
	} else {
		fwrite(&data, sizeof(uint16_t), 1, file);
	}
}

