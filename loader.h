#ifndef LOADER_H_INCLUDED
#define LOADER_H_INCLUDED
/**
This should load a chip8 file into the emulator.

For debugging purposes, convert chip8 files into a list of all opcodes used.
ASSUMES LITTLE ENDIAN MACHINE.
*/


int loadFile(char* title);

#endif