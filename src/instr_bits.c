#include <stdio.h>
#include <stdint.h>

#define H_0 "0000"
#define H_1 "0001"
#define H_2 "0010"
#define H_3 "0011"
#define H_4 "0100"
#define H_5 "0101"
#define H_6 "0110"
#define H_7 "0111"
#define H_8 "1000"
#define H_9 "1001"
#define H_A "1010"
#define H_B "1011"
#define H_C "1100"
#define H_D "1101"
#define H_E "1110"
#define H_F "1111"

#define B_0000 0x0
#define B_0001 0x1
#define B_0010 0x2
#define B_0011 0x3
#define B_0100 0x4
#define B_0101 0x5
#define B_0110 0x6
#define B_0111 0x7
#define B_1000 0x8
#define B_1001 0x9
#define B_1010 0xa
#define B_1011 0xb
#define B_1100 0xc
#define B_1101 0xd
#define B_1110 0xe
#define B_1111 0xf

#define WORD uint64_t
#define OPCODE_SIZE 4
#define OP_SIZE 14
#define WORD_SIZE (sizeof(WORD) * 8)

#define OPCODE_MASK 0xF000000000000000
#define OP1_MASK 	0x0FFFC00000000000
#define OP2_MASK	0x00003FFF00000000
#define OP3_MASK	0x00000000FFFC0000
#define OP4_MASK	0x000000000003FFF0
#define EXTRA_MASK	0x000000000000000F

#define OPCODE_SHIFT (WORD_SIZE - OPCODE_SIZE)
#define OP1_SHIFT (OPCODE_SHIFT - OP_SIZE)
#define OP2_SHIFT (OP1_SHIFT - OP_SIZE)
#define OP3_SHIFT (OP2_SHIFT - OP_SIZE)
#define OP4_SHIFT (OP3_SHIFT - OP_SIZE)

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

WORD constructInstr(WORD opcode, WORD op1, WORD op2, WORD op3, WORD op4) {
	WORD instr = 0x0;
	instr = SHIFTIN(opcode, OPCODE_SHIFT, instr);
	instr = SHIFTIN(op1, OP1_SHIFT, instr);
	instr = SHIFTIN(op2, OP2_SHIFT, instr);
	instr = SHIFTIN(op3, OP3_SHIFT, instr);
	instr = SHIFTIN(op4, OP4_SHIFT, instr);
	return instr;
}

void destructInstr(WORD instr) {
	printf("0x%016llx\n", SHIFTOUT(instr, OPCODE_SHIFT, OPCODE_MASK));
	printf("0x%016llx\n", SHIFTOUT(instr, OP1_SHIFT, OP1_MASK));
	printf("0x%016llx\n", SHIFTOUT(instr, OP2_SHIFT, OP2_MASK));
	printf("0x%016llx\n", SHIFTOUT(instr, OP3_SHIFT, OP3_MASK));
	printf("0x%016llx\n", SHIFTOUT(instr, OP4_SHIFT, OP4_MASK));
}

int main() {
	int instr_clear = 0x0;
	WORD instr = 0x0;
	WORD test = 0x1;
	WORD opcode = 0xc;
	WORD op1 = 0x4c6;
	WORD op2 = 0x23a;
	WORD op3 = 0x57b;
	WORD op4 = 0x16c;

	printf("OPCODE_MASK %016llx OPCODE_SHIFT %d\n", OPCODE_MASK, OPCODE_SHIFT);
	printf("OP1_MASK %016llx OP1_SHIFT %d OP1 %016llx SHIFTED %016llx\n", OP1_MASK, OP1_SHIFT, op1, (op1 << OP1_SHIFT));
	printf("OP2_MASK %016llx OP2_SHIFT %d OP2 %016llx SHIFTED %016llx\n", OP2_MASK, OP2_SHIFT, op2, (op2 << OP2_SHIFT));
	printf("OP3_MASK %016llx OP3_SHIFT %d OP3 %016llx SHIFTED %016llx\n", OP3_MASK, OP3_SHIFT, op3, (op3 << OP3_SHIFT));
	printf("OP4_MASK %016llx OP4_SHIFT %d OP4 %016llx SHIFTED %016llx\n", OP4_MASK, OP4_SHIFT, op4, (op4 << OP4_SHIFT));

	instr = constructInstr(opcode, op1, op2, op3, op4);
	printWord(instr);
	destructInstr(instr);

	return 0;
}
