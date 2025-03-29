#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>
#include <stdio.h>
#include "stack.h"
#include "instruction.h"

typedef struct {
	STACK* stack;
	uint16_t sp;
	uint16_t pc;
	uint32_t res;
	uint32_t acc;
	uint8_t flags;
} MACHINE;

MACHINE* machine_init();
void machine_free(MACHINE* machine);

void machine_exec(MACHINE* machine, const char* program);
INSTRUCTION* machine_read_instruction(MACHINE* machine, FILE* program);
void machine_exec_instr(MACHINE* machine, INSTRUCTION* instr);

#endif

