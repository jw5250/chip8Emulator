#include "keyboard.h"
#include "dataType.h"
#define TOTAL_INPUTS 16

/*
Input diagram (respective to how chip 8 keys are mapped):
	1 2 3 4           1 2 3 C
	q w e r           4 5 6 D
	a s d f mapped to 7 8 9 E
	z x c v           A 0 B F
*/


const SDL_Keycode inputs[TOTAL_INPUTS] = {
	SDLK_x, SDLK_1, SDLK_2, SDLK_3,
	SDLK_q, SDLK_w, SDLK_e, SDLK_a,
	SDLK_s, SDLK_d, SDLK_z, SDLK_c,
	SDLK_4, SDLK_r, SDLK_f, SDLK_v
};

//Keeps track of which inputs are being held down at the moment.
bool inputBoard[TOTAL_INPUTS];

//Keeps track of the input made during the cycle.
byte mostRecentPressed;

void initInput(){
    for(int i = 0; i < TOTAL_INPUTS;i++){
        inputBoard[i] = false;
    }
    mostRecentPressed = EMPTY_KEY;
}
//Takes in a keyboard input and represents it on the input board.
void takeInput(SDL_Keycode c){
    byte encoding = 0;
    while(encoding < TOTAL_INPUTS){
        if(c == inputs[encoding]){
            //printf("Input in:%c\n", inputsCharRep[encoding]);
            inputBoard[encoding] = true;
            mostRecentPressed = encoding;
            return;
        }
        encoding++;
    }
}

void removeInput(SDL_Keycode c){
    byte encoding = 0;
    while(encoding < TOTAL_INPUTS){
        if(c == inputs[encoding]){
            //printf("Input out:%c\n", inputsCharRep[encoding]);
            inputBoard[encoding] = false;
            return;
        }
        encoding++;
    }
}

byte getMostRecentKeyPressed(){
    return mostRecentPressed;

}

//Check if a key is pressed.
bool findKey(byte encode){
    return (inputBoard[encode] == true);
}

void resetRecentPressed(){
    mostRecentPressed = EMPTY_KEY;
}