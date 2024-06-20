#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED
//Handles all of the timers (sound and delay)
#include "dataType.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

//Initialize all timer data.
void initTimers();


//Decrement timer values based on frequency.
void decrementTimers();

//Set the delay timer to some value.
void setDelayTimer(byte milliseconds);




//Get the current value of the delay timer.
byte getDelayTimerValue();


//Set the sound timer to some value.
void setSoundTimer(byte milliseconds);

//See if the sound timer has expired (return true) or not (return false)
bool checkSoundTimer();


#endif