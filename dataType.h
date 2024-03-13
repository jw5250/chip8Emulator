#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED
typedef unsigned char byte;

typedef union {
	unsigned short WORD;
	struct{
		#ifdef LIL_ENDIAN
			byte UPPER;
			byte LOWER;
		#else
			byte LOWER;
			byte UPPER;
		#endif
	}BYTE;

} word;
#endif
