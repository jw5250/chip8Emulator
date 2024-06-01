#include "loader.h"
//LIL_ENDIAN will change loadfile to load 
#include "dataType.h"

#include "cpu.h"
#include "memory.h"
#include <stdio.h>


//Should add a translator from chip8 code to hex opcodes (in string form) in order to read each instruction.
void loadFile(char* title){
    //Open a file in binary and read mode.
    FILE* chip8File = fopen(title, "rb");
    FILE* newFile = fopen("instructions","wb");
    word instruction;
    size_t bytesRead;
    int addr = STARTADDR;
    /*While file is not empty...*/
    while(!feof(chip8File)){
        bytesRead = fread(&instruction.WORD, sizeof(word), 1, chip8File);
        //In case we are running chip8 with little endian memory.
		//Ex. Loading in little endian chip8 ROM
        #ifdef LIL_ENDIAN
            //This works!
			byte BUFF;
            BUFF = instruction.BYTE.LOWER;
            instruction.BYTE.LOWER = instruction.BYTE.UPPER;
            instruction.BYTE.UPPER = BUFF;
		#endif

        //writeMemory(addr, instruction.UPPER);//Upper byte in here.
        //writeMemory(addr+1, instruction.LOWER);//Lower byte in here.
        //addr += sizeof(word);
        
        //fprintf(newFile, "%x|", instruction.BYTE.UPPER);
        //fprintf(newFile, "%x\n", instruction.BYTE.LOWER);

        /*
        insert byte read from instruction into chip 8 memory, increment starting from start address.
        */    
    }

    fclose(chip8File);
}