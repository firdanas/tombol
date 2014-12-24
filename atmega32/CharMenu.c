/**
 * (c) 2014-2015 Izzulmakin
 * made from 23 Dec 2014 based on makin.h (github.com/imakin/sarjiya)
 * released under LGPL license, derivative works are bind to use LGPL license
 */
#include "CharMenu.h"
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include "lcd_lib.h"
#include <util/delay.h>


void DrawNumber(uint16_t bil, uint8_t x, uint8_t y,uint8_t _c)
{
	uint8_t pjg;
	char lcdchar[30];
			LCDGotoXY(x,y);
			if (_c!=0)
			{
				
				for (uint8_t i=1; i<=_c; i++)
				{
					LCDstring((uint8_t*)" ",1);
				}
				LCDGotoXY(x,y);
			}
			snprintf(lcdchar,15, "%d",bil);
			pjg = strlen(lcdchar);
			LCDstring((uint8_t*)lcdchar,pjg);
	
}
///
void DrawNumberCPos(uint16_t bil, uint8_t _c)
{
	uint8_t pjg;
	char lcdchar[30];
	if (_c!=0)
	{
		
		for (uint8_t i=1; i<=_c; i++)
		{
			LCDstring((uint8_t*)" ",1);
		}
	}
	snprintf(lcdchar,15, "%d",bil);
	pjg = strlen(lcdchar);
	LCDstring((uint8_t*)lcdchar,pjg);
	
}

///Clears block with whitespace replacing
void LcdDelete(uint8_t xawal, uint8_t xakhir, uint8_t _Y)
{
	LCDGotoXY(xawal,_Y);
	for (uint8_t i=xawal;i<=xakhir;i++)
	{
		LCDstring((uint8_t*)" ",1);
	}
}


uint8_t ButtonEnter()
{
	return isclear(BUTTON_ENTER_PIN,BUTTON_ENTER_DOWN);
}
uint8_t ButtonBack()
{
	return isclear(BUTTON_BACK_PIN,BUTTON_BACK_DOWN);
}
uint8_t ButtonNext()
{
	return isclear(BUTTON_NEXT_PIN,BUTTON_NEXT_DOWN);
}
uint8_t ButtonPrev()
{
	return isclear(BUTTON_PREV_PIN,BUTTON_PREV_DOWN);
}



void ButtonWait()
{
	while (isset(BUTTON_ENTER_PIN,BUTTON_ENTER_DOWN) && isset(BUTTON_BACK_PIN,BUTTON_BACK_DOWN) && isset(BUTTON_NEXT_PIN,BUTTON_NEXT_DOWN) && isset(BUTTON_PREV_PIN,BUTTON_PREV_DOWN));
}
uint8_t ButtonIsPressed()
{
	return (isclear(BUTTON_ENTER_PIN,BUTTON_ENTER_DOWN) || isclear(BUTTON_BACK_PIN,BUTTON_BACK_DOWN) || isclear(BUTTON_NEXT_PIN,BUTTON_NEXT_DOWN) || isclear(BUTTON_PREV_PIN,BUTTON_PREV_DOWN));
}
uint8_t ButtonIsNotPressed()
{
	return (isset(BUTTON_ENTER_PIN,BUTTON_ENTER_DOWN) || isset(BUTTON_BACK_PIN,BUTTON_BACK_DOWN) || isset(BUTTON_NEXT_PIN,BUTTON_NEXT_DOWN) || isset(BUTTON_PREV_PIN,BUTTON_PREV_DOWN));
}
uint8_t ButtonRead()
{
	uint8_t output;
	ButtonWait();
	if (isclear(BUTTON_ENTER_PIN,BUTTON_ENTER_DOWN))
		output = BUTTON_ENTER_DOWN;
	else if (isclear(BUTTON_BACK_PIN,BUTTON_BACK_DOWN))
		output = BUTTON_BACK_DOWN;
	else if (isclear(BUTTON_NEXT_PIN,BUTTON_NEXT_DOWN))
		output = BUTTON_NEXT_DOWN;
	else 
		output = BUTTON_PREV_DOWN;
	_delay_ms(100);
	return (output);
}

void CharMenuInit()
{
	MenuMain[0].cursorNum = 1;
	MenuMain[0].menuText = (uint8_t*)"";//main menu text will not be used
	MenuMain[0].numOfChildren = MAIN_MENU_NUMBER_OF_CHILDREN;
	MenuMain[0].parentIndex = 0;
	MenuMain[0].actFunction = 0;
}

void CharMenuRelink()
{
	uint8_t x=0;
	for (x=0;x<TOTAL_MENU;x++)
	{
		MenuMain[x].actFunction = 0;
	}
}

void CharMenuDraw()
{
	if (MenuMain[gState].actFunction!=0)
	{
		MenuMain[gState].actFunction();
		_delay_ms(500);
		while (!ButtonBack());
		gCursor = MenuMain[gState].cursorNum;
		gState = MenuMain[gState].parentIndex;
		return;
	}
	uint8_t x=0;
	///Catch the current menu and cursor, x start from 1 (not main menu)
	for (x=1;x<TOTAL_MENU;x++)
	{
		///current state is gState, find children of MenuMain[gState]
		if ((MenuMain[x].parentIndex == gState) && ((MenuMain[x].cursorNum) == gCursor))
		{
			///Draw the current menu at current cursor
			LCDGotoXY(0,0);
			LCDstring((uint8_t*)(MenuMain[x].menuText),16);
			PrintScroll(gCursor,MenuMain[MenuMain[x].parentIndex].numOfChildren);
			
			///wait signal
			uint8_t action = ButtonRead();
			if (action == BUTTON_ENTER_DOWN)
			{
				gState = x;
				gCursor = 1;
				//~ DrawNumber(gState,0,1,3);
				//~ uint8_t ii;
				//~ for (ii=1;ii<TOTAL_MENU;ii++)
				//~ {
					//~ if ((MenuMain[ii].parentIndex==x) && (MenuMain[ii].cursorNum==1))
					//~ {
						//~ gCursor = 1;
						//~ gState = ii;
						//~ break;
					//~ }
				//~ }
				//~ _delay_ms(1000);
			}
			else if (action == BUTTON_NEXT_DOWN)
			{
				gCursor++;
				if (gCursor>MenuMain[MenuMain[x].parentIndex].numOfChildren)
					gCursor = 1;
			}
			else if (action == BUTTON_PREV_DOWN)
			{
				gCursor--;
				if (gCursor<1)
					gCursor = MenuMain[MenuMain[x].parentIndex].numOfChildren;
			}
			else if (action == BUTTON_BACK_DOWN)
			{
				gState = MenuMain[gState].parentIndex;
				//~ gCursor = MenuMain[gState].cursorNum;
				gCursor = 1;
				//~ return;
			}
			return;
		}
	}
}

uint8_t GetCursor(uint16_t numstate)
{
	if (numstate>=20)
		return (numstate%10);
	return numstate;
}
uint8_t GetParent(uint16_t numstate)
{
	if (numstate<20)
		return 0;///its parent is menu
	return (numstate/10);
}
uint8_t GetChild(uint8_t parent, uint8_t childid)
{
	return (parent*10)+childid;
}

void ReStrainScroll()
{
	if (gCursor<1)
	{
		gCursor = gScrollMax;
	}
	else if (gCursor>gScrollMax)
	{
		gCursor = 1;
	}
}

void PrintScroll(uint8_t sNum, uint8_t sMax)
{
	uint8_t mulai = 0;
	uint8_t nScroll = 1;
	LcdDelete(0,15,1);
	mulai = 8-(sMax/2);
	LCDGotoXY(mulai,1);
	for (nScroll = 1; nScroll<=sMax; nScroll++)
	{
		if (nScroll == sNum)
		{
			LCDstring((uint8_t*)"#",1);
		}
		else
		{
			LCDstring((uint8_t*)"-",1);
		}
	}
}
