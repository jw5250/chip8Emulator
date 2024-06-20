#include "loader.h"
//LIL_ENDIAN will change loadfile to load in little endian ROM
#include "dataType.h"
#include "cpu.h"
#include "memory.h"
#include <stdio.h>

//When loading in extra garbage, it's always the same contents of the previous word except the lower and upper bytes are swapped.


//Should add a translator from chip8 code to hex opcodes (in string form) in order to read each instruction.
int loadFile(char* title){
    //Open a file in binary and read mode.
    FILE* chip8File = fopen(title, "rb");
    if(chip8File == NULL){
        fprintf(stderr, "File doesn't exist or couldn't be opened.\n");
        return -1;
    }
    FILE* newFile = fopen("instructions","wb");
    word instruction;
    size_t bytesRead;
    word addr;
    addr.WORD = STARTADDR;
    //If memory doesn't exist
    if(memoryExists() == 0){
        fprintf(stderr, "Memory doesn't exist.\n");
        int memoryThere = initMemory();
        if(memoryThere == 0){
            fprintf(stderr, "Couldn't initialize memory\n");
            return 0;
        }
        printf("Memory initialized.\n");
    }
    /*While file is not empty...*/
    while(1){
        bytesRead = fread(&instruction.WORD, sizeof(word), 1, chip8File);
        //If this is not here, fread is somehow still valid. Why?
        //Reports state from that of the most recent IO operation. If fread errors out because it reached the end of file, only then it will report that the end of the file was reached.
        if(feof(chip8File)){
            fclose(chip8File);
            fclose(newFile);
            return 1;
        }else if(ferror(chip8File)){
            fprintf(stderr, "Error with reading chip8File.\n");
            fclose(chip8File);
            fclose(newFile);
            return 1;
        }

        //In case we are running chip8 with little endian memory.
		//Ex. Loading in little endian chip8 ROM
        #ifdef LIL_ENDIAN
            //This works!
			byte BUFF;
            BUFF = instruction.BYTE.LOWER;
            instruction.BYTE.LOWER = instruction.BYTE.UPPER;
            instruction.BYTE.UPPER = BUFF;
		#endif
        writeMemory(addr, instruction.BYTE.UPPER);//Upper byte in here.
        addr.WORD++;
        writeMemory(addr, instruction.BYTE.LOWER);//Lower byte in here.
        addr.WORD++;
        
        //Prints the upper and lower bytes of each instructions, separated by a |.
        fprintf(newFile, "%x|", instruction.BYTE.UPPER);
        fprintf(newFile, "%x\n", instruction.BYTE.LOWER);

        /*
        insert byte read from instruction into chip 8 memory, increment starting from start address.
        */
    }
}