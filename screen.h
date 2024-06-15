#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

//This should contain data and functions related to the screen.

#include <stdbool.h>
#include <SDL2/SDL.h>

#define SCRNLEN 64

#define SCRNHEIGHT 32
//Size of the pixel.
#define PIXELSIZE 10


//64x32 bit

//Initialize all data pertaining to the screen. Clear it.
int initScreen();

//Clear the screen.
void clearScreen();

//Update the screen based on data from the frame buffer.
void updateScreen();

//Delete the screen.
void deleteScreen();

//Update a single chip 8 "pixel."
bool updatePixelInFrameBuffer(int x, int y, bool val);

#endif