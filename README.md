# EMU
#### A C-based emulator of ASM instruction set.

Building: run `make clean && make` from root directory

Usage: `./bin/emu [asm_instruction_file] [option flags]`

Option flags:
* `-b`: halts execution between instructions and waits for user to hit <Enter>
* `-f`: prints a list of parsed instructions, along with their relative addresses
* `-r`: prints the registers' contents
* `-s`: prints a stack trace
