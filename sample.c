/****************************************Copyright (c)****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "game/game.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

volatile uint8_t prevBall_x_position;
volatile uint8_t prevBall_y_position;
struct struct_ball ball;
struct struct_paddle paddle;

int main(void)
{
	SystemInit(); /* System Initialization (i.e., PLL)  */

	LCD_Initialization();
	LCD_Clear(Black);
	BUTTON_init();

	//init_RIT(0x004C4B40); /* RIT Initialization 50 msec       	*/


	// init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	// init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	// init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	GAME_init();
	//init_timer(0, 0, 0, 3, 0x00003F7A); /* 8us * 25MHz = 200 ~= 0xC8 */ // XXX tempo di refresh
	init_timer(0, 0, 0, 3, 0x00007EF4); //XXX 1,3 ms
	//todo try 50 ms
	init_RIT(0x004C4B40); /* RIT Initialization 50 msec       	*/
	enable_RIT();		  /* RIT enabled												*/

	// enable_timer(0);

	LPC_SC->PCON |= 0x1; /* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);

	while (1)
	{
		__ASM("wfi");
	}
}

/*********************************************************************************************************
	  END FILE
*********************************************************************************************************/
