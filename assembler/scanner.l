%{
#include "parser.tab.h"
%}

%%
ADD { yylval.opcode = 2; return OPCODE; }
SUB { yylval.opcode = 3; return OPCODE; }
DIV { yylval.opcode = 4; return OPCODE; }
MUL { yylval.opcode = 5; return OPCODE; }
PUSH { yylval.opcode = 6; return OPCODE; }
POP { yylval.opcode = 7; return OPCODE; }
PRINT { yylval.opcode = 8; return OPCODE; }
READ { yylval.opcode = 9; return OPCODE; }
JZ { yylval.opcode = 10; return OPCODE; }
JNZ { yylval.opcode = 11; return OPCODE; }
JLZ { yylval.opcode = 12; return OPCODE; }
JGZ { yylval.opcode = 13; return OPCODE; }
JMP { yylval.opcode = 14; return OPCODE; }

SP { return SP; }
ACC { return ACC; }
RES { return RES; }
FLAGS { return FLAGS; }

"," { return COMMA; }
"-" { return MINUS; }
"(" { return OP; }
")" { return CP; }
"$" { return DOLLAR; }
\n          { return EOL; }
[ \t]       { /* ignore whitespace*/ }
[0-9]+  { yylval.value = atoi(yytext); return LITERAL; }
%%
