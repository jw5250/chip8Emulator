#include "timer.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
/*

General philosophy for how the timers work:
    Progress of the timer is the current time since the SDL library was initialized minus that of when the timer started.

    The difference between the current time since initialization and that taken when the timer started is the time that
    currently has passed.

*/


/*
Separate value here is needed to determine at what time the timer should go.

soundTimer has the same purpose for its respective timer.
*/
uint32_t delayTimer;


//Reference of when the timer was started. startSoundTime has the same purpose for its respective timer.
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
}


uint32_t getDelayTimerValue(){
	//Gets the amount of time that elapsed since the timer started.
    uint32_t currentTime = SDL_GetTicks() - startDelayTime;
    if(currentTime >= delayTimer){
        return 0;
    }
    return delayTimer-currentTime;
}

void setSoundTimer(uint32_t milliseconds){
    soundTimer = milliseconds;
    startSoundTime = SDL_GetTicks();
}

bool checkSoundTimer(){
    if ( (SDL_GetTicks() - startSoundTime) >= soundTimer ){
        //If this is true, stop the sound.
        return true;
    }
    //If beep sound isn't playing, play beep sound.
    return false;
}