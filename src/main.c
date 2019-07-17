#include <stdio.h>
#include <stdint.h>

#include "mem_structure.h"

#define SEPARATOR "---------------------------------------------------------------------------\n"

#define BYTE 8
#define BYTES(a) (a * BYTE)

int STRACE = 0;
int BREAK = 0;
int REGS = 0;
int FUNCS = 0;
int MEM = 0;

typedef enum regs {
	X0 = 0, X1, X2, X3,
	X4, X5, X6, X7,
	X8, X9, X10, X11,
	X12, X13, X14, X15,
	PC, SP,
	NUM_REGS
} Registers;

typedef enum set {
	STR = NUM_REGS + 1,						//STR [REG]							REG -> stack
	LDR,									//LDR [REG] [VAL]					VAL -> REG
	ADD,									//ADD [DEST_REG] [REGA] [REGB]		DEST_REG = REGA + REGB
	SUB,									//SUB [DEST_REG] [REGA] [REGB]		DEST_REG = REGA - REGB
	PSH,									//PSH [VAL]							VAL -> stack
	POP,									//POP								stack ->
	BR,										//BR [PC_ADDR]						pc -> PC_ADDR
	EX,										//EX								pc -> stack
	SAV,									//SAV								pc -> stack
	END,
	L = -1									//function label
} InstrSet;

typedef struct Label {
	char 		l[17];
	uint64_t* 	addr;
} label;

label labels[512];
int label_count = 0;

uint64_t 	registers[NUM_REGS];

#define sp registers[SP]
#define pc registers[PC]

#define push(arg) ((*(uint64_t*)(sp -= WORD_BYTES)) = arg)
#define pop() (sp += WORD_BYTES, (*(uint64_t*)(sp - WORD_BYTES)))

int 		executing = 1;

void exec();
void branch(uint64_t* new_addr);

//forward declarations
uint64_t 	match(char* s);
int 		interpret(char* file);

void 	printInstr(WORD instr);
void 	printStack();
void 	printRegs();

void setFlags(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	setFlags(argc, argv);
	if(MEM) {
		printMemStruct();
		printf("Text: "HEX, (uint64_t)TEXT);
	}
	sp = STACK_TOP;
	char c;
	if(interpret(argv[1]) < 0) {
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
		case BR: branch((WORD*)(TEXT + op1));
			break;
		case EX: branch((WORD*)pop());
			break;
		case SAV: push((uint64_t)pc);
			break;
		case END: {
			executing = -1;
		}
			break;
	}
}

void branch(uint64_t* new_addr) {
	pc = (uint64_t)new_addr;
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

#include "funcs.h"
