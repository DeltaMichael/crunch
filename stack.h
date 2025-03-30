#ifndef STACK_H
#define STACK_H

#define STACK_OFFSET 0x8100
#define STACK_SIZE 0xE040
#define DATA_OFFSET 0xE040

#include <stdint.h>

typedef struct {
	uint8_t* data;
	uint16_t pointer;
} STACK;

STACK* stack_init();
void stack_free(STACK* stack);
void stack_push(STACK* stack, uint32_t value);
uint32_t stack_pop32(STACK* stack);
uint32_t stack_get32(STACK* stack, uint16_t address);
void stack_write32(STACK* stack, uint16_t address, uint32_t value);
void stack_print(STACK* stack);

#endif

