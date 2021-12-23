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
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
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
	int i=0;
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();
	
  
	TP_Init();
	//TouchPanel_Calibrate();
	
	LCD_Clear(Black);

	ball.posX = 7;
	ball.posY = 270;
	ball.h_direc = -1;
	ball.v_direc = -1;
    ball.h_speed = 3;
	ball.v_speed = 1;
	// prevBall_x_position= 100;
	// prevBall_y_position= 120;


	//draw ball
	// for(i=0; i<5; i++) {
	// 	LCD_DrawLine(100, 120+i, 104, 120+i, Blue);
	// }
	for(i=0; i<5; i++) {
		LCD_DrawLine(ball.posX, ball.posY+i, ball.posX+4, ball.posY+i, Green);
	}
	
	// TP_DrawPoint(50, 70);
	// TP_DrawPoint(51, 71);

	// draw paddle (50)
	paddle.posX = 10;
	paddle.posY = 277;
	for(i=0; i<10; i++) {
		LCD_DrawLine(paddle.posX, paddle.posY+i, paddle.posX+49, paddle.posY+i, Green);
	}
	
	//draw walls
	for(i=0; i<5; i++) {
		//left wall
		LCD_DrawLine(i, 0, i, 276, Red);
		//right wall
		LCD_DrawLine(235+i, 0, 235+i, 276, Red);
		//roof wall
		LCD_DrawLine(5, i, 235, i, Red);
	}
	


	//GUI_Text(0, 280, (uint8_t *) " touch here : 1 sec to clear  ", Blue, White);
	
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	init_timer(0, 0, 0, 3, 0x00003F7A); 						    /* 8us * 25MHz = 200 ~= 0xC8 */ //XXX tempo di refresh
	

	
	enable_timer(0);
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
