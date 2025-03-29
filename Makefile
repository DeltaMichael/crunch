FLAGS = -g -lc -Wall -Wextra # -fsanitize=address
O_FILES = stack.o instruction.o machine.o main.o

crunch: $(O_FILES)
	cc $(FLAGS) -o crunch $(O_FILES)

stack.o: stack.h
instruction.o: instruction.h
machine.o: stack.h machine.h instruction.h

.PHONY: clean
clean:
	rm $(O_FILES)
