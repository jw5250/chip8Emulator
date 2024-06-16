//Status:
	//Tests passed:
		//1, 2, 3, 4, , 6, 7
	//Tests failed:
		//5
			//In case of going for Chip 8:
				//Missing cases:
					//Draw rate.
						//Drawing sprites wait for vertical blank interrupt. Speed is limited to 60 frames per second.
						//Needs more cycles per frame.
							//How to count the amount of cycles per frame in your program?
								//hZ
	//Execution loop works properly
//What next?:
	//Write command line system for running this emulator.
	//Maybe add a debugging tool to this?
	//Extend to super chip 8
//The main machine.
//As of now, screen refreshes whenever the draw function is called.
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
	sound = Mix_LoadWAV("sounds/low.wav");
	if(sound == NULL){
		fprintf(stderr, "Failed to load in sound. Error:%s\n", Mix_GetError());
	}
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
			Mix_HaltChannel(channel);
		}else{
			if(Mix_Playing(-1) == 0){
        		channel = Mix_PlayChannel(-1, sound, 0);
			}
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
//How fast are certain operations on this computer?
int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "Needs a .ch8 file: ./main [file name]\n");
		return 1;
	}

	startMachine(argv[1]);
	return 0;
}