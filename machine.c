#include "machine.h"
#include <stdlib.h>
#include <assert.h>

void read_x_bytes(FILE* binary, int num_bytes, uint32_t* dest) {
	uint8_t byte = 0x00;
	for (int i = 0; i < num_bytes; i++) {
		*dest = *dest << 8;
		fread(&byte, sizeof(uint8_t), 1, binary);
		*dest = *dest | byte;
	}
}

void read_4_bytes(FILE* binary, uint32_t* dest) {
	read_x_bytes(binary, 4, dest);
}

void read_2_bytes(FILE* binary, uint16_t* dest) {
	uint8_t byte = 0x00;
	for (int i = 0; i < 2; i++) {
		*dest = *dest << 8;
		fread(&byte, sizeof(uint8_t), 1, binary);
		*dest = *dest | byte;
	}
}

void read_1_byte(FILE* binary, uint8_t* dest) {
	fread(dest, sizeof(uint8_t), 1, binary);
}

MACHINE* machine_init() {
	MACHINE* machine = malloc(sizeof(MACHINE));
	machine->stack = stack_init();
	machine->sp = &machine->stack->pointer;
	machine->pc = 0;
	machine->res = 0;
	machine->acc = 0;
	machine->flags = 0;
	return machine;
}

void machine_free(MACHINE* machine) {
	if(machine == NULL) {
		return;
	}
	stack_free(machine->stack);
	machine->stack = NULL;
	free(machine);
	machine = NULL;
}



void machine_exec(MACHINE* machine, const char* program) {
	FILE* bin = fopen(program, "rb");
	uint32_t magic;
	uint16_t data_offset;
	uint16_t program_length;
	uint8_t byte;

	// read magic number
	read_4_bytes(bin, &magic);
	assert(magic == 0xFEFAFAFC);

	// read data offset
	read_2_bytes(bin, &data_offset);
	machine->data_offset = data_offset;

	program_length = data_offset - 8;

	INSTRUCTION* instr = malloc(sizeof(INSTRUCTION));

	while(program_length > 0) {
		program_length -= machine_read_instruction(machine, bin, instr);
		machine_exec_instr(machine, bin, instr);
	}
}

int machine_read_instruction(MACHINE* machine, FILE* bin, INSTRUCTION* instr) {
	uint8_t opcode = 0;
	uint16_t address1 = 0;
	uint16_t address2 = 0;
	read_1_byte(bin, &opcode);

	read_2_bytes(bin, &address1);
	if(address1 < STACK_OFFSET) {
		fseek(bin, -2, SEEK_CUR);
		instr->opcode = opcode;
		instr->address1 = 0;
		instr->address2 = 0;
		return 1;
	}

	read_2_bytes(bin, &address2);

	if(address2 < STACK_OFFSET) {
		fseek(bin, -2, SEEK_CUR);
		instr->opcode = opcode;
		instr->address1 = address1;
		instr->address2 = 0;
		return 3;
	}

	instr->opcode = opcode;
	instr->address1 = address1;
	instr->address2 = address2;
	return 5;
}

void machine_exec_instr(MACHINE* machine, FILE* bin, INSTRUCTION* instr) {
	// printf("OPCODE: %02X ADDR1: %04X ADDR2: %04X\n", instr->opcode, instr->address1, instr->address2);
	switch (instr->opcode) {
		case PUSH:
			if(instr->address1 != 0 && instr->address1 >= DATA_OFFSET) {
				uint32_t data = machine_read_data_uint32(machine, bin, instr->address1);
				// printf("NUM: %08X\n", data);
				stack_push(machine->stack, data);
			} else if (instr->address1 != 0 && instr->address1 >= STACK_OFFSET) {

			} else {
				// TODO: handle error
			}
			break;
		case ADD:
			uint32_t first = stack_pop32(machine->stack);
			uint32_t second = stack_pop32(machine->stack);
			stack_push(machine->stack, first + second);
			break;
		case PRINT:
			uint32_t out = stack_pop32(machine->stack);
			printf("%d", out);
			break;
	}
}

uint32_t machine_read_data_uint32(MACHINE* machine, FILE* bin, uint16_t address) {
	uint32_t result;
	long curr = ftell(bin);
	long offset = machine->data_offset + address - DATA_OFFSET;
	fseek(bin, offset, SEEK_SET);
	read_4_bytes(bin, &result);
	fseek(bin, curr, SEEK_SET);
	return result;
}

