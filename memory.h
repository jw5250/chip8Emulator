#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED
#include "dataType.h"
#define FONT_HEIGHT 5
/*
TODO:
    Write in ROM that automatically inserts font data for each digit in hex.
*/

//4096 bytes of memory (available addresses)
//Bytes are 8 bit
//Interpreter occupies first 512 bytes of memory
    //Font data (0-F sprites) usually stored in start of memory.
//Upper 256 for display refresh
//96 lower bytes for other usage (call stack, etc.)


int initMemory();
int memoryExists();
byte readMemory(register int address);
void writeMemory(register word address, register byte value);
#endif

