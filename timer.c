#include "timer.h"
#include "dataType.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

/*
Timers run at 60hz.
*/

/*
Separate value here is needed to determine at what time the timer should go.

soundTimer has the same purpose for its respective timer.
*/
static byte delayTimer;

static byte soundTimer;



void initTimers(){
    delayTimer = 0;
    soundTimer = 0;
}
void decrementTimers(){
    if(delayTimer != 0){
        delayTimer--;
    }
    if(soundTimer != 0){
        soundTimer--;
    }
}

void setDelayTimer(byte milliseconds){
    delayTimer = milliseconds;
}


byte getDelayTimerValue(){

    return delayTimer;
}

void setSoundTimer(byte milliseconds){
    soundTimer = milliseconds;
}

bool checkSoundTimer(){
    //printf("Sound timer value:%d\n", soundTimer);
    if ( soundTimer == 0 ){
        //If this is true, stop the sound.
        return true;
    }
    //If beep sound isn't playing, play beep sound.
    return false;
}