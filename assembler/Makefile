FLAGS = -g -lc -Wall -Wextra # -fsanitize=address
# O_FILES = hmap.o util.o sbuilder.o arglist.o interpreter.o
O_FILES = hmap.o


crasm: scanner.l parser.y $(O_FILES)
	bison -d parser.y
	flex scanner.l
	cc $(FLAGS) -o crasm parser.tab.c lex.yy.c $(O_FILES) -lfl

hmap.o: hmap.h

.PHONY: clean
clean:
	rm $(O_FILES) parser.tab.c parser.tab.h lex.yy.c crasm
