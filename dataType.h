#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED

/*
The main datatypes used for memory.

If this program is run on a big endian system, the upper and lower bytes would be swapped. However,

The loader should handle these issues.
*/

//Assumes machine running this is little endian.

typedef unsigned char byte;

typedef union {
	unsigned short WORD;
	struct{
			//Lower byte address, then upper byte address
			byte LOWER;
			byte UPPER;
	}BYTE;
	//Chip 8 is big endian.
} word;
#endif
