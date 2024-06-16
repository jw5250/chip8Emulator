#ifndef CPU_C_INCLUDED
#define CPU_C_INCLUDED
#define WORD_LEN 2
#define BYTE_LEN_IN_BITS 8
#include "cpu.h"
#include "timer.h"
#include "memory.h"
#include "screen.h"
#include "keyboard.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define STACKSIZE 48
//0x50 is 80 in hex
#define STACKADDR 0x50
//inline:Larger executable, but functions declared with it don't need to be pushed onto the stack and popped (alongside result).
//I could rewrite the giant if else statement into a jump table. Is it worth it though?
	//I could also rewrite it into a switch statement.
//Stores whether the machine should wait for an input or not.
static bool awaitKeyboardInput;
//Stores the register target in case of await input.
static int registerStorage;
//Stores the most recent key input into the cpu.
static byte keyStorage;


void initCpu(){
	awaitKeyboardInput = false;
	keyStorage = EMPTY_KEY;
	cpu.pc.WORD = STARTADDR;
	//Stack size is 96 bytes long. Up to 48 levels of function calls
	//According to my sources its actually half that but I want to try this first.
	cpu.sp = 0;
}



//7XNN
static inline void addReg(int reg1, byte b1){
	cpu.reg[reg1] += b1;
}


/* 
	The following three instructions will reset the vF flag to 0 or not.
	According to chip8 specifications, vF flags must be reset.
*/

//8XY1
static inline void or(int reg1, int reg2){
	cpu.reg[reg1] |= cpu.reg[reg2];
	cpu.reg[0xF] = 0;
}
//8XY2
static inline void and(int reg1, int reg2){
	cpu.reg[reg1] &= cpu.reg[reg2];
	cpu.reg[0xF] = 0;
}

//8XY3
static inline void xor(int reg1, int reg2){
	cpu.reg[reg1] ^= cpu.reg[reg2];
	cpu.reg[0xF] = 0;
}

//8XY4
//Handles carry flags
//If reg f would have been modified, its value is overridden by the carry flag.
static inline void addCarry(int reg1, int reg2){
	//cpu.reg[0xf] = ((byte)(~(cpu.reg[reg1]))) < (byte)(cpu.reg[reg2]);
	//Note: If I don't cast these as bytes the operator won't treat the results as such.

	//printf("Registers:%d, %d\n", reg1, reg2);
	cpu.reg[reg1] += cpu.reg[reg2];
	//When overflow causes a wrap around, result will always be less than the least valued register.
	cpu.reg[0xf] = (byte)(cpu.reg[reg1]) < (byte)cpu.reg[reg2];
}

//8XY5
//If reg f would have been modified, its value is overridden by the carry flag.
static inline void subCarry(int reg1, int reg2){
	byte a = cpu.reg[reg1];
	byte b = cpu.reg[reg2];

	cpu.reg[reg1] -= cpu.reg[reg2];
	cpu.reg[0xf] = (a >= b);
	//printf("SubCarry\nReg %d:%d\n", reg1, cpu.reg[reg1]);
}

//8XY7
static inline void subCarryInvert(int reg1, int reg2){
	byte a = cpu.reg[reg1];
	byte b = cpu.reg[reg2];

	cpu.reg[reg1] = cpu.reg[reg2] - cpu.reg[reg1];
	cpu.reg[0xf] = (a <= b);
	//printf("SubCarry\nReg %d:%d\n", reg1, cpu.reg[reg1]);
}


/*
According to chip8, following code doesn't have vX modify itself.
vX = vY <</>> n
*/


//8XY6
//Shift right contents in a register by 1
//If reg f would have been modified, its value is overridden by the carry flag.
static inline void rsh(int reg1, int reg2){
	byte a = (cpu.reg[reg1]) & 0x01;
	cpu.reg[reg1] = cpu.reg[reg2] >> 1;
	cpu.reg[0xf] = a;
}

//8XYE
//Shift left contents in a register by 1
//If reg f would have been modified, its value is overridden by the carry flag.
static inline void lsh(int reg1, int reg2){
	//printf("Left shift register %d before:%d\n", reg1, cpu.reg[reg1]);
	byte a = ( ((cpu.reg[reg1]) & 0x80) != 0);//This doesn't work.
	//printf("Overflow flag:%d\n", a);
	cpu.reg[reg1] = cpu.reg[reg2] << 1;
	cpu.reg[0xf] = a;
	//printf("Left shift register %d:%d\n", reg1, cpu.reg[reg1]);
}

//1NNN
static inline void jmp(word nextInstr){
	//printf("Jumping to address %x\n", nextInstr.WORD);
	cpu.pc.WORD = nextInstr.WORD & 0x0fff;
}
//BNNN
static inline void jmpOff(word w1){
	cpu.pc.WORD = cpu.reg[0x0] + (w1.WORD & 0x0fff);
}
//ANNN
static inline void setIndex(word w1){
	//printf("set index\n");
	cpu.i.WORD = w1.WORD & (0x0fff);
}


//FX1E
static inline void addIndex(int reg1){
	cpu.i.WORD += cpu.reg[reg1];
}

//FX29
	//Gets font data
	//If reg1 contains 1, i is set to the start address of the sprite of '1,' etc..
	//Requires font data existing for this particular sprite.
static inline void getIndexLocation(int reg1){
	// Note:
		// No information as to what happens if reg1 contains a value greater than 0xf!
	cpu.i.WORD = cpu.reg[reg1] * FONT_HEIGHT;
}


//CXNN
static inline void randomFunction(int reg1, byte b1){
	//Maybe in future I'll set seed to be more random (seed it based on time.).
	cpu.reg[reg1] = rand() & b1;
}

//3XNN, 5XY0, X, Y is a register
//For registers AND immediate values.
static inline void beq(byte b1, byte b2){
	if(b1 == b2){
		cpu.pc.WORD += WORD_LEN;
	}
}

//4XNN
static inline void bne(byte b1, byte b2){
	if(b1 != b2){
		cpu.pc.WORD += WORD_LEN;
	}
}


//6XNN
static inline void load(int reg1, byte b1){
	//printf("Register and byte:%d, %d\n", reg1, b1);
	cpu.reg[reg1] = b1;
}

//For storing a decimal number as a set of binary digits at address I.
/*
i:hundred
i+1:tens
i+2:ones
//FX33
*/
static inline void bcdEncode(int reg1){
	byte buffer = (byte)(cpu.reg[reg1]);
	writeMemory( cpu.i, buffer/100 );
	buffer = cpu.reg[reg1] % 100;
	
	word iRegStore = cpu.i;

	iRegStore.WORD = cpu.i.WORD + 1;

	writeMemory( iRegStore, buffer/10 );
	buffer = cpu.reg[reg1] % 10;

	iRegStore.WORD = cpu.i.WORD + 2;
	writeMemory( iRegStore, buffer );
}

//DXYN
//Note:
	//hor and ver don't need to be shorts. I don't remember why I did this. I'll probably change this in the future.
static inline void draw(int regX, int regY, int nBytes){
	//printf("Drawing\n");
	int i = 0;
	byte bitMasks[BYTE_LEN_IN_BITS] = {128, 64, 32, 16, 8, 4, 2, 1};
	bool collisionOccuredAtSomeTime = false;
	//Interpret initial coordinates AS screen coordinates
	unsigned short verOr = cpu.reg[regY] % SCRNHEIGHT;
	unsigned short horOr = cpu.reg[regX] % SCRNLEN;
	while(i < nBytes){
		int j = 0;
		byte b = readMemory(cpu.i.WORD + i);
		unsigned short ver = (verOr + i) /*% SCRNHEIGHT*/;
		if(ver >= SCRNHEIGHT){
			//If ver is greater than or equal to the frame buffer at any point, stop drawing.
			break;
		}
		while(j < BYTE_LEN_IN_BITS){
		 	bool collisionOccured = false;
			unsigned short hor = (horOr + j) /*% SCRNLEN*/;
			if(hor >= SCRNLEN){
				break;
			}
			
			if( (b & bitMasks[j]) != 0){
				collisionOccured = updatePixelInFrameBuffer(hor, ver, true);
			}else{
				collisionOccured = updatePixelInFrameBuffer(hor, ver, false);
			}
			if(collisionOccured == true){
				
				collisionOccuredAtSomeTime = true;
			}
			j++;
		}
		i++;
	}
	cpu.reg[0xF] = collisionOccuredAtSomeTime;
	updateScreen();
}


//Get the delay timer value(FX07)
static inline void getDTimerValue(int reg1){
	cpu.reg[reg1] = getDelayTimerValue();
	//printf("Timer value:%d\n", cpu.reg[reg1]);
}


//Set the delay timer value(FX15)
static inline void setDTimerValue(int reg1){
	setDelayTimer(cpu.reg[reg1]);
}
//Set the sound timer value(FX18)
static inline void setSTimerValue(int reg1){
	setSoundTimer(cpu.reg[reg1]);
}

//If a key is pressed that has the same value as vX (or not, depending on targetVal), skip next instruction.
//EX9E (if pressed, skip next instruction), EXA1 (if not pressed, skip next instruction.)
static inline void keyPressed(int reg1, bool targetVal){
	if(findKey(cpu.reg[reg1]) == targetVal){
		cpu.pc.WORD += WORD_LEN;
	}
}

//FX0A
static inline void awaitInput(int reg1){
	//CPU now awaits input from keyboard.
	awaitKeyboardInput = true;
	registerStorage = reg1;
	//When input is received, get the input and store in a given instruction.
}


/*
According to chip8,
	Following two opcodes below increment index pointer.
*/

//FX55
//Dump all register values into memory(0 to X), starting from address i. (corresponding to starting with reg 0)
static inline void dumpReg(int reg1){
	word addrBuffer;
	//Keeping the addrBuffer code around in case I plan to allow change of interpreter.
	addrBuffer.WORD = cpu.i.WORD;
	for(int j = 0; j <= reg1;j++){
		writeMemory(addrBuffer, cpu.reg[j]);
		addrBuffer.WORD++;
		cpu.i.WORD++;
	}
}

//FX65
//Load all register values from memory(0 to X), starting from address i. (corresponding to starting with reg 0).
static inline void loadReg(int reg1){
	word addrBuffer;
	addrBuffer.WORD = cpu.i.WORD;
	for(int j = 0; j <= reg1;j++){
		cpu.reg[j] = readMemory(addrBuffer.WORD);
		addrBuffer.WORD++;
	}
}

//Functions below correspond to the instructions with opcode 8.
static inline void bitWiseOperations(int reg1, int reg2, int id){
	if(id == 1){
		or(reg1, reg2);
	}else if(id == 2){
		and(reg1, reg2);
	}else if(id == 3){
		xor(reg1, reg2);
	}
}

static inline void arithmeticOperations(int reg1, int reg2, int id){
	if(id == 4){
		addCarry(reg1, reg2);
	}else if(id == 5){
		subCarry(reg1, reg2);
	}else if(id == 7){
		subCarryInvert(reg1, reg2);
	}
}

static inline void shiftOperations(int reg1, int reg2, int id){
	if(id == 6){
		rsh(reg1, reg2);
	}else if(id == 0xE){
		lsh(reg1, reg2);
	}
}

void cpuLoop(int mostRecentKey){
	//Issue:
		//Needs to store the key value, then wait until the key is released.
	//Issue: how do we know when the program ends?
		//Assume it loops infinitely at the end. Hopefully.	
	if(awaitKeyboardInput == true){
		if( (mostRecentKey != EMPTY_KEY) && (keyStorage == EMPTY_KEY)){
			cpu.reg[registerStorage] = mostRecentKey;
			keyStorage = mostRecentKey;
		}
		if(keyStorage == EMPTY_KEY){
			return;
		}
		if(findKey(keyStorage) == false){
			//printf("Keyboard input was received. Turning off signal\n");
			awaitKeyboardInput = false;
			keyStorage = EMPTY_KEY;
		}

		//Done in case input is awaited.
		return;
	}
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
		//Stack grows down from ROM load point. Is this a bad idea?
		if( ((firstNibble | secondNibble) | thirdNibble) == 0x00EE){
			word stackLoc;
			//printf("Returning\n");	
			cpu.sp -= 1;
			cpu.pc.BYTE.LOWER = readMemory(STACKADDR + cpu.sp);
			cpu.sp -= 1;
			cpu.pc.BYTE.UPPER = readMemory(STACKADDR + cpu.sp);
		//Clear the screen.
		} else if( ((firstNibble | secondNibble) | thirdNibble) == 0x00E0 ){
			//Calls the screen refreshing routine.
			 clearScreen();
			 updateScreen();
		}
	}else if(opcode == 1){
		//jump to address NNN
		word address;
		address.WORD = ((firstNibble | secondNibble) | thirdNibble);
		jmp( address );
	}else if(opcode == 2){
		//Can be simplified.

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
		arithmeticOperations((firstNibble >> 8), (secondNibble >> 4), thirdNibble);
		shiftOperations((firstNibble >> 8), (secondNibble >> 4), thirdNibble);
	}else if(opcode == 9){
		//skip if not equal (register)
		if(thirdNibble == 0){
			bne( cpu.reg[firstNibble >> 8], cpu.reg[secondNibble >> 4] );
		}	
	}else if(opcode == 0xA){
		word address;
		address.WORD = ((firstNibble | secondNibble) | thirdNibble);
		setIndex( address );
	}else if(opcode == 0xB){
		word address;
		address.WORD = ((firstNibble | secondNibble) | thirdNibble);
		jmpOff( address );
	}else if(opcode == 0xC){
		randomFunction(firstNibble >> 8, (secondNibble | thirdNibble));
	}else if(opcode == 0xD){
		draw(firstNibble >> 8, secondNibble >> 4, thirdNibble);
	}else if(opcode == 0xE){
		if( currentInstruction.BYTE.LOWER == 0x9E ){
			keyPressed( (firstNibble >> 8), true );
		}else if(currentInstruction.BYTE.LOWER == 0xA1){
			keyPressed( (firstNibble >> 8), false );
		}
	}else if(opcode == 0xF){
		if( currentInstruction.BYTE.LOWER == 0x33 ){
			bcdEncode( (firstNibble >> 8) );
		}else if( currentInstruction.BYTE.LOWER == 0x1E ){
			addIndex( (firstNibble >> 8) );
		}else if( currentInstruction.BYTE.LOWER == 0x29 ){
			//Requires that on startup, digit fonts are already placed where they should be.
			getIndexLocation( (firstNibble >> 8) );
		}else if( currentInstruction.BYTE.LOWER == 0x55 ){
			dumpReg( (firstNibble >> 8) );
		}else if( currentInstruction.BYTE.LOWER == 0x65 ){
			//printf("Loading registers\n");
			loadReg( (firstNibble >> 8) );
		}else if( currentInstruction.BYTE.LOWER == 0x07 ){
			getDTimerValue( (firstNibble >> 8) );
		}else if( currentInstruction.BYTE.LOWER == 0x15 ){
			setDTimerValue( (firstNibble >> 8) );
		}else if( currentInstruction.BYTE.LOWER == 0x0A ){
			awaitInput( (firstNibble >> 8) );
		}else if( currentInstruction.BYTE.LOWER == 0x18 ){
			setSTimerValue( (firstNibble >> 8) );
		}
	}else{
		fprintf(stderr, "Uh oh\n");
	}
}




#endif
