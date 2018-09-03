#include "Game.h"
#include "MuxKbDs.h"
#include "Display.h"
#include "Keyboard.h"
#include "Clock.h"
#include "Leds.h"
#include <stdlib.h>
#include <stdint.h>
void gameRun(void)
{
	uint8_t game[4][3] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 },{ 0,1,0 } };
	int lives = 1,pos=1,led=0;
	double diff = 1000;
	unsigned long long t = 0;
	clock_t adv=getTime(),show=getTime();
	ledsOff();
	srand(getTime());
	while (lives)
	{
		if (((double)getTime() - show) / CLOCKS_PER_SEC > 0.2)
		{
			for (int i = 0; i < 4; i++)
				muxKbDsWriteDisplay(game[i][0] << 6 | game[i][1] | game[i][2] << 3, i);
			show = getTime();
		}
		if (((double)getTime() - adv) / CLOCKS_PER_SEC > diff / CLOCKS_PER_SEC)
		{
			if (diff > 300)
				diff-=10;
			for (int i = 2; i >= 0; i--)
			{
				for (int j = 0; j < 3; j++)
				{
					if (game[i][j] == 1 && game[i + 1][j] == 1)
						lives--;
					if(!(i+1==3 && j==pos))
						game[i+1][j] = game[i][j];
					game[i][j]=0;
				}
			}
			if (t % 2 == 0)
			{
				int obs=rand() % 4;
				if(obs==0)
					game[0][1] = 1;
				else if(obs==1)
				{
					game[0][0] = 1;
					game[0][2] = 1;
				}
				else if(obs==2)
				{
					game[0][1] = 1;
					game[0][2] = 1;
				}
				else if(obs==3)
				{
					game[0][1] = 1;
					game[0][0] = 1;
				}

			}
			ledsBlue(led);
			ledsBoard(led);
			led^=1;
			t++;
			adv=getTime();
		}
		if (kbHit())
		{
			char c = readKb();
			if (c == '8' && pos > 0)
			{
				game[3][pos] = 0;
				pos--;
				if(game[3][pos]==1)
					lives--;
				game[3][pos] = 1;
			}
			else if (c == '2' && pos < 2)
			{
				game[3][pos] = 0;
				pos++;
				if(game[3][pos]==1)
					lives--;
				game[3][pos] = 1;
			}
		}
	}
	ledsOff();
	ledsRed(1);
	print('L',3);
	print('O',2);
	print('S',1);
	print('S',0);
	while(readKb()!='#');
	ledsOff();
	print('0'+t/1000,3);
	print('0'+(t/100)%10,2);
	print('0'+(t/10)%10,1);
	print('0'+t%10,0);
	while(readKb()!='#');
	print(' ',3);
	print(' ',2);
	print(' ',1);
	print(' ',0);

}
