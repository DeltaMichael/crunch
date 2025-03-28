#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

STACK* stack_init() {
	STACK* out = malloc(sizeof(STACK));
	out->data = calloc(0xFFFF - STACK_OFFSET, sizeof(uint8_t));
	out->pointer = 0;
	return out;
}

void stack_free(STACK* stack) {
	if(stack == NULL) {
		return;
	}
	if(stack->data) {
		free(stack->data);
		stack->data = NULL;
	}
	free(stack);
	stack = NULL;
}

void stack_push(STACK* stack, uint32_t value) {
	// TODO: check for overflow
	uint32_t mask = 0xFF000000;
	uint8_t offset = 24;
	while(mask) {
		uint8_t result = (value & mask) >> offset;
		mask >>= 8;
		offset -= 8;
		stack->data[stack->pointer] = result;
		stack->pointer++;
	}
}

uint32_t stack_pop32(STACK* stack) {
	if(stack->pointer == 0) {
		return 0;
	}
	stack->pointer--;
	uint32_t out = 0;
	for(int i = 3; i >= 1; i--) {
		out |= stack->data[stack->pointer - i];
		out = out << 8;
	}
	out |= stack->data[stack->pointer];
	stack->pointer -= 3;
	return out;
}

uint32_t stack_get32(STACK* stack, uint16_t address) {
	return 0;
}

void stack_write32(STACK* stack, uint16_t address, uint32_t value) {
}

void stack_print(STACK* stack) {
	for(int i = 0; i < 32; i++) {
		if(i % 4 == 0) {
			printf("\n");
		} else {
			printf(" ");
		}
		printf("%02X",stack->data[i]);
	}
	printf("\n");
}

