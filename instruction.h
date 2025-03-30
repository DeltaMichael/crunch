#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

enum OPCODE {
	ADD = 2,
	SUB,
	DIV,
	MUL,
	PUSH,
	POP,
	PRINT,
	READ,
	JZ,
	JNZ,
	JLZ,
	JGZ,
	JMP
};

typedef struct {
	uint8_t opcode;
	uint16_t address1;
	uint16_t address2;
} INSTRUCTION;

INSTRUCTION* instr_init(uint8_t opcode, uint32_t address1, uint32_t address2);
INSTRUCTION* instr_from_bytes(uint8_t* bytes, int length);
void instr_free(INSTRUCTION* instr);

#endif

