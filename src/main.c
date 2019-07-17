#include <stdio.h>
#include <stdint.h>

#include "compiler.h"
#include "mem_structure.h"
#include "funcs.h"
#include "globals.h"

int main(int argc, char* argv[]) {
	setFlags(argc, argv);
	if(MEM) {
		printMemStruct();
		printf("Text: "HEX, (uint64_t)TEXT);
	}
	sp = STACK_TOP;
	char c;
	if(compile(argv[1]) < 0) {
		return -1;
	}
	pc = (WORD)TEXT;
	while(executing > 0) {
		printInstr(*(uint64_t*)pc);
		exec();
		if(REGS) {
			printRegs();
		}
		if(STRACE) {
			printStack();
		}
		if(BREAK) {
			scanf("%c", &c);
		}
		pc += BYTE;
	}

	return 0;
}

void exec() {
	uint16_t opcode = extractOpcode((WORD*)pc);
	uint16_t op1 = extractOp((WORD*)pc, 1);
	uint16_t op2 = extractOp((WORD*)pc, 2);
	uint16_t op3 = extractOp((WORD*)pc, 3);
	printf("0x%04x 0x%04x 0x%04x 0x%04x\n", opcode, op1, op2, op3);
	switch(opcode + INSTR_OFFSET) {
		case STR: {
			push(op1);
		}
			break;
		case LDR: {
			registers[op1] = op2;
		}
			break;
		case ADD: {
			registers[op1] = registers[op2] + registers[op3];
		}
			break;
		case SUB: {
			registers[op1] = registers[op2] + registers[op3];
		}
			break;
		case PSH: push(op1);
			break;
		case POP: pop();
			break;
		case BR: branch(getAddr((WORD*)pc));
			break;
		case EX: {
			branch(pop() + WORD_BYTES);
		}
			break;
		case SAV: push((uint64_t)pc);
			break;
		case END: {
			executing = -1;
		}
			break;
	}
}

void branch(WORD new_addr) {
	pc = new_addr;
}

void setFlags(int argc, char* argv[]) {
	if(argc == 3) {
		switch(argv[2][1]) {
			case 's': {STRACE = 1;} break;
			case 'b': {BREAK = 1;} break;
			case 'r': {REGS = 1;} break;
			case 'f': {FUNCS = 1;} break;
			case 'm': {MEM = 1;} break;
		}
	} else if(argc == 4) {
		switch(argv[2][1]) {
			case 's': {STRACE = 1;} break;
			case 'b': {BREAK = 1;} break;
			case 'r': {REGS = 1;} break;
			case 'f': {FUNCS = 1;} break;
			case 'm': {MEM = 1;} break;
		}
		switch(argv[3][1]) {
			case 's': {STRACE = 1;} break;
			case 'b': {BREAK = 1;} break;
			case 'r': {REGS = 1;} break;
			case 'f': {FUNCS = 1;} break;
			case 'm': {MEM = 1;} break;
		}
	} else if(argc == 5) {
		   switch(argv[2][1]) {
			   case 's': {STRACE = 1;} break;
			   case 'b': {BREAK = 1;} break;
			   case 'r': {REGS = 1;} break;
			   case 'f': {FUNCS = 1;} break;
			   case 'm': {MEM = 1;} break;
		   }
		   switch(argv[3][1]) {
			   case 's': {STRACE = 1;} break;
			   case 'b': {BREAK = 1;} break;
			   case 'r': {REGS = 1;} break;
			   case 'f': {FUNCS = 1;} break;
			   case 'm': {MEM = 1;} break;
		   }
		   switch(argv[4][1]) {
			   case 's': {STRACE = 1;} break;
			   case 'b': {BREAK = 1;} break;
			   case 'r': {REGS = 1;} break;
			   case 'f': {FUNCS = 1;} break;
			   case 'm': {MEM = 1;} break;
		   }
	} else if(argc == 6) {
		   switch(argv[2][1]) {
			   case 's': {STRACE = 1;} break;
			   case 'b': {BREAK = 1;} break;
			   case 'r': {REGS = 1;} break;
			   case 'f': {FUNCS = 1;} break;
			   case 'm': {MEM = 1;} break;
		   }
		   switch(argv[3][1]) {
			   case 's': {STRACE = 1;} break;
			   case 'b': {BREAK = 1;} break;
			   case 'r': {REGS = 1;} break;
			   case 'f': {FUNCS = 1;} break;
			   case 'm': {MEM = 1;} break;
		   }
		   switch(argv[4][1]) {
			   case 's': {STRACE = 1;} break;
			   case 'b': {BREAK = 1;} break;
			   case 'r': {REGS = 1;} break;
			   case 'f': {FUNCS = 1;} break;
			   case 'm': {MEM = 1;} break;
		   }
		   switch(argv[5][1]) {
			   case 's': {STRACE = 1;} break;
			   case 'b': {BREAK = 1;} break;
			   case 'r': {REGS = 1;} break;
			   case 'f': {FUNCS = 1;} break;
			   case 'm': {MEM = 1;} break;
		   }
	} else if(argc == 7) {
		STRACE = 1;
		BREAK = 1;
		REGS = 1;
		FUNCS = 1;
		MEM = 1;
	}
}
