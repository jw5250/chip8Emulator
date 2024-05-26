//Proposed Libraries:
	//Graphics and sounds:
		//SDL
//Status:
	//Execution loop works properly (as of now)
	//Finished writing pixel draw functions for the code.
//Work on:
	//Figure out how to implement the following:
		//Sprite functions
			//Functions:
		//Timers
			//Timer Stuff:
				//Probably use operating system clock feature.
		//Sounds
			//Sounds:
				//Beep played when sound timer value reaches zero
		//Inputs
		//Other opcodes that aren't the machine routine start.
			//ex. FX33


//Main issues:
	//Need to build I/O
		//The input board
			//Handle with SDL event queue system
#include "cpu.h"
#include "memory.h"
#include "screen.h"
#include "dataType.h"
#include <stdio.h>
#include <SDL2/SDL.h>//Audio and sound library.
#include <stdlib.h>
#include <time.h>
//The following code below tests all of the screen related functions.
int main(int argc, char** argv){
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
