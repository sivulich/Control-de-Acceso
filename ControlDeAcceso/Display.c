#include "Display.h"
#include <ctype.h>
#include "MuxKbDs.h"
int dispInit()
{
	int ret=muxKbDsInit();
	if(ret==1)
	{
		print(' ',0);
		print(' ',1);
		print(' ',2);
		print(' ',3);
	}
	return ret;
}

//Tiene que poner 1 0 en a b c d e f g en la posici�n pos
static void writeToDisp(unsigned char c, int pos) {
	//SOlo hay que shiftear el char para saber el valor que escribir
	muxKbDsWriteDisplay(c, pos);
}

static const unsigned char nums[] = {
    //a,b,c,d,e,f,g
	0b1111110 , //0
	0b0110000 , //1
	0b1101101 , //2
 	0b1111001 , //3
    0b0110011 , //4
    0b1011011 , //5
    0b1011111 , //6
    0b1110000 , //7
    0b1111111 , //8
    0b1110011   //9
};
static const unsigned char letters[] = {
	0x77, //A
	0x7D, //a
	0x7F,
	0x1F,
	0x4E,
	0x0D,
	0x7E,
	0x3D,
	0x4F,
	0x6f,
	0x47,
	0x47,
	0x5E,
	0x7B,
	0x37,
	0x17,
	0x30,
	0x10,
	0x3C,
	0x38,
	0x37,
	0x17,
	0x0E,
	0x06,
	0x55,
	0x55,
	0x15,
	0x15,
	0x7E,
	0x1D,
	0x67,
	0x67,
	0x73,
	0x73,
	0x77,
	0x05,
	0x5B,
	0x5B,
	0x46,
	0x0F,
	0x3E,
	0x1C,
	0x27,
	0x23,
	0x3F,
	0x2B,
	0x25,
	0x25,
	0x3B,
	0x33,
	0x6D,
};
void print(char c, int pos)
{
	if (0 <= pos && pos <= 3)
	{
		if ('0' <= c && c <= '9')
			writeToDisp(nums[c - '0'], pos);

		else
		{
			
			if ('a'<=c && c<='z')
				writeToDisp(letters[2 * (c - 'a')+1], pos);
			else if ('A'<=c && c<='Z')
				writeToDisp(letters[2 * (c - 'A')], pos);
			else if (c == ' ')
				writeToDisp(0, pos);
			else if (c== '*')
				writeToDisp(0b0000001,pos);
		}
	}
}
