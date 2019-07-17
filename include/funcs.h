#ifndef FUNCS_H
#define FUNCS_H

#include "mem_structure.h"

#define MAX_INSTR (TEXT_SIZE / WORD_BYTES)

uint64_t atoi(char* s) {
	uint64_t ret = 0;
	char* ptr = s;
	while(*ptr != '\0') {
		ret *= 10;
		ret += *ptr - '0';
		++ptr;
	}

	return ret;
}

int strcmp(char* s1, char* s2) {
	char* ptr1 = s1, *ptr2 = s2;
	while((*ptr1 != '\0') && (*ptr2 != '\0')) {
		if(*ptr1 != *ptr2) {
			return 0;
		}
		++ptr1;
		++ptr2;
	}

	if(((*ptr1 == '\0') && (*ptr2 != '\0')) || ((*ptr1 != '\0') && (*ptr2 == '\0'))) {
		return 0;
	} else {
		return 1;
	}
}

void strcpy(char* dest, char* src) {
	char* ptr = dest;
	while(*src != '\0') {
		*ptr++ = *src++;
	}
	*ptr = '\0';
}

uint64_t match(char* s) {
	if(strcmp(s, "STR")) {
		return STR;
	} else if(strcmp(s, "LDR")) {
		return LDR;
	} else if(strcmp(s, "ADD")) {
		return ADD;
	} else if(strcmp(s, "SUB")) {
		return SUB;
	} else if(strcmp(s, "PSH")) {
		return PSH;
	} else if(strcmp(s, "POP")) {
		return POP;
	} else if(strcmp(s, "BR")) {
		return BR;
	} else if(strcmp(s, "EX")) {
		return EX;
	} else if(strcmp(s, "SAV")) {
		return SAV;
	} else if(strcmp(s, "END")) {
		return END;
	} else if(strcmp(s, "X0")) {
		return X0;
	} else if(strcmp(s, "X1")) {
		return X1;
	} else if(strcmp(s, "X2")) {
		return X2;
	} else if(strcmp(s, "X3")) {
		return X3;
	} else if(strcmp(s, "X4")) {
		return X4;
	} else if(strcmp(s, "X5")) {
		return X5;
	} else if(strcmp(s, "X6")) {
		return X6;
	} else if(strcmp(s, "X7")) {
		return X7;
	} else {
		if(s[0] >= '0' && s[0] <= '9') {
			return atoi(s);
		} else {
			return L;
		}
	}
}

int find_jump(char* l) {
	int i = 0;
	for(i = 0; i < label_count; ++i) {
		if(strcmp(l, labels[i].l) == 1) {
			return i;
		}
	}
	return -1;
}

void trim_flag(char* s) {
	char* ptr = s;
	while(*ptr != '\n') {
		*ptr = *(ptr + 1);
		++ptr;
	}
	*(ptr-1) = '\0';
}

void handle_label(char* temp) {
	trim_flag(temp);
	strcpy(labels[label_count].l, temp);
	labels[label_count].addr = (WORD*)pc;
	label_count++;
}

WORD match_label(char* l) {
	int i = 0;
	for(i = 0; i < label_count; ++i) {
		if(strcmp(labels[i].l, l) == 1) {
			return (WORD)labels[i].addr;
		}
	}
	return TEXT_BOT;
}

void pushInstr(WORD type, FILE* f) {
	char op1[64+1];
	char op2[64+1];
	char op3[64+1];
	WORD instr = 0;
	switch(type) {
		case STR: {
			fscanf(f, "%s", op1);
			instr = constructInstr(type, match(op1), 0, 0);
		}
			break;
		case LDR: {
			fscanf(f, "%s", op1);
			fscanf(f, "%s", op2);
			instr = constructInstr(type, match(op1), match(op2), 0);
		}
			break;
		case ADD: {
			fscanf(f, "%s", op1);
			fscanf(f, "%s", op2);
			fscanf(f, "%s", op3);
			instr = constructInstr(type, match(op1), match(op2), match(op3));
		}
			break;
		case SUB: {
			fscanf(f, "%s", op1);
			fscanf(f, "%s", op2);
			fscanf(f, "%s", op3);
			instr = constructInstr(type, match(op1), match(op2), match(op3));
		}
			break;
		case PSH: {
			fscanf(f, "%s", op1);
			instr = constructInstr(type, match(op1), 0, 0);
		}
			break;
		case POP: { instr = constructInstr(type, 0, 0, 0); }
			break;
		case BR: {
			fscanf(f, "%s", op1);
			instr = constructInstr(type, 0, 0, match_label(op1));
		}
			break;
		case EX: { instr = constructInstr(type, 0, 0, 0); }
			break;
		case SAV: { instr = constructInstr(type, 0, 0, 0); }
			break;
		case END: { instr = constructInstr(type, 0, 0, 0); }
			break;
	}
	*(WORD*)pc = instr;
}

int scan_labels(char* file) {
	FILE* f = fopen(file, "r");
	if(!f) {
		printf("%s: file not found\n", file);
		return -1;
	}
	pc = TEXT;
	char temp[64+1];
	while(1) {
		fgets(temp, 64, f);
		if(temp[0] == '*') {
			break;
		}
		if(temp[0] == '-') {
			handle_label(temp);
		}
		pc += WORD_BYTES;
	}
	return 1;
}

int interpret(char* file) {
	if(scan_labels(file) < 0) {
		return -1;
	}
	pc = TEXT;
	FILE* f = fopen(file, "r");
	if(!f) {
		printf("%s: file not found\n", file);
		return -1;
	}
	char temp[64 + 1];
	int count = 0;
	uint64_t opcode, op1, op2, op3, op4;
	while(1) {
		fscanf(f, "%64s", temp);
		if(temp[0] == '*') {
			break;
		}
		if(count == MAX_INSTR) {
			break;
		}
		WORD type = match(temp);
		if(type != L) {
			pushInstr(type, f);
		}
		if(FUNCS) {
			printInstr(*(WORD*)pc);
		}
		pc += WORD_BYTES;
		++count;
	}
	return 1;
}

void printInstr(WORD instr) {
	uint16_t opcode = extractOpcode((WORD*)pc);
	uint16_t op1 = extractOp((WORD*)pc, 1);
	uint16_t op2 = extractOp((WORD*)pc, 2);
	uint16_t op3 = extractOp((WORD*)pc, 3);
	switch(opcode + INSTR_OFFSET) {
		case STR: printf("STR 0x%04x 0x%04x 0x%04x 0x%016llx\n", op1, op2, op3, (uint64_t)pc - (uint64_t)TEXT);
			break;
		case LDR: printf("LDR 0x%04x 0x%04x 0x%04x 0x%016llx\n", op1, op2, op3, (uint64_t)pc - (uint64_t)TEXT);
			break;
		case ADD: printf("ADD 0x%04x 0x%04x 0x%04x 0x%016llx\n", op1, op2, op3, (uint64_t)pc - (uint64_t)TEXT);
			break;
		case SUB: printf("SUB 0x%04x 0x%04x 0x%04x 0x%016llx\n", op1, op2, op3, (uint64_t)pc - (uint64_t)TEXT);
			break;
		case PSH: printf("PSH 0x%04x 0x%04x 0x%04x 0x%016llx\n", op1, op2, op3, (uint64_t)pc - (uint64_t)TEXT);
			break;
		case POP: printf("POP 0x%04x 0x%04x 0x%04x 0x%016llx\n", op1, op2, op3, (uint64_t)pc - (uint64_t)TEXT);
			break;
		case BR: printf("BR 0x%04x 0x%04x 0x%04x 0x%016llx\n", op1, op2, op3, (uint64_t)pc - (uint64_t)TEXT);
			break;
		case EX: printf("EX 0x%04x 0x%04x 0x%04x 0x%016llx\n", op1, op2, op3, (uint64_t)pc - (uint64_t)TEXT);
			break;
		case SAV: printf("SAV 0x%04x 0x%04x 0x%04x 0x%016llx\n", op1, op2, op3, (uint64_t)pc - (uint64_t)TEXT);
			break;
		case END: printf("END 0x%04x 0x%04x 0x%04x 0x%016llx\n", op1, op2, op3, (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X0: printf("X0 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X1: printf("X1 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X2: printf("X2 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X3: printf("X3 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X4: printf("X4 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X5: printf("X5 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X6: printf("X6 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X7: printf("X7 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X8: printf("X8 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X9: printf("X9 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X10: printf("X10 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X11: printf("X11 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X12: printf("X12 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X13: printf("X13 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X14: printf("X14 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		case X15: printf("X15 0x%016llx\n", (uint64_t)pc - (uint64_t)TEXT);
			break;
		default: printf("0x%016llx 0x%016llx\n", instr, (uint64_t)pc - (uint64_t)TEXT);
	}
}

void printStack() {
	int i = 0;
	int stackPtr = -1;
	WORD* ptr = (WORD*)sp - 1;
	while((STACK_TOP - (WORD)ptr) % (4 * WORD_BYTES) != 0) {
		ptr--;
	}
	printf("Stack\n");
	while(ptr != (WORD*)STACK_TOP) {
		printf("0x%016llx ", *ptr);
		if(ptr == (uint64_t*)sp) {
			stackPtr = i % 4;
		}
		++i;
		if(i % 4 == 0) {
			if(stackPtr != -1) {
				printf("\n");
				while(stackPtr > 0) {
					printf("                   ");
					--stackPtr;
				}
				printf("^\n");
			} else {
				printf("\n");
			}
			stackPtr = -1;
		}
		++ptr;
	}
	printf(SEPARATOR);
}

void printRegs() {
	printf("%sRegisters\n", SEPARATOR);
	uint64_t* ptr = registers;
	int i = 0;
	while(ptr != registers + NUM_REGS - 2) {
		printf("0x%016llx ", *ptr);
		++i;
		if(i % 4 == 0) {
			printf("\n");
		}
		++ptr;
	}
	printf("PC:      0x%064llx\n", *ptr);
	ptr++;
	printf("SP:      0x%064llx\n", *ptr);
	printf("%s", SEPARATOR);
}

#endif
