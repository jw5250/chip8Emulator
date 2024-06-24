#include "loader.h"
//LIL_ENDIAN will change loadfile to load in little endian ROM
#include "dataType.h"
#include "cpu.h"
#include "memory.h"
#include <stdio.h>
//Loader doesn't consider how bytes are added into the program.
//Needs to be fixed so it handles odd sized files as well.

//Should add a translator from chip8 code to hex opcodes (in string form) in order to read each instruction.
int loadFile(char* title){
    //Open a file in binary and read mode.
    FILE* chip8File = fopen(title, "rb");
    if(chip8File == NULL){
        fprintf(stderr, "File doesn't exist or couldn't be opened.\n");
        return -1;
    }
    FILE* newFile = fopen("instructions","wb");
    if(memoryExists() == 0){
        fprintf(stderr, "Memory doesn't exist. Please initialize memory first\n");
    }
    word instruction;
    size_t bytesRead;
    word addr;
    addr.WORD = STARTADDR;
    byte mem;
    word positionFromInstruction;
    positionFromInstruction.WORD = sizeof(word)-1;
    while(1){
        //Reports state from that of the most recent IO operation. If fread errors out because it reached the end of file, only then it will report that the end of the file was reached.
        bytesRead = fread(&mem, sizeof(byte), 1, chip8File);
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
		//Ex. Loading in chip8 ROM from big endian file
        #ifdef BIG_ENDIAN
            writeMemory(addr, mem);//Upper byte in here.
            fprintf(newFile, "Address %x: %x\n", addr.WORD, mem);
            addr.WORD++;
            //Add offset to this and add.
            /*
                byte BUFF_swap;
                BUFF_swap = instruction.BYTE.LOWER;
                instruction.BYTE.LOWER = instruction.BYTE.UPPER;
                instruction.BYTE.UPPER = BUFF_swap;
            */
        #else
            //
            word buffAddr;
            buffAddr.WORD = positionFromInstruction.WORD + addr.WORD;
            writeMemory(buffAddr, mem);

            //If the last byte written was at addr.WORD...
            if(positionFromInstruction.WORD == 0){
                for(word i = addr; i.WORD < addr.WORD + sizeof(word);i.WORD++){
                    fprintf(newFile, "Address %x: %x\n", i.WORD, readMemory(i.WORD));
                }
                addr.WORD += sizeof(word);
                positionFromInstruction.WORD = sizeof(word)-1;
            }
            //Subtract the position from the instruction to invert byte positions.
            positionFromInstruction.WORD--;
            
		#endif
        /*
            writeMemory(addr, instruction.BYTE.UPPER);//Upper byte in here.
            fprintf(newFile, "Address %x: %x\n", addr.WORD, instruction.BYTE.UPPER);
            addr.WORD++;
            writeMemory(addr, instruction.BYTE.LOWER);//Lower byte in here.
            fprintf(newFile, "Address %x: %x\n", addr.WORD, instruction.BYTE.LOWER);
            addr.WORD++;
        */
    }
}