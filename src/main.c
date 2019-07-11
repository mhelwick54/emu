#include <stdio.h>
#include <stdint.h>

#define SEPARATOR "---------------------------------------------------------------------------\n"

#define BYTE 8

typedef enum set {
	STR = 9,//STR [REG]							REG -> stack
	LDR,	//LDR [REG] [VAL]					VAL -> REG
	ADD,	//ADD [DEST_REG] [REGA] [REGB]		DEST_REG = REGA + REGB
	SUB,	//SUB [DEST_REG] [REGA] [REGB]		DEST_REG = REGA - REGB
	PSH,	//PSH [VAL]							VAL -> stack
	POP,	//POP								stack ->
	BR,		//BR [PC_ADDR]						pc -> PC_ADDR
	EX,		//EX								pc -> stack
	SAV,	//SAV								pc -> stack
	END
} InstrSet;

typedef enum regs {
	X0 = 0, X1, X2, X3,
	X4, X5, X6, X7,
	NUM_REGS
} Registers;

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

#define STACK_SIZE 32
uint64_t 	stack[STACK_SIZE];
uint64_t* 	sp = stack + STACK_SIZE;

uint64_t 	registers[NUM_REGS];

#define push(arg) ((*(--sp)) = arg)
#define pop() (*(sp++))

uint64_t* 	pc;
int 		executing = 1;

void exec();
void branch(uint64_t* new_addr);

//forward declarations
uint64_t 	match(char* s);
void 		interpret(uint64_t* instr, char* file);

void 	printInstr(int instr);
void 	printStack();
void 	printRegs();

int main(int argc, char* argv[]) {
	char c;
	interpret(instructions, argv[1]);
	if(instructions == NULL) {
		return -1;
	}
	pc = instructions;
	while(executing > 0) {
		printInstr(*pc);
		exec();
		printRegs();
		printStack();
		scanf("%c", &c);
		++pc;
	}

	return 0;
}

void exec() {
	switch(*pc) {
		case STR: {
			push(registers[*(++pc)]);
		}
			break;
		case LDR: {
			registers[*(pc + 1)] = *(pc + 2);
			pc += 2;
		}
			break;
		case ADD: {
			registers[*(pc + 1)] = registers[*(pc + 2)] + registers[*(pc + 3)];
			pc += 3;
		}
			break;
		case SUB: {
			registers[*(pc + 1)] = registers[*(pc + 2)] - registers[*(pc + 3)];
			pc += 3;
		}
			break;
		case PSH: push(*(++pc));
			break;
		case POP: pop();
			break;
		case BR: branch(instructions + *(++pc));
			break;
		case EX: branch((uint64_t*)(pop() + (2 * BYTE)));
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
	pc = new_addr;
}

#include "funcs.h"
