//This should contain data related to the screen.
//Progress:
	//Everything here works as expected.
		//Extremely slow to update! Later will work on figuring out how to make it run faster.
			//Suspect blit may be a big contributor, don't know enough about SDL currently to make the decision.

#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <stdbool.h>
#include <SDL2/SDL.h>

#define SCRNLEN 64

#define SCRNHEIGHT 32
//Size of the pixel.
#define PIXELSIZE 10


//64x32 bit

/*
	Removed win, framebuffer these from this file, as they are supposed to be concrete values only used in this file.
	If I don't, mac compiler complains that these labels have no actual value respective to them
		Seen as unresolved symbols in linker
*/

int initScreen();

void clearScreen();

void updateScreen();

void deleteScreen();

bool updatePixelInFrameBuffer(int x, int y, bool val);
#endif