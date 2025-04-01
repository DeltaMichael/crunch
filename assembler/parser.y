%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdint.h>

%}

%code requires {
	typedef struct {
		uint8_t opcode;
		uint16_t address1;
		uint16_t address2;
	} INSTRUCTION;
}

%union {
    uint32_t value;
    uint16_t address;
    uint8_t opcode;
	INSTRUCTION* instruction;
}

%token <value> LITERAL
%token <opcode> OPCODE
%token MINUS
%token OP
%token CP
%token COMMA
%token DOLLAR
%token SP
%token RES
%token ACC
%token FLAGS
%token EOL

%type <instruction> instr
%type <address> register
%type <address> offset

%%

instrlist:
       | instrlist instr EOL {printf("%02X %04X %04X\n", $2->opcode, $2->address1, $2->address2);}
       ;

instr: OPCODE offset COMMA offset { INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION)); out->opcode = $1; out->address1 = $2; out-> address2 = $4; $$ = out; }
	   | OPCODE register COMMA offset { INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION)); out->opcode = $1; out->address1 = $2; out-> address2 = $4; $$ = out; }
	   | OPCODE offset COMMA register { INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION)); out->opcode = $1; out->address1 = $2; out-> address2 = $4; $$ = out; }
	   | OPCODE register COMMA register { INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION)); out->opcode = $1; out->address1 = $2; out-> address2 = $4; $$ = out; }
	   | OPCODE register COMMA LITERAL { INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION)); out->opcode = $1; out->address1 = $2; out-> address2 = $4; $$ = out; }
	   | OPCODE offset COMMA LITERAL { INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION)); out->opcode = $1; out->address1 = $2; out-> address2 = $4; $$ = out; }
	   | OPCODE offset { INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION)); out->opcode = $1; out->address1 = $2; $$ = out; }
	   | OPCODE register { INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION)); out->opcode = $1; out->address1 = $2; $$ = out; }
	   | OPCODE LITERAL { INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION)); out->opcode = $1; out->address1 = $2; $$ = out; }
       | OPCODE { INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION)); out->opcode = $1; $$ = out; }
       ;

offset: DOLLAR OP SP MINUS LITERAL CP { $$ = $5; }
	  | DOLLAR OP SP CP { $$ = 0; }
	  ;

register: RES { $$ = 0x0000; }
		| ACC { $$ = 0x0001; }
		| FLAGS { $$ = 0x0002; }
		;
%%


int main(int argc, char** argv) {
	extern FILE* yyin;
	if(argc > 1) {
    	if(!(yyin = fopen(argv[1], "r"))) {
      		perror(argv[1]);
      		return (1);
    	}
  	}
    yyparse();
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
}
