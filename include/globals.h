#ifndef GLOBALS_H
#define GLOBALS_H

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
void branch(WORD new_addr);
void setFlags(int argc, char* argv[]);

#endif //GLOBALS_H
