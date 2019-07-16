#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define BYTE 8

typedef enum set {
	STR = 0,								//STR [REG]							REG -> stack
	LDR,									//LDR [REG] [VAL]					VAL -> REG
	ADD,									//ADD [DEST_REG] [REGA] [REGB]		DEST_REG = REGA + REGB
	SUB,									//SUB [DEST_REG] [REGA] [REGB]		DEST_REG = REGA - REGB
	PSH,									//PSH [VAL]							VAL -> stack
	POP,									//POP								stack ->
	BR,										//BR [PC_ADDR]						pc -> PC_ADDR
	EX,										//EX								pc -> stack
	SAV,									//SAV								pc -> stack
	END,
	L,									//function label
	INSTR_COUNT
} InstrSet;

#define STACK_SIZE 1024
#define WORD_SIZE sizeof(uint64_t)
#define STACK_BYTES STACK_SIZE * WORD_SIZE
#define ADDR_BITS log2(STACK_BYTES)
#define INSTR_BITS ceil((double)log2(INSTR_COUNT))

int main() {
	printf("word: %ld bytes %ld bits\n", WORD_SIZE, WORD_SIZE * BYTE);
	printf("stack: %ld bytes %ld bits\n", STACK_BYTES, STACK_BYTES * BYTE);
	printf("address: %f bits\n", ADDR_BITS);

	printf("instruction: %f bits\n", INSTR_BITS);

	printf("needed: %f bits\n", (ADDR_BITS * 4) + INSTR_BITS);

	return 0;
}
