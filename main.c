//TODO:
	//Learn SDL_Mix
//Status:
	//According to all tests, everything is working properly.
	//Maybe add a debugging tool to this as well?
	//Tests passed:
		//1, 2, 3, 4
			//Need to check for quirks of the system.
				//Goal: Chip8 quirks only.
	//Execution loop works properly
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



//Conceptual test for sound:
	//Await an input, then play the beep sound for 0xFF milliseconds.
void soundROM(){
	word startAddr;
	startAddr.WORD = STARTADDR;
	//Await an input (FX0A)
	writeMemory(startAddr, 0xF0);
	startAddr.WORD++;
	writeMemory(startAddr, 0x0A);
	startAddr.WORD++;
	
	writeMemory(startAddr, 0x61);
	startAddr.WORD++;
	writeMemory(startAddr, 0xF0);
	startAddr.WORD++;

	//Set the sound timer. (FX18)

	writeMemory(startAddr, 0xF1);
	startAddr.WORD++;
	writeMemory(startAddr, 0x18);
	startAddr.WORD++;

	//Loop to the first instruction. (1NNN)

	writeMemory(startAddr, 0x12);
	startAddr.WORD++;
	writeMemory(startAddr, 0x00);
	startAddr.WORD++;


}

int startMachine(char* ROM){
	initInput();
	initMemory();
	//loadFile(ROM);
	soundROM();
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

int main(int argc, char** argv){

	startMachine("testFiles/3-corax+.ch8");
	/*if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0){
		fprintf(stderr, "Couldn't initialize timer or audio subsystems.\n");
		return 1;
	}
   if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    }

	Mix_Chunk* test = Mix_LoadWAV("sounds/low.wav");
	if(test == NULL){
		fprintf(stderr, "Failed to load in test. Error:%s\n", Mix_GetError());
	}
	
	Mix_PlayChannel(-1, test, -1);

	Mix_FreeChunk(test);
	test = NULL;
	
	initTimers();
	//10 second delay timer.

	
	setSoundTimer(1000);
	bool soundOff = checkSoundTimer();
	while(soundOff == false){
		soundOff = checkSoundTimer();
	}
	setDelayTimer(10000);
	uint32_t value = getDelayTimerValue(); 
	while(value != 0){
		value = getDelayTimerValue(); 
	}
	printf("Finished delay timer.\n");

	
	printf("End of test.\n");
	
	Mix_Quit();
	SDL_Quit();
	
	*/
	return 0;
}