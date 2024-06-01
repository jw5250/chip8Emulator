#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED
#include "dataType.h"
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

chip8reg cpu;

void initCpu();
void cpuLoop();

inline void bitWiseOperations(int reg1, int reg2, int id);
inline void arithmeticOperations(int reg1, int reg2, int id);
inline void shiftOperations(int reg1, int id);


inline void or(int reg1, int reg2);
inline void and(int reg1, int reg2);
inline void xor(int reg1, int reg2);


inline void addReg(int reg1, byte b1);
inline void subCarry(int reg1, int reg2);
inline void addCarry(int reg1, int reg2);

inline void lsh(int reg1);
inline void rsh(int reg1);

inline void jmp(word nextInstr);
inline void jmpOff(word w1);

inline void setIndex(word w1);
inline void addIndex(int reg1);

inline void beq(byte b1, byte b2);
inline void bne(byte b1, byte b2);

inline void load(int reg1, byte b1);

inline void randomFunction(int reg1, byte b1);



//Currently untested.
inline void bcdEncode(int reg1);
inline void draw(int regX, int regY, int nBytes);

#endif
