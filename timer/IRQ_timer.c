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

void TIMER0_IRQHandler(void)
{
    // Vertical movement  : 1 = Sud,
    //					   -1 = North
    // Horizontal movement: 1 = East,
    //					   -1 = West

    uint8_t previous_h_speed, previous_v_speed, longer_speed, isColliding;
    uint8_t delta = 4;
    uint8_t is_game_over = 0;
    uint8_t i = 0;

    while (!is_game_over)
    {

        previous_h_speed = ball.h_speed;
        previous_v_speed = ball.v_speed;

        longer_speed = ball.v_speed >= ball.h_speed ? ball.v_speed : ball.h_speed;

        for (i = 0; i < longer_speed; i++)
        {

            // if ball is moving HORIZONTALLY and Horizontal speed is not lapsed
            if (ball.h_direc && ball.h_speed)
            {
                isColliding = is_colliding(ball, 'h');

                if (isColliding)
                {
                    ball.h_speed = previous_h_speed;
                    ball.v_speed = previous_v_speed;
                    ball.h_direc = -ball.h_direc;
                    // move_ball(ball);
                    LPC_TIM0->IR = 1; /* clear interrupt flag */
                    return;
                }
                if (ball.h_direc < 0)
                {
                    // MOVE horizontal left
                    // delete horizontal right
                    LCD_DrawLine(ball.posX + delta, ball.posY, ball.posX + delta, ball.posY + delta, Black);
                    // paint horizontal left
                    LCD_DrawLine(ball.posX + ball.h_direc, ball.posY, ball.posX + ball.h_direc, ball.posY + delta, Green);
                }
                else
                {
                    // MOVE horizontal right
                    // delete horizontal left
                    LCD_DrawLine(ball.posX, ball.posY, ball.posX, ball.posY + delta, Black);
                    // paint horizontal right
                    LCD_DrawLine(ball.posX + ball.h_direc + delta, ball.posY, ball.posX + ball.h_direc + delta, ball.posY + delta, Green);
                }
                // update x
                ball.posX = ball.posX + ball.h_direc;
                // decrement horizontal speed
                ball.h_speed--;
            }
            // if ball is moving VERTICALLY and Vertical speed is not Elapsed
            if (ball.v_direc && ball.v_speed)
            {
                isColliding = is_colliding(ball, 'v');
                if (isColliding)
                {
                    // handle vertical collission..
                    // ..opposite reflection angle if hits roof (v_direc < 0)
                    if (ball.v_direc < 0)
                    {
                        ball.h_speed = previous_h_speed;
                        ball.v_speed = previous_v_speed;
                        ball.v_direc = -ball.v_direc;
                    }
                    // ..handle reflection angle if hits paddle (v_direc > 0)
                    else
                    {
                        handle_paddle_collsion();
                        ball.v_direc = -ball.v_direc;
                    }

                    /**@todo handle different angles**/
                    LPC_TIM0->IR = 1; /* clear interrupt flag */
                    return;
                }
                if (ball.v_direc < 0)
                {
                    // MOVE vertical up
                    // delete vertical down
                    LCD_DrawLine(ball.posX, ball.posY + delta, ball.posX + delta, ball.posY + delta, Black);
                    // paint vertical up
                    LCD_DrawLine(ball.posX, ball.posY + ball.v_direc, ball.posX + delta, ball.posY + ball.v_direc, Green);
                }
                else
                {
                    // MOVE vertical down
                    // delete vertical up
                    LCD_DrawLine(ball.posX, ball.posY, ball.posX + delta, ball.posY, Black);
                    // paint vertical down
                    LCD_DrawLine(ball.posX, ball.posY + ball.v_direc + delta, ball.posX + delta, ball.posY + ball.v_direc + delta, Green);
                }
                // update y
                ball.posY = ball.posY + ball.v_direc;

                is_game_over = ball.posY > 278;
                if (is_game_over)
                    /**@todo handle Game over**/
                    break;
                // decrement vertical speed
                ball.v_speed--;
            }

            // restart if cycle ended up with no collisions
            if (longer_speed - 1 == i)
            {
                ball.h_speed = previous_h_speed;
                ball.v_speed = previous_v_speed;
                LPC_TIM0->IR = 1; /* clear interrupt flag */
                return;
            }
        }
    }

    LPC_TIM0->IR = 1; /* clear interrupt flag */
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
void TIMER1_IRQHandler(void)
{
    LPC_TIM1->IR = 1; /* clear interrupt flag */
    return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
