/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../GLCD/GLCD.h"
#include "../game/game.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;
unsigned short AD_last = 0xFF; /* Last converted value               */

void ADC_IRQHandler(void)
{
  unsigned short mov = 0;
  uint8_t increment = 0;
  int8_t dir = 0;
  uint8_t i = 0;
  uint8_t delta_del;
  uint8_t delta_draw;

  AD_current = ((LPC_ADC->ADGDR >> 4) & 0xFFF); /* Read Conversion Result             */

  mov = AD_current * 8 / 0xFFF;

  // move_paddle(mov);

  // mov == 0-3 move left
  // mov == 4   hold
  // mov == 5-8 move right

  dir = mov > 4 ? 1 : -1;  // -1
  delta_del  = dir > 0 ? 0 : 49; // 49
  delta_draw = dir > 0 ? 49 : 0; // 0

  switch (mov)
  {
  case 0:
  case 8:
    increment = 8;
    break;
  case 1:
  case 7:
    increment = 6;
    break;
  case 2:
  case 6:
    increment = 4;
    break;
  case 3:
  case 5:
    increment = 2;
    break;
  case 4:
    increment = 0;
    dir = 0;
    return;
  }

  for (i = 0; i < increment; i++)
  {
    if ((dir > 0 && paddle.posX < 188) || (dir < 0 && paddle.posX > 0))
    {
      //delete line
      LCD_DrawLine(paddle.posX + (delta_del/* + i*dir*/), paddle.posY, paddle.posX + (delta_del/* + i*dir*/), paddle.posY + 9, Black);
      //draw line
			paddle.posX = paddle.posX + dir;
      LCD_DrawLine(paddle.posX + (delta_draw/* + i*dir*/), paddle.posY, paddle.posX + (delta_draw/* + i*dir*/), paddle.posY + 9, Green);
    }
  }
  // change paddle position
  
}
