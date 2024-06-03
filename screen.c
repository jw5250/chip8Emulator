#include "screen.h"
#include <SDL2/SDL.h>//Audio and sound library.

//Functions exclusive to this file:
void drawPixel(int x, int y, int n, bool val, SDL_Surface *surf);

bool frameBuffer[SCRNHEIGHT][SCRNLEN];

SDL_Window* win = NULL;

int initScreen(){
	win = SDL_CreateWindow("Chip8 Screen",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				SCRNLEN*PIXELSIZE,
				SCRNHEIGHT*PIXELSIZE,
				SDL_WINDOW_SHOWN
	);
	if(win == NULL){
		fprintf(stderr, "Couldn't create screen.\n");
		return -1;
	}
	clearScreen();
	return 0;
}


//Set all of the internal frame buffer values to 0, and then update the frame buffer.
void clearScreen(){
	for(int i = 0; i < SCRNHEIGHT;i++){
		for(int j = 0; j < SCRNLEN; j++){
			frameBuffer[i][j] = false;
		}
	}
}

//Update the screen based on the frame buffer.
void updateScreen(){
	SDL_Surface *winSurface = SDL_GetWindowSurface(win);
	for(int i = 0; i < SCRNHEIGHT; i++){
		for(int j = 0; j < SCRNLEN; j++){
			drawPixel(j, i, PIXELSIZE, frameBuffer[i][j], winSurface);
		}
	}
	SDL_FreeSurface(winSurface);
	winSurface = NULL;
}
//Updates the pixel in the frame buffer.
//0 if a collision occured, 1 otherwise.
bool updatePixelInFrameBuffer(int x, int y, bool val){
	//Guarantee the resultant boolean is ALWAYS 1 or 0
	bool collision = ( frameBuffer[y][x] && val );
	frameBuffer[y][x] = (frameBuffer[y][x] ^ val);
	return collision;
}


//Transforms the relative screen coordinates to the window's actual coordinates, then draws a monochrome pixel.
//Draw a nxn sized pixel. n is a constant that determines the size of a "pixel".
//Assume all pixels are stored in three bytes, one for each color (R, B, G)
void drawPixel(int x, int y, int n, bool val, SDL_Surface *surf){
	frameBuffer[y][x] = val;
	//Starting from origin, draw.
		//Draw start:x*(size of a pixel), y*(size of pixel)
		//Within range of this
	//Get how colors are stored in the surface.
	SDL_PixelFormat* fmt = surf->format;
	//Get start draw position.
	int location = (surf->pitch * y * n) + (n * x * fmt->BytesPerPixel);
	SDL_LockSurface(surf);
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			//Origin + rows of bytes from the new location + pixels away from the new location.
			int currentPixelLocation = location + (surf->pitch * i) + (fmt->BytesPerPixel * j);
			//Assumes 3 byte pixel.
			if(val >= true){	//Booleans are set to 1 or greater for true
				*( (uint8_t*)(surf->pixels) + currentPixelLocation + 0 ) = 255;
				*( (uint8_t*)(surf->pixels) + currentPixelLocation + 1 ) = 255;
				*( (uint8_t*)(surf->pixels) + currentPixelLocation + 2 ) = 255;
			}else{
				*( (uint8_t*)(surf->pixels) + currentPixelLocation + 0 ) = 0;
				*( (uint8_t*)(surf->pixels) + currentPixelLocation + 1 ) = 0;
				*( (uint8_t*)(surf->pixels) + currentPixelLocation + 2 ) = 0;
			}
		}
	}
	SDL_Surface *winSurface = SDL_GetWindowSurface(win);
	SDL_BlitSurface(surf, NULL, winSurface, NULL);
	SDL_UpdateWindowSurface(win);
	SDL_FreeSurface(winSurface);
	winSurface = NULL;
}
//Cleans up resources used for the screen.
void deleteScreen(){
	SDL_DestroyWindow(win);
	win = NULL;
	SDL_Quit();
}
