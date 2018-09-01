
/***************************
 * INCLUDE HEADER FILES
 **************************/
#include "gpio.h"
#include "math.h"
#include "Mcr.h"
#include <stdint.h>
#define MCR_TRACK2_TOTAL_CHARS 40
#define MCR_BITS_PER_CHAR 5
#define MCR_DIGITS_BEFORE_FS 20
#define MCR_DIGITS_BEFORE_ES 38

#define MCR_SS ';'
#define MCR_ES '?'
#define MCR_FS '='

/***************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 **************************/
void handleEnable(void);
void saveCardNumber(void);
void initVariables(void);
uint8_t checkDataFormat(void);

void getTransition(void);
void getData(void);
void tryFindingSS(uint8_t dataIn);
void pushFront(uint8_t *array, uint8_t len,uint8_t data);
void shiftRArray(uint8_t *array, uint8_t len); //Shiftea de manera que [4] pasa a ser [3] y así
uint8_t getAsciiSymbol(uint8_t * arrayDigit);

void shiftLCardNumberBuffer(void);
void saveDigitInBuffer(uint8_t *array, uint8_t index);
uint8_t digitIsOk(uint8_t *rawDataBuffer);
uint8_t checkCardParity(void);
uint8_t checkDigitParity(uint8_t *array);
uint8_t checkParity(uint8_t *array, uint8_t len,uint8_t parity);

uint8_t cardNumberAsciiBuffer[MCR_CARD_MEMORY][MCR_CARD_VALUE_LENGTH]; //Aqui se guarda el ID de la card en ASCII
uint8_t cardDigitsBuffer[MCR_TRACK2_TOTAL_CHARS][MCR_BITS_PER_CHAR],rawDataBuffer[MCR_BITS_PER_CHAR];
uint8_t cardNumbersStored,SSFound,finishedReadingCard,dataOverflow,digitsRecieved, bitCount,transitionOcurred;
/***************************
 ***************************
                        GLOBAL FUNCTION DEFINITIONS
 ***************************
 **************************/
static int init = 0;
int mcrInit(void)
{
	if (init == 0)
	{
#ifndef _WIN32
		pinMode(MCR_ENABLE, INPUT);
		pinMode(MCR_CLK, INPUT);
		pinMode(MCR_DATA, INPUT);
		pinConfigureIRQ(MCR_CLK, GPIO_IRQ_MODE_RISING_EDGE, getData);
		pinConfigureIRQ(MCR_ENABLE, GPIO_IRQ_MODE_BOTH_EDGES, handleEnable);
		pinConfigureIRQ(MCR_DATA, GPIO_IRQ_MODE_FALLING_EDGE, getTransition);
		cardNumbersStored = 0;
#endif
		init = 1;
	}
	return init;
}

int hasCardValue()
{
	return (cardNumbersStored&&1);
}

void getLastCard(char* dest)
{
	if (cardNumbersStored > 0)
	{
		for (uint8_t j = 0; j<MCR_CARD_VALUE_LENGTH; j++)
		{
			if (cardNumberAsciiBuffer[0][j] != '=')
				dest[j] = cardNumberAsciiBuffer[0][j];
			else
			{
				dest[j] = 0;
				break;
			}
		}
		dest[MCR_CARD_VALUE_LENGTH] = 0;
		shiftLCardNumberBuffer();
		cardNumbersStored--;
	}
}


/***************************
 ***************************
                        LOCAL FUNCTION DEFINITIONS
 ***************************
 **************************/
void handleEnable(void)
{
	uint8_t reading= !digitalRead(MCR_ENABLE);
	if(reading==1)
		initVariables();
	else if(finishedReadingCard && !dataOverflow && checkCardParity())
		saveCardNumber();
}
void initVariables(void)
{
	SSFound=0;
	for (uint8_t i=0;i<MCR_BITS_PER_CHAR;i++)
		rawDataBuffer[i]=0;
	finishedReadingCard=0;
	dataOverflow=0;
	digitsRecieved=0;
	bitCount=0;
	transitionOcurred=0;
}
void getTransition(void)
{
	transitionOcurred=1;
}
void getData(void)
{
	uint8_t dataIn = transitionOcurred;
	transitionOcurred=0;
	if(!SSFound) //Se busca cuando empieza a transmitir datos
		tryFindingSS(dataIn);
	else //Una vez encontrado, se buscan obtener 5 bits
	{
		bitCount++;
		pushFront(rawDataBuffer, MCR_BITS_PER_CHAR,dataIn); //Se van guardando en un buffer hasta tener la palabra de 5 bits
		if(bitCount==MCR_BITS_PER_CHAR && digitIsOk(rawDataBuffer))
		{

			if((digitsRecieved < MCR_TRACK2_TOTAL_CHARS-1 &&checkDigitParity(rawDataBuffer)) || digitsRecieved == MCR_TRACK2_TOTAL_CHARS-1)
			{
				saveDigitInBuffer(rawDataBuffer, digitsRecieved);
				digitsRecieved++;
				if( digitsRecieved == MCR_TRACK2_TOTAL_CHARS-1)
					finishedReadingCard=1;
			}
			bitCount=0;
		}
	}
}
uint8_t digitIsOk(uint8_t * rawDataBuffer)
{
	uint8_t retVal=0;
	if((digitsRecieved < MCR_DIGITS_BEFORE_ES)  || (digitsRecieved == MCR_TRACK2_TOTAL_CHARS-1)) //Datos que pueden tomar cualquier valor
		retVal=1;
	else if(digitsRecieved == MCR_DIGITS_BEFORE_ES && (getAsciiSymbol(rawDataBuffer) == MCR_ES))
		retVal=1;
	return retVal;
}
void tryFindingSS(uint8_t dataIn)
{
	static uint8_t counter;
	counter++;
	pushFront(rawDataBuffer, MCR_BITS_PER_CHAR,dataIn);
	if(checkDigitParity(rawDataBuffer) && getAsciiSymbol(rawDataBuffer) == MCR_SS)
	{
		SSFound=1;
		saveDigitInBuffer(rawDataBuffer, digitsRecieved);
		digitsRecieved++;
	}
}
void saveCardNumber()
{
	if(cardNumbersStored == MCR_CARD_MEMORY) //Si el buffer circular está lleno, se shiftea a la izq todas las cards stored
	{
		shiftLCardNumberBuffer();
		cardNumbersStored--;
	}
	for(uint8_t j=0;j<MCR_CARD_VALUE_LENGTH;j++)
		cardNumberAsciiBuffer[cardNumbersStored][j]=getAsciiSymbol(cardDigitsBuffer[j+1]);
	cardNumbersStored++;
}
void shiftLCardNumberBuffer(void)
{
	for(uint8_t i=0;i<MCR_CARD_MEMORY;i++)
		for(uint8_t j=0;j<MCR_CARD_VALUE_LENGTH;j++)
			cardNumberAsciiBuffer[i][j]=cardNumberAsciiBuffer[i+1][j];
}
void pushFront(uint8_t *array, uint8_t len,uint8_t data)
{
	shiftRArray(array,len);
	array[0]=data;
}

void shiftRArray(uint8_t *array, uint8_t len)
{
	uint8_t i=len-1;
	uint8_t quit=0;
	while(!quit)
	{
		array[i] =array[i-1];
		i--;
		if(i==0)
			quit=1;
	}

}

void saveDigitInBuffer(uint8_t *array, uint8_t index)
{
	if(index<MCR_TRACK2_TOTAL_CHARS)
	{
		for(uint8_t i=0; i<MCR_BITS_PER_CHAR;i++)
			cardDigitsBuffer[index][i]=array[i];
	}
	else
		dataOverflow=1;
}

uint8_t checkCardParity(void)
{
	uint8_t retVal=0;
	uint8_t cardParity[MCR_BITS_PER_CHAR], counter=0;
	for(uint8_t k=0;k<MCR_BITS_PER_CHAR;k++)
		cardParity[k]=0;
	for(uint8_t i=0;i<MCR_BITS_PER_CHAR;i++)
		for(uint8_t j=0;j<MCR_TRACK2_TOTAL_CHARS-1;j++)
			cardParity[i]+=cardDigitsBuffer[j][i];
	for(uint8_t i=0;i<MCR_BITS_PER_CHAR;i++)
	{
		if( (cardParity[i]%2)==cardDigitsBuffer[MCR_TRACK2_TOTAL_CHARS-1][i] )
			counter++;
	}
	if(counter==MCR_BITS_PER_CHAR)
		retVal=1;
	return retVal;
}
uint8_t checkDigitParity(uint8_t *array)
{
	return checkParity(array+1,MCR_BITS_PER_CHAR-1,array[0]);
}
uint8_t checkParity(uint8_t *array, uint8_t len,uint8_t parity)
{
	uint8_t total=0;
	for (uint8_t i=0;i<len;i++)
		total += array[i];
	return (uint8_t) ((total%2) != parity);
}
uint8_t getAsciiSymbol(uint8_t * arrayDigit)
{
	uint8_t total=0;
	for (uint8_t i=1;i<MCR_BITS_PER_CHAR;i++)
		total += (arrayDigit[i]<< (MCR_BITS_PER_CHAR-i-1));
	total=total + (uint8_t) '0';
	return total;
}
