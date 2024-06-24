//What next?:
	//Maybe add a debugging tool to this?
	//Extend to super chip 8(?)
	//Redesign to allow custom beep sounds.
//The main machine.
//Design concerns:
	//Need to now maintain two constants controlling frequency of operation:
		//One in cpu.c draw function
		//One here.
//Current goals:
	//Add in new debug feature to switch between running it by step vs having it automatically go.
		//How do I sync the timers with the step?
	//Refactor the emulator in accordance to Design Concerns section.
	//Try optimizing draw functions
	//Test danm8ku.ch8 more.
		//why does it chug so bad? are the bullets supposed to linger on the left side of the screen?
//When steps are on:
	//Milliseconds used to sync timer will only be processed if a button is pressed
	//Sound will be resumed if a button is presesed, paused otherwise.
	//CPU is processed after a single button pressed.
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
	const int eventMilliseconds = 17;//17 actually, rounding up from 16.6666 (1000 milliseconds per 60 events.)
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
	if(Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

	srand(time(NULL));
	//Keep track of all events.
	SDL_Event e;
	bool quit = false;
	Mix_Chunk* sound = NULL;
	int channel = -1;
	sound = Mix_LoadWAV("sounds/beep.wav");
	if(sound == NULL){
		fprintf(stderr, "Failed to load sound. Error:%s\n", Mix_GetError());
	}
	//Milliseconds needed for an event to occur.


	uint32_t timer = SDL_GetTicks();

	bool stepsOn = false;
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
			if(e.key.keysym.sym == SDLK_p){
				stepsOn = !stepsOn;
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
	freeMemory();
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
	return 0;
}