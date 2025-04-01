#include <stdio.h>
#include "stack.h"
#include "machine.h"

int main(int argc, char** argv) {
	if(argc != 2) {
		printf("Usage: crunch path/to/binary/file");
	}
	MACHINE* machine = machine_init();
	machine_exec(machine, argv[1]);
	return 0;
}

