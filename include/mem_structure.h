#ifndef MEM_STRUCT_H
#define MEM_STRUCT_H

#include <stdint.h>

#include "bit_defs.h"

#define HEX "0x%016llx\n"

/****************************/	//0x7fff = 0b0111 1111 1111 1111
/*	Stack					*/
/*							*/
/*							*/
/*							*/
/****************************/
/*							*/
/*							*/
/*							*/
/*							*/
/****************************/	//0x0800 = 0b0000 1000 0000 0000
/*	Heap					*/
/*							*/
/*							*/
/*							*/
/****************************/	//0x0400 = 0b0000 0100 0000 0000
/*	Data					*/
/*							*/
/****************************/	//0x0200 = 0b0000 0010 0000 0000
/*	Text					*/
/*							*/
/****************************/	//0x0000 = 0b0000 0000 0000 0000

/*******************************************************************************
 * 64 opcode 60 | 59   op1   44 | 43   op2   28 | 27   op3   12 | 11  extra  0 |
 ******************************************************************************/

#define WORD uint64_t
#define WORD_BYTES sizeof(uint64_t)
#define WORD_SIZE sizeof(uint64_t) * 8

#define MEM_SIZE 4096
#define MEM_BYTES MEM_SIZE * WORD_BYTES
WORD memory[MEM_SIZE];		//262,144 bits = 2^18 bits

#define MEM_MOD MEM_SIZE * WORD_BYTES

#define TEXT_WORDS 64
#define DATA_WORDS 64
#define HEAP_WORDS 128
#define STACK_WORDS (MEM_SIZE - (TEXT_WORDS + DATA_WORDS + HEAP_WORDS))

#define TEXT_SIZE (TEXT_WORDS * WORD_BYTES)
#define DATA_SIZE (DATA_WORDS * WORD_BYTES)
#define HEAP_SIZE (HEAP_WORDS * WORD_BYTES)
#define STACK_SIZE (STACK_WORDS * WORD_BYTES)

#define TEXT_BOT ((WORD)memory)
#define TEXT_TOP (TEXT_BOT + TEXT_SIZE - WORD_BYTES)

#define DATA_BOT (TEXT_TOP + WORD_BYTES)
#define DATA_TOP (DATA_BOT + DATA_SIZE - WORD_BYTES)

#define HEAP_BOT (DATA_TOP + WORD_BYTES)
#define HEAP_TOP (HEAP_BOT + HEAP_SIZE - WORD_BYTES)

#define STACK_BOT (HEAP_TOP + WORD_BYTES)
#define STACK_TOP (STACK_BOT + STACK_SIZE)

#define TEXT TEXT_BOT

#define OPCODE_MASK 0xF000000000000000
#define OP1_MASK 	0x0FFFF00000000000
#define OP2_MASK	0x00000FFFF0000000
#define OP3_MASK	0x000000000FFFF000
#define EXTRA_MASK	0x0000000000000FFF
#define ADDR_MASK	0x0FFFFFFFFFFFFFFF

#define OPCODE_SIZE 4
#define OP_SIZE 16

#define OPCODE_SHIFT (WORD_SIZE - OPCODE_SIZE)
#define OP1_SHIFT (OPCODE_SHIFT - OP_SIZE)
#define OP2_SHIFT (OP1_SHIFT - OP_SIZE)
#define OP3_SHIFT (OP2_SHIFT - OP_SIZE)

#define INSTR_OFFSET 19

#define PRINT(a) printf("%s", a)

void printWord(WORD word) {
	int i = 0;
	for(i = 15; i >= 0; --i) {
		switch((word >> (i * 4)) & 0xF) {
			case B_0000: PRINT(H_0);
				break;
			case B_0001: PRINT(H_1);
				break;
			case B_0010: PRINT(H_2);
				break;
			case B_0011: PRINT(H_3);
				break;
			case B_0100: PRINT(H_4);
				break;
			case B_0101: PRINT(H_5);
				break;
			case B_0110: PRINT(H_6);
				break;
			case B_0111: PRINT(H_7);
				break;
			case B_1000: PRINT(H_8);
				break;
			case B_1001: PRINT(H_9);
				break;
			case B_1010: PRINT(H_A);
				break;
			case B_1011: PRINT(H_B);
				break;
			case B_1100: PRINT(H_C);
				break;
			case B_1101: PRINT(H_D);
				break;
			case B_1110: PRINT(H_E);
				break;
			case B_1111: PRINT(H_F);
				break;
			default: printf("err");
		}
	}
	printf("\n");
}

#define SHIFTIN(a,b,c) ((a << b) ^ c)
#define SHIFTOUT(a,b,c) ((a & c) >> b)

WORD constructInstr(WORD opcode, WORD op1, WORD op2, WORD op3) {
	WORD instr = 0x0;
	instr = SHIFTIN((opcode - INSTR_OFFSET), OPCODE_SHIFT, instr);
	instr = SHIFTIN(op1, OP1_SHIFT, instr);
	instr = SHIFTIN(op2, OP2_SHIFT, instr);
	instr = SHIFTIN(op3, OP3_SHIFT, instr);
	return instr;
}

void destructInstr(WORD instr) {
	printf("0x%04llx\n", SHIFTOUT(instr, OPCODE_SHIFT, OPCODE_MASK));
	printf("0x%016llx\n", SHIFTOUT(instr, OP1_SHIFT, OP1_MASK));
	printf("0x%016llx\n", SHIFTOUT(instr, OP2_SHIFT, OP2_MASK));
	printf("0x%016llx\n", SHIFTOUT(instr, OP3_SHIFT, OP3_MASK));
}

int extractOpcode(WORD* instr) {
	return SHIFTOUT(*instr, OPCODE_SHIFT, OPCODE_MASK);
}

int extractOp(WORD* instr, int op) {
	switch(op) {
		case 1: return SHIFTOUT(*instr, OP1_SHIFT, OP1_MASK);
		case 2: return SHIFTOUT(*instr, OP2_SHIFT, OP2_MASK);
		case 3: return SHIFTOUT(*instr, OP3_SHIFT, OP3_MASK);
	}
	return 0;
}

WORD getAddr(WORD* instr) {
	return SHIFTOUT(*instr, OP3_SHIFT, ADDR_MASK);
}

void printMemStruct() {
	printf("Stack: "HEX"virtual: "HEX"decimal: %lld\n", STACK_TOP, STACK_TOP - (WORD)memory, STACK_TOP - (WORD)memory);
	printf(".\n.\n.\n.\n");
	printf(HEX"virtual: "HEX"decimal: %lld\n", STACK_BOT, STACK_BOT - (WORD)memory, STACK_BOT - (WORD)memory);
	printf("Heap: "HEX"virtual: "HEX"decimal: %lld\n", HEAP_TOP, HEAP_TOP - (WORD)memory, HEAP_TOP - (WORD)memory);
	printf(".\n.\n.\n.\n");
	printf(HEX"virtual: "HEX"decimal: %lld\n", HEAP_BOT, HEAP_BOT - (WORD)memory, HEAP_BOT - (WORD)memory);
	printf("Data: "HEX"virtual: "HEX"decimal: %lld\n", DATA_TOP, DATA_TOP - (WORD)memory, DATA_TOP - (WORD)memory);
	printf(".\n.\n");
	printf(HEX"virtual: "HEX"decimal: %lld\n", DATA_BOT, DATA_BOT - (WORD)memory, DATA_BOT - (WORD)memory);
	printf("Text: "HEX"virtual: "HEX"decimal: %lld\n", TEXT_TOP, TEXT_TOP - (WORD)memory, TEXT_TOP - (WORD)memory);
	printf(".\n.\n");
	printf(HEX"virtual: "HEX"decimal: %lld\n", TEXT_BOT, TEXT_BOT - (WORD)memory, TEXT_BOT - (WORD)memory);
}

#endif
