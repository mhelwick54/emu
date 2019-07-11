#include <stdio.h>
#include <stdint.h>

#define SEPARATOR "---------------------------------------------------------------------------\n"

#define BYTE 8
#define BYTES(a) (a * BYTE)

typedef enum regs {
	X0 = 0, X1, X2, X3,
	X4, X5, X6, X7,
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
	END
} InstrSet;

uint64_t L = -1;

/*
const int instructions[] = {
	LDR, X0, 2,
	LDR, X1, 5,
	ADD, X2, X0, X1,
	PSH, 1,
	STR, X2,
	LDR, X3, 1,
	SUB, X7, X2, X3,
	POP,
	PSH, 2,
	STR, X7,
	SAV,
	BR, 29,
	END,
	LDR, X5, 10,
	LDR, X6, 10,
	ADD, X5, X5, X6,
	EX
};
*/
uint64_t instructions[1024];

typedef struct Label {
	char 		l[17];
	uint64_t* 	addr;
} label;

label labels[128];
int label_count = 0;

uint64_t 	registers[NUM_REGS];

#define sp registers[SP]
#define pc registers[PC]

#define STACK_SIZE 32
uint64_t 	stack[STACK_SIZE];

#define push(arg) ((*(uint64_t*)(sp -= BYTE)) = arg)
#define pop() (sp += BYTE, (*(uint64_t*)(sp - BYTE)))

int 		executing = 1;

void exec();
void branch(uint64_t* new_addr);

//forward declarations
uint64_t 	match(char* s);
int 		interpret(char* file);

void 	printInstr(int instr);
void 	printStack();
void 	printRegs();

int main(int argc, char* argv[]) {
	printf("0x%016llx\n", (uint64_t)instructions);
	sp = (uint64_t)(stack + STACK_SIZE);
	char c;
	if(interpret(argv[1]) < 0) {
		return -1;
	}
	pc = (uint64_t)instructions;
	while(executing > 0) {
		printInstr(*(uint64_t*)pc);
		exec();
		printRegs();
		printStack();
		scanf("%c", &c);
		pc += BYTE;
	}

	return 0;
}

void exec() {
	switch(*(uint64_t*)pc) {
		case STR: {
			push(registers[*(uint64_t*)(pc += BYTES(1))]);
		}
			break;
		case LDR: {
			registers[*(uint64_t*)(pc + BYTES(1))] = *(uint64_t*)(pc + BYTES(2));
			pc += BYTES(2);
		}
			break;
		case ADD: {
			registers[*(uint64_t*)(pc + BYTES(1))] = registers[*(uint64_t*)(pc + BYTES(2))] + registers[*(uint64_t*)(pc + BYTES(3))];
			pc += BYTES(3);
		}
			break;
		case SUB: {
			registers[*(uint64_t*)(pc + BYTES(1))] = registers[*(uint64_t*)(pc + BYTES(2))] + registers[*(uint64_t*)(pc + BYTES(3))];
			pc += BYTES(3);
		}
			break;
		case PSH: push(*(uint64_t*)(pc += BYTES(1)));
			break;
		case POP: pop();
			break;
		case BR: branch(instructions + *(uint64_t*)(pc += BYTES(1)));
			break;
		case EX: branch((uint64_t*)(pop() + BYTES(2)));
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

#include "funcs.h"
