%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdint.h>
	#include "hmap.h"
	FILE* output_file;
	uint16_t instr_length;
	extern HMAP* symtab;
%}

%code requires {
	typedef struct {
		uint8_t opcode;
		uint16_t address1;
		uint16_t address2;
	} INSTRUCTION;
}

%union {
	char* str_val;
    uint32_t value;
    uint16_t address;
    uint8_t opcode;
	INSTRUCTION* instruction;
}

%token <str_val> LITERAL
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
       | instrlist instr EOL {
	   		if($2->opcode) {
				fwrite(&$2->opcode, sizeof(uint8_t), 1, output_file);
			}
			if($2->address1) {
				fwrite(&$2->address1, sizeof(uint16_t), 1, output_file);
			}
			if($2->address2) {
				fwrite(&$2->address2, sizeof(uint16_t), 1, output_file);
			}
		}
       ;

instr: OPCODE offset COMMA offset {
	 		INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION));
			out->opcode = $1;
			out->address1 = $2;
			out-> address2 = $4;
			$$ = out;
			instr_length += 5;
		}
	   | OPCODE register COMMA offset {
	   		INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION));
	   		out->opcode = $1;
			out->address1 = $2;
			out-> address2 = $4;
			$$ = out;
			instr_length += 5;
		}
	   | OPCODE offset COMMA register {
	   		INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION));
			out->opcode = $1;
			out->address1 = $2;
			out-> address2 = $4;
			$$ = out;
			instr_length += 5;
		}
	   | OPCODE register COMMA register {
	   		INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION));
	   		out->opcode = $1;
	   		out->address1 = $2;
	   		out-> address2 = $4;
	   		$$ = out;
			instr_length += 5;
	   }
	   | OPCODE register COMMA LITERAL {
	   		INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION));
			out->opcode = $1;
			out->address1 = $2;
			out->address2 = atoi(hmap_get(symtab, $4));
			$$ = out;
			instr_length += 5;
		}
	   | OPCODE offset COMMA LITERAL {
	   		INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION));
			out->opcode = $1;
			out->address1 = $2;
			out->address2 = atoi(hmap_get(symtab, $4));
			$$ = out;
			instr_length += 5;
		}
	   | OPCODE offset {
	   		INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION));
			out->opcode = $1;
			out->address1 = $2;
			$$ = out;
			instr_length += 3;
		}
	   | OPCODE register {
	   		INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION));
			out->opcode = $1;
			out->address1 = $2;
			$$ = out;
			instr_length += 3;
		}
	   | OPCODE LITERAL {
	   		INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION));
			out->opcode = $1;
			out->address1 = atoi(hmap_get(symtab, $2));
			$$ = out;
			instr_length += 3;
		}
       | OPCODE {
	   		INSTRUCTION* out = calloc(1, sizeof(INSTRUCTION));
			out->opcode = $1;
			$$ = out;
			instr_length += 1;
		}
       ;

offset: DOLLAR OP SP MINUS LITERAL CP { $$ = 0x8200 + atoi($5); }
	  | DOLLAR OP SP CP { $$ = 0; }
	  ;

register: RES { $$ = 0x8101; }
		| ACC { $$ = 0x8102; }
		| FLAGS { $$ = 0x8103; }
		;
%%


int main(int argc, char** argv) {
	extern FILE* yyin;
	symtab = hmap_init();
	if(argc > 1) {
    	if(!(yyin = fopen(argv[1], "r"))) {
      		perror(argv[1]);
      		return (1);
    	}
    	if(!(output_file = fopen("out.bin", "w"))) {
      		perror(argv[1]);
      		return (1);
    	}
  	}
	uint32_t magic = 0xFEFAFAFC;
	uint16_t data_offset = 0x0000;

	// write magic bytes
	fwrite(&magic, sizeof(uint32_t), 1, output_file);

	// write placeholder for data offset
	long do_p = ftell(output_file);
	fwrite(&data_offset, sizeof(uint16_t), 1, output_file);
    yyparse();

	// write the data offset
	fseek(output_file, do_p, SEEK_SET);
	printf("%d\n", instr_length);
	fwrite(&instr_length, sizeof(uint16_t), 1, output_file);

	// write the data length
	fseek(output_file, 0, SEEK_END);
	data_offset += symtab->inserted * 4;
	fwrite(&data_offset, sizeof(uint16_t), 1, output_file);

	// save beginning of data segment and fill with zeroes
	long dp = ftell(output_file);
	uint32_t zero = 0;
	for(size_t i = 0; i < symtab->inserted; i++) {
		fwrite(&zero, sizeof(uint32_t), 1, output_file);
	}

	for(size_t i = 0; i < symtab->size; i++) {
		ENTRY* entry = symtab->entries[i];
		if(entry != NULL) {
			long address = atoi(entry->value) - 0xE041;
			uint32_t key = atoi(entry->key);
			fseek(output_file, dp + address, SEEK_SET);
			fwrite(&key, sizeof(uint32_t), 1, output_file);
		}
	}

    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
}
