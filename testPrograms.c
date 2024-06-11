#include "cpu.h"
#include "memory.h"
#include "dataType.h"
#include <stdio.h>
#include <SDL2/SDL.h>//Audio and sound library.
#include <stdlib.h>

/*
Instructions must be stored big-endian.
	Given number 0x1234

	Stored in memory as:
	address n:12
	address (n + 1):34


*/
	
//General loop used to run at least one of the following below.
	/*initMemory();
	testFunctions();
	initCpu();

	cpuLoop();
	cpuLoop();
	cpuLoop();
	cpuLoop();
	cpuLoop();
	cpuLoop();
	cpuLoop();
	cpuLoop();

	for(int i = 0; i < 16;i++){
		printf("Register %d:%d\n", i, cpu.reg[i]);
	}
	free(memory);*/

void testFetchDecodeExecute(){
	//6XNN 6109 8XY0
	
	//Load 25 into register 1
	memory[STARTADDR] = 0x61;
	memory[STARTADDR+1] = 0x19;

	//Load register 1 into register 2
	memory[STARTADDR+2] = 0x82;
	memory[STARTADDR+3] = 0x10;

	//8XY4
	//Add to register 1 register 2.
	memory[STARTADDR+4] = 0x81;
	memory[STARTADDR+5] = 0x24;

}

//Writes 239 to register 0, then stores bcd encoded version at address 0x0111
void bcdEncodeTest(){
	//Save a number into the register vX.
	//6XNN
	word start;
	start.WORD = STARTADDR;
	writeMemory(start, 0x60);
	start.WORD++;	
	writeMemory(start, 0xEF);
	start.WORD++;
	//Set i register to equal some address.
	writeMemory(start, 0xA1);
	start.WORD++;
	writeMemory(start, 0x11);
	start.WORD++;
	//Call BCD encode on the register.
	//FX33
	writeMemory(start, 0xF0);
	start.WORD++;
	writeMemory(start, 0x33);
}

void randTest(){
	//Save a number into register vX via CXNN.
	word start;
	start.WORD = STARTADDR;
	writeMemory(start, 0xC0);
	start.WORD++;
	writeMemory(start, 0x0F);
	start.WORD++;
	writeMemory(start, 0xC0);
	start.WORD++;
	writeMemory(start, 0x0F);
	start.WORD++;
	writeMemory(start, 0xC0);
	start.WORD++;
	writeMemory(start, 0x0F);
}


void testConditionals(){
	//Load in a program that:
	//Loads an immediate value into register 0.
	//Branches to a statement given an immediate value.
		//Either sets register 1 to either one or another value.
	//6XNN
	//Load immediate
	memory[STARTADDR] = 0x60;
	memory[STARTADDR+1] = 0x05;
	
	//Branch if r0 != 5
	memory[STARTADDR+2] = 0x40;
	memory[STARTADDR+3] = 0x06;

	//Jump to r0 = 8
	memory[STARTADDR+4] = 0x12;
	memory[STARTADDR+5] = 0x0A;

	//r0 = 3
	memory[STARTADDR+6] = 0x60;
	memory[STARTADDR+7] = 0x03;

	memory[STARTADDR+8] = 0x12;
	memory[STARTADDR+9] = 0x0C;

	//r0 = 8
	memory[STARTADDR+10] = 0x60;
	memory[STARTADDR+11] = 0x08;


}

void testFunctions(){
	//Load zero into register 0
	memory[STARTADDR] = 0x60;
	memory[STARTADDR+1] = 0x00;


	//Jump to address 0x020A
	memory[STARTADDR+2] = 0x22;
	memory[STARTADDR+3] = 0x0A;

	//Add 3 to register 0
	memory[STARTADDR+4] = 0x70;
	memory[STARTADDR+5] = 0x03;
	

	//Add 1 to register 0
	memory[STARTADDR+10] = 0x70;
	memory[STARTADDR+11] = 0x01;


	//Jump to address 0x0220
	memory[STARTADDR+12] = 0x22;
	memory[STARTADDR+13] = 0x20;


	//Return
	memory[STARTADDR+14] = 0x00;
	memory[STARTADDR+15] = 0xEE;



	//Add 3 to register 0
	memory[STARTADDR+32] = 0x70;
	memory[STARTADDR+33] = 0x03;


	//Return.
	memory[STARTADDR+34] = 0x00;
	memory[STARTADDR+35] = 0xEE;


	//Do an addition, then run a piece of code somewhere else in memory.
}

//The following code below tests all of the screen related functions.
/*

Input diagram (respective to how chip 8 keys are mapped):
	1 2 3 4           1 2 3 C
	q w e r           4 5 6 D
	a s d f mapped to 7 8 9 E
	z x c v           A 0 B F

Keyboard implementation:
	If the event type is something, convert into a number corresponding to its position.

array:
	char inputs[15] = {
	'x', '1', '2', '3',
	'q', 'w', 'e', 'a',
	's', 'd', 'z', 'c',
	'4', 'r', 'f', 'v'};
	//Conversion:
	//SDL_Event -> inputs -> positional code
	//To convert to corresponding opcode, just iterate through array until keyboard input is equal to the character.

Input related instructions:
EX9E:Skip to next instruction if key stored in vX was pressed.
EXA1:Skip to next instruction if key stored in vX wasn't pressed.
FX0A:Halt all operations until an input is made.

*/
int test(){
	initScreen();
	SDL_Event e;
	bool quit = false;
	bool drawRandom = false;
	srand(1);
	while(!quit){
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}
			if (e.type == SDL_KEYDOWN){
				drawRandom = true;
			}
		}

		if(drawRandom == true){
			//Get a pseudorandom position on the screen and write to it to the frame buffer.
			//Change seed.
			//srand((unsigned)time(NULL));
			int x = rand() % SCRNLEN;
			int y = rand() % SCRNHEIGHT;

			updatePixelInFrameBuffer(x, y, true);

			drawRandom = false;
		}
		updateScreen();
	}
	deleteScreen();
	//Need event loop to keep the screen up.
	
	return 0;
}

//Test for the draw function's following features:
	//Drawing sprites onto the screen.
	//Forcing vF to be 0.
//Test for the draw function's following features:
	//Drawing sprites onto the screen.
	//Forcing vF to be 0.
void drawTest(){
	//Store font data in some arbitrary address (basic line).
	word fontLoc;
	fontLoc.WORD = 0x0000;
	writeMemory(fontLoc, 0xFF);
	fontLoc.WORD++;
	writeMemory(fontLoc, 0x81);
	fontLoc.WORD++;
	writeMemory(fontLoc, 0xFF);

	//Start of actual program:
	word startAddr;
	startAddr.WORD = STARTADDR;
	//Set arbitrary coordinates with vX and vY.
	writeMemory(startAddr, 0x60);//vX = 60
	startAddr.WORD++;
	writeMemory(startAddr, 0x3C);
	startAddr.WORD++;

	writeMemory(startAddr, 0x61);//vY = 1
	startAddr.WORD++;
	writeMemory(startAddr, 0x01);
	startAddr.WORD++;
	//Set i to point to the font data's location.
	writeMemory(startAddr, 0xA0);
	startAddr.WORD++;
	writeMemory(startAddr, 0x00);
	//Register vF is now 2.
	startAddr.WORD++;
	writeMemory(startAddr, 0x6F);
	startAddr.WORD++;
	writeMemory(startAddr, 0x02);
	
	//Call the draw function.
	startAddr.WORD++;
	writeMemory(startAddr, 0xD0);
	startAddr.WORD++;
	writeMemory(startAddr, 0x13);
	//Call again to XOR the drawing out.
	startAddr.WORD++;
	writeMemory(startAddr, 0xD0);
	startAddr.WORD++;
	writeMemory(startAddr, 0x13);
}

void dumpAndStore(){
	//Load in a bunch of numbers into registers from v0-vX, then write to i.
	word startAddr;
	startAddr.WORD = STARTADDR;
	//6XNN
	writeMemory(startAddr, 0x60);
	startAddr.WORD++;
	writeMemory(startAddr, 0x00);
	startAddr.WORD++;

	writeMemory(startAddr, 0x61);
	startAddr.WORD++;
	writeMemory(startAddr, 0x01);
	startAddr.WORD++;

	writeMemory(startAddr, 0x62);
	startAddr.WORD++;
	writeMemory(startAddr, 0x02);
	startAddr.WORD++;

	//ANNN
	//Initialize AN
	writeMemory(startAddr, 0xA0);
	startAddr.WORD++;
	writeMemory(startAddr, 0x00);
	startAddr.WORD++;

	//FX55
	writeMemory(startAddr, 0xF2);
	startAddr.WORD++;
	writeMemory(startAddr, 0x55);
	startAddr.WORD++;


	//FX65
	writeMemory(startAddr, 0xF2);
	startAddr.WORD++;
	writeMemory(startAddr, 0x65);
	startAddr.WORD++;
}

void addCarryTest(){
	//Save a number into the register vX.
	//6XNN
	word start;
	start.WORD = STARTADDR;
	writeMemory(start, 0x6F);
	start.WORD++;
	writeMemory(start, 0x34);
	start.WORD++;
	writeMemory(start, 0x60);
	start.WORD++;
	writeMemory(start, 0xF0);
	start.WORD++;
	//Add zero register to itself.
	//8XY4
	writeMemory(start, 0x80);
	start.WORD++;
	writeMemory(start, 0x04);
}


void loadTestInputs(){

	word startAddr;
	startAddr.WORD = STARTADDR;
	//FIRST LOOP BELOW
	//Instruction that skips to the next loop if a key is pressed(EX9E)
	writeMemory(startAddr, 0xE1);
	startAddr.WORD++;
	writeMemory(startAddr, 0xA1);

	//Instruction that jumps to the instruction after this one.(1NNN) 0x204
	startAddr.WORD++;
	writeMemory(startAddr, 0x12);

	startAddr.WORD++;
	writeMemory(startAddr, 0x06);


	//Instruction that jumps back to the start address(1NNN)
	startAddr.WORD++;
	writeMemory(startAddr, 0x12);

	startAddr.WORD++;
	writeMemory(startAddr, 0x00);


	//SECOND LOOP BELOW
	//Instruction that jumps to the await instruction if a key isn't pressed(EXA1)
	startAddr.WORD++;
	writeMemory(startAddr, 0xE1);

	startAddr.WORD++;
	writeMemory(startAddr, 0x9E);

	//Instruction that jumps to the await instruction(1NNN)
	startAddr.WORD++;
	writeMemory(startAddr, 0x12);

	startAddr.WORD++;
	writeMemory(startAddr, 0x0C);


	//Instruction that jumps back to the start address of the second loop.(1NNN)
	startAddr.WORD++;
	writeMemory(startAddr, 0x12);

	startAddr.WORD++;
	writeMemory(startAddr, 0x06);


	//AWAIT Part
	//Await instruction(FX0A)
	startAddr.WORD++;
	writeMemory(startAddr, 0xF2);

	startAddr.WORD++;
	writeMemory(startAddr, 0x0A);

	//Subroutine to print fonts
		//Clear the screen.(0x00E0)
	startAddr.WORD++;
	writeMemory(startAddr, 0x00);
	startAddr.WORD++;
	writeMemory(startAddr, 0xE0);

	//FX29
	//Takes the key value from register 2, set i to the font location.
	startAddr.WORD++;
	writeMemory(startAddr, 0xF2);
	startAddr.WORD++;
	writeMemory(startAddr, 0x29);

	//6XNN
	//Set x, y registers.
	startAddr.WORD++;
	writeMemory(startAddr, 0x63);
	startAddr.WORD++;
	writeMemory(startAddr, 0x0A);

	startAddr.WORD++;
	writeMemory(startAddr, 0x64);
	startAddr.WORD++;
	writeMemory(startAddr, 0x0A);

	//DXYN
	//writes the number to (10, 10) on screen.
	startAddr.WORD++;
	writeMemory(startAddr, 0xD3);
	startAddr.WORD++;
	writeMemory(startAddr, 0x45);

	//Instruction that jumps to await instruction.
	startAddr.WORD++;
	writeMemory(startAddr, 0x12);
	startAddr.WORD++;
	writeMemory(startAddr, 0x0C);
}

//Test following instructions:
	//Timer instructions (FX15, FX07)
	//Also should use print statements to log changes to make sure everything works properly.
void timerROM(){
	//Set the timer.
	//Get the value of the timer and put into register
	//If timer value is zero, jump to a function that draws.
	//Else, jump to the instruction that just gets the timer value.
	word startAddr;
	//Load in a value into register 0
	startAddr.WORD = STARTADDR;
	writeMemory(startAddr, 0x60);
	startAddr.WORD++;
	writeMemory(startAddr, 0x01);

	startAddr.WORD++;
	writeMemory(startAddr, 0x61);
	startAddr.WORD++;
	writeMemory(startAddr, 0x00);

	//Set the timer (FX15)
	startAddr.WORD++;
	writeMemory(startAddr, 0xF0);
	startAddr.WORD++;
	writeMemory(startAddr, 0x15);

	//Get the timer value, put in register 0.
	startAddr.WORD++;
	writeMemory(startAddr, 0xF0);
	startAddr.WORD++;
	writeMemory(startAddr, 0x07);

	//If timer value isn't equal to zero, skip next instruction.
	//Target: Register zero
	startAddr.WORD++;
	writeMemory(startAddr, 0x40);
	startAddr.WORD++;
	writeMemory(startAddr, 0x00);

	//Jump to a part that will constantly loop
	startAddr.WORD++;
	writeMemory(startAddr, 0x12);
	startAddr.WORD++;
	writeMemory(startAddr, 0x0E);

	//Jump to the get timer.
	startAddr.WORD++;
	writeMemory(startAddr, 0x12);
	startAddr.WORD++;
	writeMemory(startAddr, 0x06);

	startAddr.WORD++;
	writeMemory(startAddr, 0x12);
	startAddr.WORD++;
	writeMemory(startAddr, 0x0E);


}