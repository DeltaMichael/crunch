#include <stdlib.h>
#include <stdint.h>
#include "instruction.h"

INSTRUCTION* instr_init(uint8_t opcode, uint32_t address1, uint32_t address2) {
	INSTRUCTION* out = malloc(sizeof(INSTRUCTION));
	return out;
}

void instr_free(INSTRUCTION* instr) {
	free(instr);
	instr = NULL;
}

