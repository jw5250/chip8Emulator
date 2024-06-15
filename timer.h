#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED
//Handles all of the timers (sound and delay)

#include <SDL2/SDL.h>
#include <stdbool.h>

//Initialize all timer data.
void initTimers();

//Set the delay timer to some value.
void setDelayTimer(uint32_t milliseconds);



//Get the current value of the delay timer.
uint32_t getDelayTimerValue();



//Set the sound timer to some value.
void setSoundTimer(uint32_t milliseconds);

//See if the sound timer has expired (return true) or not (return false)
bool checkSoundTimer();


#endif