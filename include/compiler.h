#ifndef COMPILER_H
#define COMPILER_H

#include "funcs.h"
#include "globals.h"

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

int compile(char* file) {
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
		WORD type = match(temp);
		if(type != L) {
			if(count == MAX_INSTR - 1) {
				type = END;
			}
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

#endif //COMPILER_H
