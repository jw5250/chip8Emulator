#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED
#include "dataType.h"

#define MEMSIZE 4096
//4096 bytes of memory (available addresses)
//Bytes are 8 bit
//Interpreter occupies first 512 bytes of memory
//Upper 256 for display refresh
//96 lower bytes for other usage (call stack, etc.)
byte* memory;

int initMemory();
byte readMemory(register int address);
void writeMemory(register word address, register byte value);
#endif

