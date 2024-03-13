#ifndef CPU_C_INCLUDED
#define CPU_C_INCLUDED
#define WORD_LEN 2
#include "cpu.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#define STARTADDR 0x0200
#define STACKSIZE 48
#define STACKADDR 0x0
//I could rewrite the giant if else statement into a jump table. Is it worth it?

//Writing the actual "hardware":
	//Store the actual interpreter in the memory
	//Needed instructions:
	//Fix add carry.
	//draw functions
	//key operations
	//delay and timer functions

void initCpu(){
	cpu.pc.WORD = STARTADDR;
	//Stack size is 96 bytes long. Up to 48 levels of function calls
	//According to my sources its actually half that but I want to try this first.
	cpu.sp = 0;
}

void cpuLoop(){
	//Issue: how do we know when the program ends?
		//Assume it loops infinitely at the end. Hopefully.
	word currentInstruction;
	//Big endian storage means most signficant byte is stored at word address.
	currentInstruction.BYTE.UPPER = readMemory(cpu.pc.WORD);
	currentInstruction.BYTE.LOWER = readMemory(cpu.pc.WORD+1);
	cpu.pc.WORD += WORD_LEN;//Increment by one word.
	
	//Get opcode.
	unsigned short opcode = currentInstruction.WORD & 0xF000;
	opcode >>= 12;//three nibbles
	//Decode
	
	unsigned short firstNibble = (currentInstruction.WORD & 0x0F00);
	unsigned short secondNibble = (currentInstruction.WORD & 0x00F0);
	unsigned short thirdNibble = (currentInstruction.WORD & 0x000F);
	if(opcode == 0){
		//call machine code routine
		//clear screen
		//return from a subroutine (set to address on stack)
		if( ((firstNibble | secondNibble) | thirdNibble) == 0x00EE){
			word stackLoc;
			printf("Returning\n");	
			cpu.sp -=1;
			cpu.pc.BYTE.LOWER = readMemory(STACKADDR + cpu.sp);
			cpu.sp -= 1;
			cpu.pc.BYTE.UPPER = readMemory(STACKADDR + cpu.sp);
		}
	}else if(opcode == 1){
		//jump to address NNN
		word address;
		address.WORD = ((firstNibble | secondNibble) | thirdNibble);
		jmp( address );
	}else if(opcode == 2){
		//Interpret the address as a word
		word address;//Address stores instruction after the branch.
		word stackLoc;
		stackLoc.WORD = STACKADDR + cpu.sp;
		address.WORD = ((firstNibble | secondNibble) | thirdNibble);
		writeMemory(stackLoc, cpu.pc.BYTE.UPPER);
		stackLoc.WORD += 1;
		writeMemory(stackLoc, cpu.pc.BYTE.LOWER);
		cpu.sp += 2;//Sp points to top of stack as of now, points above it.`
		jmp( address );
		
	}else if(opcode == 3){
		//skip if equal (immediate)
		beq(cpu.reg[firstNibble >> 8], (secondNibble | thirdNibble) );
	}else if(opcode == 4){
		//skip if not equal (immediate)
		
		bne(cpu.reg[firstNibble >> 8], (secondNibble | thirdNibble) );
	}else if(opcode == 5){
		//skip if equal (register)
		if(thirdNibble == 0){
			beq(cpu.reg[(firstNibble >> 8)], cpu.reg[(secondNibble >> 4)]);
		}
	}else if(opcode == 6){
		//set value (immediate)
		load( (firstNibble >> 8), (secondNibble | thirdNibble) );
	}else if(opcode == 7){
		addReg( (firstNibble >> 8), (secondNibble | thirdNibble) );
		//add to register without carry (immediate)
	}else if(opcode == 8){
		if(thirdNibble == 0){
			load(firstNibble >> 8, cpu.reg[secondNibble >> 4]);
		}
		bitWiseOperations((firstNibble >> 8), (secondNibble >> 4), thirdNibble);
		arithmeticOperations(firstNibble >> 8, secondNibble >> 4, thirdNibble);
		shiftOperations(cpu.reg[(firstNibble >> 8)], thirdNibble);
	}else if(opcode == 9){
		//skip if not equal (register)
		if(thirdNibble == 0){
			bne( cpu.reg[firstNibble >> 8],cpu.reg[secondNibble >> 4] );
		}	
	}else if(opcode == 10){
		word address;
		address.WORD = ((firstNibble | secondNibble) | thirdNibble);
		setIndex( address );
	}else if(opcode == 11){
		word address;
		address.WORD = ((firstNibble | secondNibble) | thirdNibble);
		jmpOff( address );
	}else if(opcode == 12){
		randomFunction(firstNibble >> 8, (secondNibble | thirdNibble));
	}else if(opcode == 13){
		
	}else if(opcode == 14){
		
	}else if(opcode == 15){
		
	}else{
		fprintf(stdout, "Uh oh\n");
	}
}
//Functions below correspond to the instructions with opcode 8.
void bitWiseOperations(int reg1, int reg2, int id){
	if(id == 1){
		or(reg1, reg2);
	}else if(id == 2){
		and(reg1, reg2);
	}else if(id == 3){
		xor(reg1, reg2);
	}
}

void arithmeticOperations(int reg1, int reg2, int id){
	if(id == 4){
		addCarry(reg1, reg2);
	}else if(id == 5){
		subCarry(reg1, reg2);
	}else if(id == 7){
		subCarry(reg2, reg1);
	}
}
void shiftOperations(int reg1, int id){
	if(id == 6){
		rsh(reg1);
	}else if(id == 0xE){
		lsh(reg1);
	}
}

//7XNN
void addReg(int reg1, byte b1){
	cpu.reg[reg1] += b1;
}
//8XY5
void subCarry(int reg1, int reg2){
	cpu.reg[0xf] = (cpu.reg[reg1] > cpu.reg[reg2]);
	cpu.reg[reg1] -= cpu.reg[reg2];
}
//8XY1
void or(int reg1, int reg2){
	cpu.reg[reg1] |= cpu.reg[reg2];
}
//8XY2
void and(int reg1, int reg2){
	cpu.reg[reg1] &= cpu.reg[reg2];
}
//8XY4
//Handles carry flags
void addCarry(int reg1, int reg2){
	cpu.reg[0xf] = (!(cpu.reg[reg1])) < cpu.reg[reg2];
	
	//printf("Registers:%d, %d\n", reg1, reg2);
	cpu.reg[reg1] += cpu.reg[reg2];
}
//8XY3
void xor(int reg1, int reg2){
	cpu.reg[reg1] ^= cpu.reg[reg2];
}
//8XYE
void lsh(int reg1){
	cpu.reg[0xf] = ((cpu.reg[reg1] & 0x8000) != 0);
	cpu.reg[reg1] <<= 1;
}
//8XY6
void rsh(int reg1){
	cpu.reg[0xf] = cpu.reg[reg1] & 0x0001;
	cpu.reg[reg1] >>= 1;
}
//1NNN
void jmp(word nextInstr){
	printf("Jumping\n");
	cpu.pc.WORD = nextInstr.WORD & 0x0FFF;
}
//BNNN
void jmpOff(word w1){
	cpu.pc.WORD = cpu.reg[0x0] + (w1.WORD & 0x0fff);
}
//ANNN
void setIndex(word w1){
	cpu.i.WORD = w1.WORD & (0x0fff);
}

//FX1E
void addIndex(int reg1){
	cpu.i.WORD += cpu.reg[reg1];
}

//CXNN
void randomFunction(int reg1, byte b1){
	cpu.reg[reg1] = rand() & b1;
}

//3XNN, 5XY0, X, Y is a register
//For registers AND immediate values.
void beq(byte b1, byte b2){
	if(b1 == b2){
		cpu.pc.WORD += WORD_LEN;
	}
}
//4XNN
void bne(byte b1, byte b2){
	printf("Not equal:%d, %d\n", b1, b2);
	if(b1 != b2){
		cpu.pc.WORD += WORD_LEN;
	}
}
//

//For registers and immediate values.
//6XNN
void load(int reg1, byte b1){
	printf("Register and byte:%d, %d\n", reg1, b1);
	cpu.reg[reg1] = b1;
}
#endif
