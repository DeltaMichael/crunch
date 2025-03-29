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
	machine->sp = 0;
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
	uint16_t data_length;
	uint8_t byte;

	// read magic number
	read_4_bytes(bin, &magic);
	printf("MAGIC: %08X\n", magic);
	assert(magic == 0xFEFAFAFC);

	// read data section length
	read_2_bytes(bin, &data_length);
	printf("DATA_LENGTH: %04X\n", data_length);

	printf("DATA: \n");
	while(data_length > 0) {
		data_length--;
		read_1_byte(bin, &byte);
		printf("%02X ", byte);
	}
	printf("\n");
}

INSTRUCTION* machine_read_instruction(MACHINE* machine, FILE* program) {
	return NULL;
}

void machine_exec_instr(MACHINE* machine, INSTRUCTION* instr) {

}

