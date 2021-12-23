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
unsigned short AD_last = 0xFF;     /* Last converted value               */

void ADC_IRQHandler(void) {
  	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current != AD_last){
	
    unsigned short www=0;
    www = AD_last*7/0xFFF;
    www = AD_current*7/0xFFF;
		
		if(www>4) {

      if(paddle.posX<188) {
          
          LCD_DrawLine(paddle.posX, paddle.posY, paddle.posX, paddle.posY+9, Black);
          paddle.posX++;
          LCD_DrawLine(paddle.posX+49, paddle.posY, paddle.posX+49, paddle.posY+9, Green);
      }
		}
		else if (www<4) {
        if(paddle.posX>0) {
          LCD_DrawLine(paddle.posX+49, paddle.posY, paddle.posX+49, paddle.posY+9, Black);
          paddle.posX--;
          LCD_DrawLine(paddle.posX, paddle.posY, paddle.posX, paddle.posY+9, Green);
        }
			
		}
		
  
		// LED_Off(AD_last*7/0xFFF);	  // ad_last : AD_max = x : 7 		LED_Off((AD_last*7/0xFFF));	
		// LED_On(AD_current*7/0xFFF);	// ad_current : AD_max = x : 7 		LED_On((AD_current*7/0xFFF));	
		
		AD_last = AD_current;
  }	
}
