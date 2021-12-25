/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int key1_down = 0;
volatile int key2_down = 0;
volatile int key0_down = 0;
volatile int keyReset_down = 0;

void RIT_IRQHandler(void)
{

	/* ADC management */
	ADC_start_conversion();

	/* Button De Bouncing Management */
	// KEY 1
	if (key1_down != 0)
	{
		if ((LPC_GPIO2->FIOPIN & (1 << 11)) == 0)
		{ /* KEY1 pressed */
			key1_down++;
			switch (key1_down)
			{
			case 2: // RIT is always on due to adc, so i wait 2*50ms to execute button action
				ADC_init();			  /* ADC Initialization	for paddle movement	*/
			  enable_timer(0);
			
				break;
			default:
				break;
			}
		}
		else
		{ /* button released */
			key1_down = 0;
			NVIC_EnableIRQ(EINT1_IRQn);		  /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4 |= (1 << 22); /* External interrupt 0 pin selection */
		}
	}

	LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
