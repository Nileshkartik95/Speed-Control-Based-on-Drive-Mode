/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    main.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include <math.h>
#include "gpiodef.h"
#include "leds.h"
#include "i2c.h"
#include "mma8451.h"
#include "delay.h"
#include <string.h>
#include "gpiodef.h"
#include "UART.h"
#include "sysclock.h"
#include "cmd_handle.h"
#include "cb_fifo.h"
#include "TSI_module.h"

#include "systick.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
#define RES_NUMERATOR_TIMER (625)					/* scaling the resolution of 62.5ms*/
#define RES_DENOMINATOR_TIMER (10)					/* scaling the resolution of denominator for 62.5ms*/
#define PWM_PERIOD (0xFF)
#define TIMERVAL_125MSEC	(0.125)					/* 125msec value to be compared with the get timer value*/
#define TIMER_125MSEC  (2)							/* time period 125 msec*/
#define UPHILLSTATE_COLORVAL (0x0000FF00)			/*uphill state LED color value*/
#define DOWNHILLSTATE_COLORVAL (0x000000FF)			/*Downhill state LED color value*/
#define FLATSURFACESTATE_COLORVAL (0x00000000)		/*flat surface state LED color value*/
#define PARKSTATE_COLORVAL (0x00FF0000)			/*crosswalk state LED color value*/
#define DEG_45				(45)				/*45 degree equivalent*/
#define DELAY_100			(100)				/*delay for 100ticks*/

typedef enum stvehicledrive_t
{
	PLAINTERRAIN,                             	 		/* Enumeration for plain terrain*/
	UPHILL, 							  				/* Enumeration for Uphill*/
	DOWNHILL,                             				/* Enumeration for Downhill*/
	PARKBRAKE,                              			/* Enumeration for park brake*/
	ENDOFSTATE,											/* Enumeration for end of state*/
}st_vehicledrive;


void TakeAction_Event(st_vehicledrive state);			/* function prototype for the call back function*/

typedef void (*callback_t)(st_vehicledrive );			/*Call back function typedef*/

typedef struct event_t
{
	st_vehicledrive emergencystop_state;				/*emergency stop state*/
	st_vehicledrive flatsurface_state;					/*flat surface state*/
	callback_t action_transition_in;					/*action state*/
}state_table_entry_t;

state_table_entry_t state_table[] =
{
		/* flat surface    */{PARKBRAKE		, PLAINTERRAIN, TakeAction_Event 		},
		/* uphill          */{PARKBRAKE		, PLAINTERRAIN, TakeAction_Event 		},
		/* downhill        */{PARKBRAKE 	, PLAINTERRAIN, TakeAction_Event 		},
		/* Emergency stop  */{PARKBRAKE 	, PLAINTERRAIN, TakeAction_Event 		},

};

char* state_print[] = {"PLAINTERRAIN","UPHILL","DOWNHILL","PARKBRAKE"};	/* state equivalent to enum for printing*/

bool event_touch = false;								/* Event touch variable to be set when event is active*/

int color = FLATSURFACESTATE_COLORVAL;					/* Initial color to be loaded is flat surface*/
int prev_color = 0x00FFFFFF;							/* Previous color to be loaded invalid*/
st_vehicledrive transition_state = PLAINTERRAIN;		/*initial transition state is plain terrain*/


/* extract the red LED value and store it in redled_val*/
int get_redled_intensity(int color)
{
	return  ((color & (0x00FF0000))>>16);
}

/* extract the green LED value and store it in greenled_val*/
int get_greenled_intensity(int color)
{
	return ((color & (0x0000FF00))>>8);
}

/* extract the blue LED value and store it in blueled_val*/
int get_blueled_intensity(int color)
{
	return ((color & (0x000000FF)));
}

void TakeAction_Event(st_vehicledrive state)
{
	static st_vehicledrive prev_state = ENDOFSTATE;
	static bool bump_detected = false, roll_detected = false;
	{
		switch(state)
		{
		case UPHILL:
			if(state != prev_state)
			{
				/*print the current state along with the time only at edge*/
				reset_timer();
				bump_detected = false;
				printf("Current Vehicle State UPHILL at %d msec \r\n",((now()*RES_NUMERATOR_TIMER)/RES_DENOMINATOR_TIMER));
			}
			/*Set the led for the uphill drive mode*/
			event_touch = setledpwm_uphill();
			break;
		case DOWNHILL:
			if((get_timer() < TIME_PERIOD_1SEC) && (bump_detected == false) )
			{
				/*Detect bump only if the downhill is detected after uphill within 1 sec*/
				printf("Bump Detected\r\n");
				bump_detected = true;
			}
			else if(state != prev_state)
			{
				printf("Current Vehicle State DOWNHILL at %d msec \r\n",((now()*RES_NUMERATOR_TIMER)/RES_DENOMINATOR_TIMER));
			}
			/*Set the led for the downhill drive mode*/
			event_touch = setledpwm_downhill();	/*set the led for Downhill */
			break;
		case PARKBRAKE:
			if(state != prev_state)
			{
				printf("Current Vehicle State PARKBRAKE at %d msec \r\n",((now()*RES_NUMERATOR_TIMER)/RES_DENOMINATOR_TIMER));
			}
			event_touch = setledpwm_parkbrake();	/*set the led for Park brake terrain*/
			break;
		case PLAINTERRAIN:
			if(state != prev_state)
			{
				printf("Current Vehicle State PLAINTERRAIN at %dmsec \r\n",((now()*RES_NUMERATOR_TIMER)/RES_DENOMINATOR_TIMER));
			}
			event_touch = setledpwm_plainterrain();		/*set the led for plain terrrian*/
			break;
		case ENDOFSTATE:
			break;
		}
		if(event_touch == true)
		{
			/*if event touch is encountered set the Transition to cross walk based on the FSM */
			transition_state = state_table[state].emergencystop_state;
		}
		else
		{
			read_full_xyz();
			convert_xyz_to_roll_pitch();
			if(pitch > DEG_10)
			{
				transition_state = UPHILL;		/* if the pitch is greater than 10 deg set the transiion state to uphill*/
			}
			else if(pitch < -DEG_10)
			{
				transition_state = DOWNHILL;	/* if the pitch is lesser than 10 deg set the transiion state to Downhill*/
			}
			else if (fabs(pitch)<DEG_5)
			{
				transition_state = PLAINTERRAIN;	/* if the pitch is within than 5 deg set the transiion state to plain terrain*/
			}

			if((fabs(roll)>DEG_45) && (roll_detected == false))
			{
				roll_detected = true;
				printf("Roll Stability Support needs activation\r\n"); /* warning if the roll angle is more than 45 degree*/
			}
			else if(fabs(roll)<DEG_10)
			{
				roll_detected = false;
			}
		}

		switch(transition_state)
		{
		case (UPHILL):
					color = UPHILLSTATE_COLORVAL;			/* set the target color for Uphill*/
		break;
		case (DOWNHILL):
					color = DOWNHILLSTATE_COLORVAL;			/*set the target color for the downhill*/
		break;
		case (PLAINTERRAIN):
					color = FLATSURFACESTATE_COLORVAL;		/*set the target color for plain terrain */
		break;
		case (PARKBRAKE):
					color = PARKSTATE_COLORVAL;	/*Set the target color for Parkbrake */
		break;
		case (ENDOFSTATE):
				break;
		}
		prev_state = state;
	}
}



void transition_parkbrake(void)
{
	int red_val, green_val, blue_val;					/*Store R G B LED value*/
	int prev_red_val, prev_green_val, prev_blue_val;	/*Store the Previous R G B LED value*/
	int temp_red_val, temp_green_val, temp_blue_val;	/*Store the Intermediate LED value*/
	int ref_time_1000msec = 0;
	int factor_offset = 1;

	/*Mask and get the R G B Led value*/
	red_val = get_redled_intensity(color);
	green_val = get_greenled_intensity(color);
	blue_val = get_blueled_intensity(color);

	/*Mask and get the R G B previous Led value for transition*/
	prev_red_val = get_redled_intensity(prev_color);
	prev_green_val = get_greenled_intensity(prev_color);
	prev_blue_val = get_blueled_intensity(prev_color);


	ref_time_1000msec = now();					/*store the reference timer value for 1 sec check */
	reset_timer();						/*reset the timer to get 125 msec timer value*/
	while((now()-ref_time_1000msec) < TIME_PERIOD_1SEC)			/*looping for 1sec*/
	{
		if(get_timer() == TIMER_125MSEC)							/*check the timer for 125sec, timer resolution is 62*/
		{
			/* Get the Interpolated value for R G B Led value*/
			temp_red_val = ((red_val - prev_red_val)*factor_offset*TIMERVAL_125MSEC + prev_red_val);
			temp_green_val = ((green_val - prev_green_val)*factor_offset*TIMERVAL_125MSEC + prev_green_val);
			temp_blue_val = ((blue_val - prev_blue_val)*factor_offset*TIMERVAL_125MSEC + prev_blue_val);
			factor_offset++;

			/* Load the Intermediate vlaue to get the desired duty cycle */
			TPM2->CONTROLS[0].CnV = temp_red_val;
			TPM2->CONTROLS[1].CnV = temp_green_val;
			TPM0->CONTROLS[1].CnV = temp_blue_val;
			reset_timer();
		}
	}
}

/*
 * @brief   Application entry point.
 */
int main (void) {


	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif
	init_systick();
	sysclock_init();												/*initializes the sysclock*/
	uart0_init(BAUD_RATE);											/*initializes the uart*/
	TSIInit();														/* TSI init for the park brake validation*/

#ifdef DEBUG
	test_cbfifo();													/* perform the cbfifo testing at the initialization*/
	printf("\n\rTest cases for Circular buffer passed!");
#endif

	volatile st_vehicledrive state = PLAINTERRAIN;
	Init_RGB_LEDs(PWM_PERIOD);
	i2c_init();														/* init i2c	*/
	if (!init_mma()) {												/* init mma peripheral */
		while (1)													/* not able to initialize mma */
			;
	}
	Delay(DELAY_100);
	while(user_authetication() == 0);
	while (1) {
		if(event_touch == true)
		{
			printf("Button Press Detected System Time %d msec\r\n",(now()*RES_NUMERATOR_TIMER)/RES_DENOMINATOR_TIMER);
			transition_parkbrake();						/*if event is touch, transition to park brake*/
			transition_state = state_table[state].emergencystop_state;	/* set the pressed state*/
			event_touch = false;						/*reset the touch event*/
		}

		prev_color = color;								/*store the previous color with current color*/
		state_table[state].action_transition_in(state);	/*perform the event */
		state = transition_state;						/*store state with new state*/
	}
}
