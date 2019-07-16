#ifndef FUNCS_H
#define FUNCS_H

#define MAX_INSTR 1024

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
	while(*ptr != '\0') {
		*ptr = *(ptr + 1);
		++ptr;
	}
}

void handle_label(char* temp, int count) {
	if(temp[0] == '-') {
		trim_flag(temp);
		int label_num = find_jump(temp);
		labels[label_num].addr = (uint64_t*)((uint64_t*)pc - instructions);
	} else {
		instructions[count] = L - 1;
		strcpy(labels[label_count].l, temp);
		label_count++;
	}
}

int interpret(char* file) {
	pc = (uint64_t)instructions;
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
		instr_type = match(temp);
		instructions[count] = match(temp);
		if(instructions[count] == L) {
			handle_label(temp, count);
		}
		if(FUNCS) {
			printInstr(instructions[count]);
		}
		pc += BYTE;
		++count;
	}
	count--;
	label_count--;
	while(count >= 0) {
		if(instructions[count] == L - 1) {
			instructions[count] = (uint64_t)labels[label_count].addr;
			label_count--;
		}
		count--;
	}
	return 1;
}

void printInstr(int instr) {
	switch(instr) {
		case STR: printf("STR 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case LDR: printf("LDR 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case ADD: printf("ADD 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case SUB: printf("SUB 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case PSH: printf("PSH 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case POP: printf("POP 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case BR: printf("BR 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case EX: printf("EX 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case SAV: printf("SAV 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case END: printf("END 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X0: printf("X0 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X1: printf("X1 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X2: printf("X2 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X3: printf("X3 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X4: printf("X4 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X5: printf("X5 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X6: printf("X6 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X7: printf("X7 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X8: printf("X8 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X9: printf("X9 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X10: printf("X10 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X11: printf("X11 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X12: printf("X12 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X13: printf("X13 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X14: printf("X14 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		case X15: printf("X15 0x%016llx\n", (uint64_t)pc - (uint64_t)instructions);
			break;
		default: printf("%d 0x%016llx\n", instr, (uint64_t)pc - (uint64_t)instructions);
	}
}

void printStack() {
	int i = 0;
	int stackPtr = -1;
	uint64_t* ptr = stack;
	printf("Stack\n");
	while(ptr != stack + STACK_SIZE) {
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
