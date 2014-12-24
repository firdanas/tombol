/* BISMILLAH			
 * based on Sarjiya project (github.com/imakin/sarjiya) 
*/
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "lcd_lib.h"
#include <avr/eeprom.h>
#include "CharMenu.h"

#define elif else if


///~ #define EepromWrite(_BLOCK,_VALUE)	eeprom_write_byte((uint8_t*)(_BLOCK),_VALUE) 
///Use update instead of write
#define EepromWrite(_BLOCK,_VALUE)	eeprom_update_byte((uint8_t*)(_BLOCK),_VALUE)
#define EepromRead(_BLOCK)	eeprom_read_byte((uint8_t*)(_BLOCK))

void Init(void);




//just value them with unique number as effective as possible
#define STATE_MENU										0 //root should be 0
#define STATE_HANDS_MENU								1
#define STATE_HANDS_ABDUCADDUC_MENU						2
#define STATE_HANDS_ABDUCADDUC_SPEED1					3
#define STATE_HANDS_ABDUCADDUC_SPEED2					4
#define STATE_HANDS_ABDUCADDUC_SPEED3					5
#define STATE_HANDS_FLEXEXT_MENU						6
#define STATE_HANDS_FLEXEXT_SPEED1						7
#define STATE_HANDS_FLEXEXT_SPEED2						8
#define STATE_HANDS_FLEXEXT_SPEED3						9
#define STATE_HANDS_ROTATION_MENU						10
#define STATE_HANDS_ROTATION_SPEED1						11
#define STATE_HANDS_ROTATION_SPEED2						12
#define STATE_HANDS_ROTATION_SPEED3						13
#define STATE_FEET_MENU									14
#define STATE_FEET_DORSIPLANTAR_MENU					15
#define STATE_FEET_DORSIPLANTAR_SPEED1					16
#define STATE_FEET_DORSIPLANTAR_SPEED2					17
#define STATE_FEET_DORSIPLANTAR_SPEED3					18

void ActFeetDorsiplantarS1(void);

int main(void)
{
	Init();	
	gCursor = 1;
	gState = STATE_MENU;
	while(1)
	{
		CharMenuDraw();
	}
}

void Init()
{
	LCDinit();
	LCDclr();
	LCDGotoXY(0,0);
	DDRC=0b11100000;
	PORTC=0b00001111;
	
	CharMenuInit();
	MenuMain[STATE_HANDS_MENU].cursorNum = 1;
	MenuMain[STATE_HANDS_MENU].menuText = (uint8_t*)"Hands           ";
	MenuMain[STATE_HANDS_MENU].numOfChildren = 3;
	MenuMain[STATE_HANDS_MENU].parentIndex = 0;
	MenuMain[STATE_HANDS_ABDUCADDUC_MENU].cursorNum = 1;
	MenuMain[STATE_HANDS_ABDUCADDUC_MENU].menuText = (uint8_t*)"Abduc-Adduc     ";
	MenuMain[STATE_HANDS_ABDUCADDUC_MENU].numOfChildren = 3;
	MenuMain[STATE_HANDS_ABDUCADDUC_MENU].parentIndex = STATE_HANDS_MENU;
	MenuMain[STATE_HANDS_FLEXEXT_MENU].cursorNum = 2;
	MenuMain[STATE_HANDS_FLEXEXT_MENU].menuText = (uint8_t*)"Flexion-Ext.    ";
	MenuMain[STATE_HANDS_FLEXEXT_MENU].numOfChildren = 3;
	MenuMain[STATE_HANDS_FLEXEXT_MENU].parentIndex = STATE_HANDS_MENU;
	MenuMain[STATE_HANDS_ROTATION_MENU].cursorNum = 3;
	MenuMain[STATE_HANDS_ROTATION_MENU].menuText = (uint8_t*)"Rotation        ";
	MenuMain[STATE_HANDS_ROTATION_MENU].numOfChildren = 3;
	MenuMain[STATE_HANDS_ROTATION_MENU].parentIndex = STATE_HANDS_MENU;
	
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED1].cursorNum = 1;
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED1].menuText = (uint8_t*)"Abduc-Adduc S1  ";
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED1].numOfChildren = 0;
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED1].parentIndex = STATE_HANDS_ABDUCADDUC_MENU;
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED2].cursorNum = 2;
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED2].menuText = (uint8_t*)"Abduc-Adduc S2  ";
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED2].numOfChildren = 0;
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED2].parentIndex = STATE_HANDS_ABDUCADDUC_MENU;
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED3].cursorNum = 3;
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED3].menuText = (uint8_t*)"Abduc-Adduc S3  ";
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED3].numOfChildren = 0;
	MenuMain[STATE_HANDS_ABDUCADDUC_SPEED3].parentIndex = STATE_HANDS_ABDUCADDUC_MENU;
	
	MenuMain[STATE_HANDS_FLEXEXT_SPEED1].cursorNum = 1;
	MenuMain[STATE_HANDS_FLEXEXT_SPEED1].menuText = (uint8_t*)"Flex-Ext Speed 1";
	MenuMain[STATE_HANDS_FLEXEXT_SPEED1].numOfChildren = 0;
	MenuMain[STATE_HANDS_FLEXEXT_SPEED1].parentIndex = STATE_HANDS_FLEXEXT_MENU;
	MenuMain[STATE_HANDS_FLEXEXT_SPEED2].cursorNum = 2;
	MenuMain[STATE_HANDS_FLEXEXT_SPEED2].menuText = (uint8_t*)"Flex-Ext Speed 2";
	MenuMain[STATE_HANDS_FLEXEXT_SPEED2].numOfChildren = 0;
	MenuMain[STATE_HANDS_FLEXEXT_SPEED2].parentIndex = STATE_HANDS_FLEXEXT_MENU;
	MenuMain[STATE_HANDS_FLEXEXT_SPEED3].cursorNum = 3;
	MenuMain[STATE_HANDS_FLEXEXT_SPEED3].menuText = (uint8_t*)"Flex-Ext Speed 3";
	MenuMain[STATE_HANDS_FLEXEXT_SPEED3].numOfChildren = 0;
	MenuMain[STATE_HANDS_FLEXEXT_SPEED3].parentIndex = STATE_HANDS_FLEXEXT_MENU;
	
	MenuMain[STATE_HANDS_ROTATION_SPEED1].cursorNum = 1;
	MenuMain[STATE_HANDS_ROTATION_SPEED1].menuText = (uint8_t*)"Rotation Speed 1";
	MenuMain[STATE_HANDS_ROTATION_SPEED1].numOfChildren = 0;
	MenuMain[STATE_HANDS_ROTATION_SPEED1].parentIndex = STATE_HANDS_ROTATION_MENU;
	MenuMain[STATE_HANDS_ROTATION_SPEED2].cursorNum = 2;
	MenuMain[STATE_HANDS_ROTATION_SPEED2].menuText = (uint8_t*)"Rotation Speed 2";
	MenuMain[STATE_HANDS_ROTATION_SPEED2].numOfChildren = 0;
	MenuMain[STATE_HANDS_ROTATION_SPEED2].parentIndex = STATE_HANDS_ROTATION_MENU;
	MenuMain[STATE_HANDS_ROTATION_SPEED3].cursorNum = 3;
	MenuMain[STATE_HANDS_ROTATION_SPEED3].menuText = (uint8_t*)"Rotation Speed 3";
	MenuMain[STATE_HANDS_ROTATION_SPEED3].numOfChildren = 0;
	MenuMain[STATE_HANDS_ROTATION_SPEED3].parentIndex = STATE_HANDS_ROTATION_MENU;
	
	
	
	MenuMain[STATE_FEET_MENU].cursorNum = 2;
	MenuMain[STATE_FEET_MENU].menuText = (uint8_t*)"Feet            ";
	MenuMain[STATE_FEET_MENU].numOfChildren = 3;
	MenuMain[STATE_FEET_MENU].parentIndex = 0;
	
	MenuMain[STATE_FEET_DORSIPLANTAR_MENU].cursorNum = 1;
	MenuMain[STATE_FEET_DORSIPLANTAR_MENU].menuText = (uint8_t*)"Dorsi-Plantar   ";
	MenuMain[STATE_FEET_DORSIPLANTAR_MENU].numOfChildren = 3;
	MenuMain[STATE_FEET_DORSIPLANTAR_MENU].parentIndex = 2;
	
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED1].cursorNum = 1;
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED1].menuText = (uint8_t*)"Dorsi-Plantar S1";
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED1].numOfChildren = 0;
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED1].parentIndex = STATE_FEET_DORSIPLANTAR_MENU;
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED1].actFunction = &ActFeetDorsiplantarS1;
	
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED2].cursorNum = 2;
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED2].menuText = (uint8_t*)"Dorsi-Plantar S2";
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED2].numOfChildren = 0;
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED2].parentIndex = STATE_FEET_DORSIPLANTAR_MENU;
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED3].cursorNum = 3;
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED3].menuText = (uint8_t*)"Dorsi-Plantar S3";
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED3].numOfChildren = 0;
	MenuMain[STATE_FEET_DORSIPLANTAR_SPEED3].parentIndex = STATE_FEET_DORSIPLANTAR_MENU;
	
	CharMenuRelink();


}

void DraftMenuCommon(uint16_t STATE_NUMBER,char* TEXT)
{
	if (gState != GetParent(STATE_NUMBER))
		return;
	if (gCursor == GetCursor(STATE_NUMBER))				
	{
		LCDGotoXY(0,0);
		LCDstring((uint8_t*)(TEXT),16);
		uint8_t action = ButtonRead();
		if (action == BUTTON_ENTER_DOWN)
			gState = STATE_NUMBER;
		else if (action == BUTTON_NEXT_DOWN)
			gCursor++;
		else if (action == BUTTON_PREV_DOWN)
		{
			gCursor--;
		}
		else if (action == BUTTON_BACK_DOWN)
		{
			gState = GetParent(GetParent(STATE_NUMBER));
			gCursor = GetCursor(GetParent(STATE_NUMBER));
			return;
		}
	}
	ReStrainScroll();
	PrintScroll(gCursor,gScrollMax);
}


void ActFeetDorsiplantarS1(){
		LCDGotoXY(0,0);
		LCDstring((uint8_t*)("BA              "),16);
	
}
