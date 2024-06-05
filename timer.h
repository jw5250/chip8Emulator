#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED


//Delay timer instructions:
    //Get the delay timer value(FX07)
        //Delay timer value 
    //Set the delay timer value(FX015)
//Sound timer instructions

//Need two timer objects:
    //One for sound
    //One for other stuff.

#include <SDL2/SDL.h>
#include <stdbool.h>

void initTimers();
void setDelayTimer(uint32_t milliseconds);
uint32_t getDelayTimerValue();

void setSoundTimer(uint32_t milliseconds);
bool checkSoundTimer();


#endif