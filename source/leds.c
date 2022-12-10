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
​ ​*​ ​@file​ ​leds.c
​ ​*​ ​@brief​  Sets the Led according to various drive modes
​ ​*
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​Dec ​9 ​2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/
#include <MKL25Z4.H>
#include "mma8451.h"
#include "leds.h"
#include "gpiodef.h"
#include "systick.h"
#include "TSI_module.h"
#include "stdio.h"
#include "math.h"

#define LED_DEG_10  			(18)				/*led Intentsity for 10 deg change*/
#define LED_DEG_20 			(36)				/*led Intentsity for 20 deg change*/
#define LED_DEG_30			(72)				/*led Intentsity for 30 deg change*/
#define LED_DEG_40			(109)				/*led Intentsity for 40 deg change*/
#define LED_DEG_50			(145)				/*led Intentsity for 50 deg change*/
#define LED_DEG_60			(182)				/*led Intentsity for 60 deg change*/
#define LED_DEG_70			(218)				/*led Intentsity for 70 deg change*/
#define LED_DEG_80			(255)				/*led Intentsity for 80 deg change*/
#define MAX_COLOR_VAL			(255)				/*255 degree equivalent*/


/*const table for the LEd color change*/
const int change_color[] = {LED_DEG_10, LED_DEG_20, LED_DEG_30, LED_DEG_40, LED_DEG_50, LED_DEG_60, LED_DEG_70, LED_DEG_80};

void Init_RGB_LEDs(int period) {
	// Enable clock to port D and portB
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK);
	/*clearing the MUX Mask*/
	PORTD->PCR[BLUE_LED_POS] &= ~(PORT_PCR_MUX_MASK);
    PORTB->PCR[RED_LED_POS] &= ~(PORT_PCR_MUX_MASK);
    PORTB->PCR[GREEN_LED_POS] &= ~(PORT_PCR_MUX_MASK);
    /* Setting the PCR for PWM operation*/
	PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(4);
    PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(3);
    PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(3);
	// Configure TPM
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	//set clock source for tpm
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);
	//load the counter and mod
	TPM0->MOD = period-1;
	TPM2->MOD = period-1;
	//set TPM count direction to up with a divide by 2 prescaler
	TPM0->SC =  TPM_SC_PS(1);
	TPM2->SC =  TPM_SC_PS(1);
	// Continue operation in debug mode
	TPM0->CONF |= TPM_CONF_DBGMODE(3);
	TPM2->CONF |= TPM_CONF_DBGMODE(3);
	// Set channel 1 to edge-aligned low-true PWM
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
	// Set initial duty cycle
	TPM0->CONTROLS[1].CnV = 0;
	TPM2->CONTROLS[0].CnV = 0;
	TPM2->CONTROLS[1].CnV = 0;
	// Start TPM
	TPM0->SC |= TPM_SC_CMOD(1);
	TPM2->SC |= TPM_SC_CMOD(1);
}

bool setledpwm_uphill(void)
{
	int idx;
	static int ref_time = 0;				/*temp variable to store the reference timer*/
	static int recover_criticond = 0;			/*flag variable for recover from critical mode*/
	ref_time = now();					/*calculate the refernce timer*/
	read_full_xyz();					/*read the acceleration sensor value*/
	convert_xyz_to_roll_pitch();

	/* Perform uphill led color change if the pitch is more than 10 degree*/
	while((pitch >= DEG_10 )&& (fabs(roll) < DEG_45) )
	{
		/* Print the angle every 1 sec*/
		if(now() - ref_time == TIME_PERIOD_1SEC)
		{
			ref_time = now();
			printf("Current Uphill Angle %d\r\n", (int)pitch);
		}
		TSI0->DATA |= TSI_DATA_SWTS_MASK; 		/*set the software interrupt trigger*/
		if(pitch < DEG_80)
		{
			recover_criticond = 0;
			if(get_touch_val() > VALID_TOUCH)
			{
				return true;
			}
			idx = (pitch/DEG_10)-1;						/*calculate the idx for the led table*/
			TPM2->CONTROLS[0].CnV = 0;
			TPM2->CONTROLS[1].CnV = change_color[idx];			/* change the green led accroding to the constant table value*/
			TPM0->CONTROLS[1].CnV = 0;						
		}
		else if(((int)(pitch) >= DEG_80) && (recover_criticond == 0))
		{
			/* reset the led color for critical drive mode*/
			recover_criticond = 1;
			printf("CRITICAL DRIVE CONDITION\r\n");
			TPM2->CONTROLS[0].CnV = 0;
			TPM2->CONTROLS[1].CnV = 0;
			TPM0->CONTROLS[1].CnV = 0;
		}
		read_full_xyz();
		convert_xyz_to_roll_pitch();
	}
	return false;
}

bool setledpwm_downhill(void)
{
	int idx;
	static int ref_time = 0;
	static int recover_criticond = 0;
	ref_time = now();							/*calculate the refernce timer*/
	read_full_xyz();							/*read the acceleration sensor value*/
	convert_xyz_to_roll_pitch();

	while((pitch < -DEG_10 )&& (fabs(roll) < DEG_45))			
	{
		if(now() - ref_time == TIME_PERIOD_1SEC)
		{
			/* Print the angle every 1 sec*/
			ref_time = now();
			printf("Current Downhill Angle %d\r\n", (int)pitch);
		}
		TSI0->DATA |= TSI_DATA_SWTS_MASK; 							/*set the software interrupt trigger*/
		if(pitch > -DEG_80)
		{
			recover_criticond = 0;
			if(get_touch_val() > VALID_TOUCH)
			{
				return true;
			}
			idx = (pitch/DEG_10)-1;
			TPM2->CONTROLS[0].CnV = 0;
			TPM2->CONTROLS[1].CnV = 0;
			TPM0->CONTROLS[1].CnV = change_color[idx];						/*store the start time for counting initial 250sec ref*/
		}
		else if(((int)pitch < -DEG_80) && (recover_criticond == 0))
		{
			/* reset the led color for critical drive mode*/
			recover_criticond = 1;
			printf("CRITICAL DRIVE CONDITION\r\n");
			TPM2->CONTROLS[0].CnV = 0;
			TPM2->CONTROLS[1].CnV = 0;
			TPM0->CONTROLS[1].CnV = 0;
		}
		read_full_xyz();
		convert_xyz_to_roll_pitch();
	}
	return false;
}
bool setledpwm_plainterrain(void)
{
	read_full_xyz();
	convert_xyz_to_roll_pitch();

	while((fabs(roll) < DEG_5) && (fabs(pitch) < DEG_10))			/*wait for 10sec period*/
	{
		TSI0->DATA |= TSI_DATA_SWTS_MASK; 				/*set the software interrupt trigger*/
		if(get_touch_val() > VALID_TOUCH)
		{
			return true;
		}
		TPM2->CONTROLS[0].CnV = 0;
		TPM2->CONTROLS[1].CnV = 0;
		TPM0->CONTROLS[1].CnV = 0;					/*reset the led for plain terrain*/
		read_full_xyz();
		convert_xyz_to_roll_pitch();
	}
	return false;
}

bool setledpwm_parkbrake(void)
{
	TSI0->DATA |= TSI_DATA_SWTS_MASK; 		/*set the software interrupt trigger*/

	while(get_touch_val() > VALID_TOUCH)			
	{
		TPM2->CONTROLS[0].CnV = MAX_COLOR_VAL;		/*set the red led color for park brake*/
		TPM2->CONTROLS[1].CnV = 0;
		TPM0->CONTROLS[1].CnV = 0;						
	}
	return false;
}

