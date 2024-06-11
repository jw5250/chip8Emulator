#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED
#include "dataType.h"


//0x0200 is 512 in decimal.
#define STARTADDR 512
typedef struct{
	//Registers
	byte reg[16];
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
void cpuLoop(int mostRecentKey);

#endif
