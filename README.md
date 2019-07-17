# EMU
#### A C-based emulator of ASM instruction set.

Building: run `make clean && make` from root directory

Usage: `./bin/emu [asm_instruction_file] [option flags]`

Option flags:
* `-b`: halts execution between instructions and waits for user to hit <Enter>
* `-f`: prints a list of parsed instructions, along with their relative addresses
* `-r`: prints the registers' contents
* `-s`: prints a stack trace
* `-m`: prints a diagram of the memory sections

### Instruction Set
`STR [REG]` - pushes the value in `[REG]` to the stack

`LDR [REG] [VAL]` - places `[VAL]` in `[REG]`

`ADD [DEST_REG] [REGA] [REGB]` - calculates `[REGA]` + `[REGB]` and places sum in `[DEST_REG]`

`SUB [DEST_REG] [REGA] [REGB]` - calculates `[REGA]` - `[REGB]` and places sum in `[DEST_REG]`

`PSH [VAL]` - pushes `[VAL]` to the stack

`POP` - pops the last value from the stack (increment SP)

`BR [PC_ADDR]` - changes PC to `[PC_ADDR]`

`EX` - pops the last value from the stack (from previous save) and sets PC to popped value

`SAV` - pushes the current PC to the stack

`END` - exits current program

### Architecture
#### Registers
X0 - X15: general purpose registers

PC: program counter, points to current instruction

SP: stack pointer, points to minimum stack byte in current use

#### Memory
Address are relative. Base will be the base address of the `memory` array when program is loaded.

|	Section				|	Hex Address	|	Binary Address		|
|---------------|-------------|-------------------|
|	Stack					|	`0xFFF`			|	`1111 1111 1111`	|
|	Heap					|	`0x0FF`			|	`0000 1111 1111`	|
|	Data					|	`0x07F`			|	`0000 0111 1111`	|
|	Text					|	`0x03F`			|	`000 0011 1111`		|
|	Base					|	`0x000`			|	`0000 0000 0000`	|
