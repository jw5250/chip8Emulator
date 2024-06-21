//Notes on the following ROMs:
	//RPS.ch8:
		//Screen moves much faster than it should be.
		//This is because there is no use of a timer, just a loop that counts to 160.
			//Solution: Have the CPU run at a certain speed.
//What next?:
	//Test on more things.
	//Write command line system for running this emulator.
	//Maybe add a debugging tool to this?
	//Extend to super chip 8
	//Redesign to allow custom beep sounds.
//The main machine.
#include "cpu.h"
#include "memory.h"
#include "screen.h"
#include "dataType.h"
#include "loader.h"
#include "timer.h"
#include "keyboard.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <time.h>


int startMachine(char* ROM){
	initInput();
	initTimers();
	initMemory();
	int flag = loadFile(ROM);
	if(flag == -1){
		return 1;
	}
	int error = initScreen();
	if(error == -1){
		fprintf(stderr, "Error with starting screen.\n");
		return 1;
	}
	initCpu();
	if( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1 ){
		fprintf(stderr, "Video, audio couldn't be initialized.\n");
		return 1;
	}
	if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }
	//Keep track of all events.
	SDL_Event e;
	bool quit = false;
	Mix_Chunk* sound = NULL;
	int channel = -1;
	sound = Mix_LoadWAV("sounds/beep.wav");
	if(sound == NULL){
		fprintf(stderr, "Failed to load in sound. Error:%s\n", Mix_GetError());
	}
	//Milliseconds needed for an event to occur.
	const int eventMilliseconds = 17;//17 actually, rounding up from 16.6666 (1000 milliseconds per 60 events.)
	uint32_t timer = SDL_GetTicks();
	//Do this to keep the screen up.
	while(!quit){
		//Parse inputs
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}
			if(e.type == SDL_KEYDOWN){
				takeInput(e.key.keysym.sym);
			}
			if(e.type == SDL_KEYUP){
				removeInput(e.key.keysym.sym);
			}
		}	
		//Check the sound.
		if( checkSoundTimer() == true ){
			//printf("Halting channel.\n");
			Mix_HaltChannel(channel);
		}else{
			if(Mix_Playing(-1) == 0){
    			channel = Mix_PlayChannel(-1, sound, 0);
			}
		}
		//Timer is at most 60hz.
		if(SDL_GetTicks()-timer >= eventMilliseconds){
			decrementTimers();
			timer = SDL_GetTicks();//Set time reference to when the last time the timers were decremented.
		}
		cpuLoop(getMostRecentKeyPressed());
		resetRecentPressed();

	}
	deleteScreen();

    Mix_FreeChunk(sound);
    sound = NULL;

	Mix_Quit();
	SDL_Quit();
	return 0;
}
int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "Needs a .ch8 file: ./main [file name]\n");
		return 1;
	}
	startMachine(argv[1]);
	/*initMemory();
	//Load in a test rom.
	int flag = loadFile("testROM.txt");
	if(flag == -1){
		return 1;
	}*/

	return 0;
}