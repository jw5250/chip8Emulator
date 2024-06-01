#ifndef MEMORY_C_INCLUDED
#define MEMORY_C_INCLUDED
#include "memory.h"
#include <stdlib.h>

//memory array will be moved into the memory.c file once I finish testing all of the instructions.
byte* memory;

int initMemory(){
	memory = (byte*)malloc(MEMSIZE*sizeof(byte));
	//As part of memory initialization:
		//Store font data!
	return memory != NULL;
}
byte readMemory(register int address){
	return memory[address];
}
void writeMemory(register word address, register byte value){
	memory[address.WORD] = value;
}
#endif
