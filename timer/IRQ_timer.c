/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../GLCD/GLCD.h"
#include "../game/game.h"
#include <stdlib.h>

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern uint8_t prevBall_x_position;
extern uint8_t prevBall_y_position;

//struct struct_ball ball;
//struct struct_paddle paddle;

void TIMER0_IRQHandler (void)
{
	// Vertical movement  : 1 = Sud,  
	//					   -1 = North
	// Horizontal movement: 1 = East,
	//					   -1 = West  
	// static int previousVerticalDirection = 1;
	// static int previousHorizontalDirection = -1;
	// static int verticalLimit = previousVerticalDirection > 0 ? 278 : 1;
	// static int horizontalLimit = previousHorizontalDirection > 0 ? 255 : 1; 
	// static int previousVerticalSpeed = 1;
	// static int previousHorizontalSpeed = 1;

	// int currentVerticalDirection = previousVerticalSpeed+? previousVerticalDirection
	// int currentVerticalDirection = 

		uint8_t previous_h_speed, previous_v_speed, longer_speed, isColliding;
	  uint8_t delta = 4;
		uint8_t is_game_over = 0;
	  uint8_t i =0;

    while (!is_game_over){
        
        //calcolo movimento
        // h_mov = ball.h_speed * ball.h_direc;
        // v_mov = ball.v_speed * ball.v_direc;
        // h_mov
        // v_mov
        previous_h_speed = ball.h_speed;
        previous_v_speed = ball.v_speed;

        
        longer_speed = ball.v_speed>=ball.h_speed ? ball.v_speed : ball.h_speed;

    
        for(i=0; i<longer_speed; i++) {
            
            //if ball is moving HORIZONTALLY and Horizontal speed is not lapsed
            if (ball.h_direc && ball.h_speed){
                /**@todo HANDLE HORIZONTAL COLLISION WITH WALLS AND PADDLE**/
                isColliding = is_colliding(ball, 'h');
           
                if(isColliding) {
                    ball.h_speed = previous_h_speed;
                    ball.v_speed = previous_v_speed;
                    ball.h_direc = -ball.h_direc;
                    // move_ball(ball);
                      LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
                }
                if(ball.h_direc<0){
                    //MOVE horizontal left
                    //delete horizontal right 
                    LCD_DrawLine(ball.posX+delta, ball.posY, ball.posX+delta, ball.posY+delta, Black);
                    //paint horizontal left
                    LCD_DrawLine(ball.posX+ball.h_direc, ball.posY, ball.posX+ball.h_direc, ball.posY+delta, Green);
                } else {
                    //MOVE horizontal right
                    //delete horizontal left 
                    LCD_DrawLine(ball.posX, ball.posY, ball.posX, ball.posY+delta, Black);
                    //paint horizontal right
                    LCD_DrawLine(ball.posX+ball.h_direc+delta, ball.posY, ball.posX+ball.h_direc+delta, ball.posY+delta, Green);
                }
                //update x
                ball.posX = ball.posX+ball.h_direc;
                //decrement horizontal speed
                ball.h_speed--;
            }
            //if ball is moving VERTICALLY and Vertical speed is not Elapsed
            if (ball.v_direc && ball.v_speed){
                isColliding = is_colligit commit -m ding(ball, 'v');
                if(isColliding) { 
                    ball.h_speed = previous_h_speed;
                    ball.v_speed = previous_v_speed;
                    ball.v_direc = -ball.v_direc;
                    // move_ball(ball);
                     LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
                }
                if(ball.v_direc<0){
                    //MOVE vertical up
                    //delete vertical down 
                    LCD_DrawLine(ball.posX, ball.posY+delta, ball.posX+delta, ball.posY+delta, Black);
                    //paint vertical up
                    LCD_DrawLine(ball.posX, ball.posY+ball.v_direc, ball.posX+delta, ball.posY+ball.v_direc, Green);
                } else {
                    //MOVE vertical down
                    //delete vertical up 
                    LCD_DrawLine(ball.posX, ball.posY, ball.posX+delta, ball.posY, Black);
                    //paint vertical down
                    LCD_DrawLine(ball.posX, ball.posY+ball.v_direc+delta, ball.posX+delta, ball.posY+ball.v_direc+delta, Green);
                }
                //update y
                ball.posY = ball.posY+ball.v_direc;

                is_game_over = ball.posY > 278;
                //decrement vertical speed
                ball.v_speed--;
            }

            //richiamo la funzione se il ciclo � finito senza collisioni
            if (longer_speed-1 == i) {
                ball.h_speed = previous_h_speed;
                ball.v_speed = previous_v_speed;
                // move_ball(ball);
                  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
            }
        }
    }


	// // delete ball
	// int i =0;
	// for(i=0; i<5; i++) {
	// 	LCD_DrawLine(ball.posX, ball.posY+i, ball.posX+4, ball.posY+i, Black);
	// }
	// ball.posX = ball.posX -3;
	// ball.posY = ball.posY +1;
	// // Blackraw ball
	// for(i=0; i<5; i++) {
	// 	LCD_DrawLine(ball.posX, ball.posY+i, ball.posX+4, ball.posY+i, Green);
	// }


	
	
//   if(getDisplayPoint(&display, Read_Ads7846(), &matrix )){ //XXX se c'� un nuovo punto da viusalizzare..
// 		if(display.y < 280){                      
// 			TP_DrawPoint(display.x,display.y);                   //..chiama la draw pount
// 			GUI_Text(200, 0, (uint8_t *) "     ", Green, Green);
// 			clear = 0;
// 		}
// 		else{			
// 			if(display.y <= 0x13E){			
// 				clear++;
// 				if(clear%20 == 0){
// 					sprintf(time_in_char,"%4d",clear/20);
// 					GUI_Text(200, 0, (uint8_t *) time_in_char, White, Green);
// 					if(clear == 200){	/* 1 seconds = 200 times * 500 us*/
// 						LCD_Clear(Green); //XXX cancella lo schermo e lo coloara di Green
// 						GUI_Text(0, 280, (uint8_t *) " touch here : 1 sec to clear ", Green, White);			
// 						clear = 0;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	else{
// 		//do nothing if touch returns values out of bounds
// 	}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
