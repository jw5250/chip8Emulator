#ifndef MEMORY_C_INCLUDED
#define MEMORY_C_INCLUDED
#include "memory.h"
#include <stdlib.h>
int initMemory(){
	memory = (byte*)malloc(MEMSIZE*sizeof(byte));
	return memory != NULL;
}
byte readMemory(register int address){
	return memory[address];
}
void writeMemory(register word address, register byte value){
	memory[address.WORD] = value;
}
#endif
