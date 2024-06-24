#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED
#include "dataType.h"
//Define the chip8 CPU.

//0x0200 is 512 in decimal.
#define STARTADDR 512
typedef struct{
	//Registers
	byte reg[16];
	//Index register. Used for memory access
	word i;
	//stack pointer
	byte sp;
	//Program counter. needs to store the 12 bit address code.
	word pc;
} chip8reg;

//Move into cpu.c after finishing.
chip8reg cpu;

//Start up the cpu.
void initCpu();

//Do the fetch decode execute cycle of the cpu.
void cpuLoop(int mostRecentKey);

#endif
