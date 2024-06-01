#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED
#include "dataType.h"

//Current problems:
	//Supposedly a big-endian machine, but use of WORD doesn't make the CPU loop independent of endianness. Fix!

//0x0200 is 512 in decimal.
#define STARTADDR 0x0200
typedef struct{
	//Registers
	byte reg[0x10];
	//Index register. Used for memory access
	word i;
	//stack pointer
	byte sp;
	//sound timer
	byte st;
	//delay timer
	byte dt;
	//Program counter. needs to store the 12 bit address code.
	word pc;
} chip8reg;

//Move into cpu.c after finishing.
chip8reg cpu;

void initCpu();
void cpuLoop();

#endif
