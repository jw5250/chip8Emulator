#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

//The keyboard used by the chip8.

#define EMPTY_KEY 16
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "dataType.h"

//Initialize the input board and the most recent key pressed.
void initInput();


//Tell the input board that a key is currently being held.
void takeInput(SDL_Keycode c);

//Tell the input board that a key isn't currently being held.
void removeInput(SDL_Keycode c);


//Checks if a key is currently being held down.
bool findKey(byte encode);


//Check the most recent key pressed.
byte getMostRecentKeyPressed();


//Reset the most recent key pressed.
void resetRecentPressed();
#endif