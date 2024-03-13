
//Proposed Libraries:
	//Graphics and sounds:
		//SDL

//Status:
	//Execution loop works properly (as of now)
		
//Work on:
	//Figure out how to implement the following:
		//Sprite functions
			//Functions:
		//Timers
			//Timer Stuff:
		//Sounds
			//Sounds:
				//Beep played when soudn timer value reaches zero
		//Inputs
		//Other opcodes that aren't the machine routine start.
			//ex. FX33
//Main issues:
	//Need to build I/O
		//The input board
			//Handle with SDL event queue system
		//The screen and I/O commands
			//62 by 32 display resolution
				//monochrome
#include "cpu.h"
#include "memory.h"
#include "dataType.h"
#include <stdio.h>
#include <SDL2/SDL.h>//Audio and sound library.
#include <stdlib.h>
	
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

int main(int argc, char** argv){
	
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
	return 0;
}
