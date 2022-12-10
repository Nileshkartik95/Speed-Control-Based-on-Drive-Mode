/*****************************************************************************
​ ​*​ ​Copyright​ ​(C)​ ​2022 ​by​ ​Nileshkartik Ashokkumar
​ ​*
​ ​*​ ​Redistribution,​ ​modification​ ​or​ ​use​ ​of​ ​this​ ​software​ ​in​ ​source​ ​or​ ​binary
​ ​*​ ​forms​ ​is​ ​permitted​ ​as​ ​long​ ​as​ ​the​ ​files​ ​maintain​ ​this​ ​copyright.​ ​Users​ ​are
​ ​*​ ​permitted​ ​to​ ​modify​ ​this​ ​and​ ​use​ ​it​ ​to​ ​learn​ ​about​ ​the​ ​field​ ​of​ ​embedded
​ ​*​ ​software.​ ​Nileshkartik Ashokkumar​ ​and​ ​the​ ​University​ ​of​ ​Colorado​ ​are​ ​not​ ​liable​ ​for
​ ​*​ ​any​ ​misuse​ ​of​ ​this​ ​material.
​ ​*
*****************************************************************************/
/**
​ ​*​ ​@file​ ​systick.c
​ ​*​ ​@brief​ ​includes the functions to support tick time at various intervals
​ ​* @reference : msec_timer.c by Howdy Perice
​ ​*
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​october ​3​ ​2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#include <stdio.h>
#include <stdint.h>
#include "systick.h"

#define SYSTICK_LOAD (48000000/16)		/* Systick Load Value */


typedef uint32_t ticktime_t;    		/*type defining the ticktime_t*/
ticktime_t g_timer_start = 0;			/*reference timer value*/
volatile ticktime_t g_timer_63msec=0;	/*main timer value for getting the now time*/


void init_systick()
{
	SysTick_Config(SYSTICK_LOAD);
}

ticktime_t now()
{
	return g_timer_63msec;   				/*return now time from the start of ECU*/
}

void reset_timer()
{
	g_timer_start = g_timer_63msec;			/*reset timer for storing the reference timer value and get timer value*/
}

ticktime_t get_timer()
{
	return g_timer_63msec - g_timer_start; 	/*subtracting the now timer value from the reference timer value*/
}

void SysTick_Handler()
{
	g_timer_63msec++;						/*incrementing the timer value every time the handler is called based on the load systick value*/
}
