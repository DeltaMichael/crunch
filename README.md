# Crunch

## How to run

```
make
crunch /path/to/binary/file
```

Run tests

```
cd test
./run_all.py
```

## Binary file format

OPCODE - 8 bits  
ADDRESS - 16 bits  
VALUE - 32 bits  
MAGIC - 32 bits - FEFA FAFC  
DATA SECTIN LENGTH - 16 bits  
CODE LENGTH - 16 bits  
TRAILING ZERO - 8 bits  

```
<MAGIC><DATA SECTION OFFSET><CODE><DATA SECTION LENGTH><DATA SECTION><TRAILING ZERO>


<CODE>
<OPCODE><ADDRESS1><ADDRESS2>
<OPCODE>
<OPCODE><ADDRESS1>
...
<OPCODE><ADDRESS1><ADDRESS2>
<CODE>

<DATA>
<VALUE>
<VALUE>
...
<VALUE>
<DATA>

```

One issue with this format is that you have to differentiate between instructions and address somehow. Since one and the same instruction can have a
variable amount of arguments, this becomes tricky. How do we know if something like `0xA0 0xBB 0xCC 0xA1 0xB2` is `<OPCODE><ADDRESS1><ADDRESS2>` or `<OPCODE><OPCODE><OPCODE><ADDRESS1>`?

We introduce an addressing offset. Since we're dealing with 16-bit addresses, the number of possible values are 0xFFFFFFFF. We have to differentiate between the first byte of an address
and an op-code. This requires that we split the 16-bit address space in two. Our offset becomes 2^8 + 2^15 = 0x8100, which is our offset. Instruction op codes can vary from 0x00 to 0x80
and there is no danger of an opcode overlapping with an address. Addresses range from 0x8100 upward.

The address space can be further split between stack offsets, stack addresses and data section addresses. We also need some dedicated space for registers.

0x8100 - 0x81FF - registers. This allows for 255 registers  
0x8200 - 0x9000 - stack offsets. This allows us to offset the stack pointer by 3584 bytes  
0x9000 - 0xE040 - stack addresses. This sets the stack size at 20544 bytes  
0xE041 - 0XFFFF - data addresses. 8192 bytes  

## Architecture

### Registers

res - 32-bit register - stores the result of the last operation  
acc - 32-bit register - accumulator for adding, subtracting, etc.  
flags - 8-bit register - bits are set based on result of last operation, e.g. if the result was zero, if the result was less than zero, etc.  
sp - 16-bit register - stores the stack pointer  
pc - 16-bit register - stores the program counter  

### Stack

Mainly used to store variables. Each frame is 32-bits wide. To keep it simple and avoid alignment issues,
all numbers are 32 bits wide. This may change in the future.

### Instruction set

Instructions can have zero, one or two arguments. The same instruction can have a variable amount of arguments, e.g.

```
ADD - pops two values off the stack, adds them and pushes the result on the stack
ADD $(SP - 4) - add value at address SP - 4 to ACC
ADD ACC, $(SP - 4) - same as above
ADD $(SP - 4), $(SP - 8) - add value at address SP - 8 to value in address SP - 4 and store the value in at address SP - 4
```

Instructions
```
ADD
SUB
DIV
MUL
PUSH
POP
JNZ
JZ
JMP
```
