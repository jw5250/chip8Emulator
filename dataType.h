#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED

//Assumes machine running this is little endian.

typedef unsigned char byte;

typedef union {
	unsigned short WORD;
	struct{


			//As addresses increase in big endian system, byte significance increases.
			//Lower byte address, then upper byte address
			byte LOWER;
			byte UPPER;

	}BYTE;
	//Chip 8 is big endian.
} word;
#endif
