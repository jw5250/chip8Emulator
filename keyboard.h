#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED
#define EMPTY_KEY 16
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "dataType.h"
void initInput();

void takeInput(SDL_Keycode c);

void removeInput(SDL_Keycode c);

bool findKey(byte encode);

byte getMostRecentKeyPressed();

void resetRecentPressed();
#endif