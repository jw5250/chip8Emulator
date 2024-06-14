#include "timer.h"
#include <SDL2/SDL.h>
#include <stdbool.h>


uint32_t delayTimer;
uint32_t startDelayTime;

uint32_t soundTimer;
uint32_t startSoundTime;





void initTimers(){
    delayTimer = 0;
    startDelayTime = 0;
    soundTimer = 0;
    startSoundTime = 0;

}

void setDelayTimer(uint32_t milliseconds){
    delayTimer = milliseconds;
    startDelayTime = SDL_GetTicks();
    //printf("%d\n", startDelayTime);
}


uint32_t getDelayTimerValue(){
	//Gets the amount of time that elapsed since the timer started.
    uint32_t currentTime = SDL_GetTicks() - startDelayTime;
    if(currentTime >= delayTimer){
        return 0;
    }
	//As the delay timer decrements at a rate of 60hz, do this operation.
    return delayTimer-currentTime;
}

void setSoundTimer(uint32_t milliseconds){
    soundTimer = milliseconds;
    startSoundTime = SDL_GetTicks();
    //printf("%d\n", startSoundTime);
    //Start sound.
}

bool checkSoundTimer(){
    if ( (SDL_GetTicks() - startSoundTime) >= soundTimer ){
        //If this is true, stop the sound.
        return true;
    }
    //If beep sound isn't playing, play beep sound.
    return false;
}