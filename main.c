#include <stdio.h>
#include "stack.h"
#include "machine.h"

int main(int argc, char** argv) {
	MACHINE* machine = machine_init();
	machine_exec(machine, "./out");
	// STACK* stack = stack_init();
	// stack_push(stack, 0xAABBCCDD);
	// stack_push(stack, 0xFFFFFFFF);
	// stack_pop32(stack);
	// stack_push(stack, 0xCAFEBABE);
	// stack_print(stack);
	// printf("%#08x\n", stack_pop32(stack));
	// printf("%#08x\n", stack_pop32(stack));
	return 0;
}

