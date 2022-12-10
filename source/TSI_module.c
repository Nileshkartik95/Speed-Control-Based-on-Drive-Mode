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
​ ​*​ ​@file​ ​TSI_module.c
​ ​*​ ​@brief​ this source includes various API including the TSI initialization and get_touch_value
 *
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​09-Oct-2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "MKL25Z4.h"
#include "TSI_module.h"
#define TOUCH_OFFSET 500  // offset value to be subtracted

#define TOUCH_DATA (TSI0->DATA & 0xFFFF)  				/*abstracting the reception of  analog value from the TS->10 data*/

volatile int touch_val_var = 0;
bool readtouch_st = false;

void TSIInit(void)
{
	SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;
	TSI0->GENCS = TSI_GENCS_MODE(0u) |
								TSI_GENCS_REFCHRG(0u) |
								TSI_GENCS_DVOLT(0u) |
								TSI_GENCS_EXTCHRG(0u) |
								TSI_GENCS_PS(0u) |
								TSI_GENCS_NSCN(31u) |
								TSI_GENCS_TSIEN_MASK |
								TSI_GENCS_STPE_MASK |  // enabling the TSI in low power modes
								TSI_GENCS_EOSF_MASK |
								TSI_GENCS_ESOR_MASK | //enabling interrupt using end of scan
								TSI_GENCS_TSIIEN_MASK; //enabling the TSI interrupt

	TSI0->DATA = 	TSI_DATA_TSICH(10u); // selecting channel 10
	//enaling interrupt in NVIC
	NVIC_SetPriority(TSI0_IRQn, 2);
	NVIC_ClearPendingIRQ(TSI0_IRQn);
	NVIC_EnableIRQ(TSI0_IRQn);
	// Allow low leakage stop mode
	SMC->PMPROT = SMC_PMPROT_ALLS_MASK; //
	// Enable low-leakage stop mode and regular run mode
	SMC->PMCTRL = SMC_PMCTRL_STOPM(3) | SMC_PMCTRL_RUNM(0);
	SMC->STOPCTRL = SMC_STOPCTRL_PSTOPO(0) | SMC_STOPCTRL_VLLSM(3);
	// Enable LLWU83
	// allow TSI0 to wake LLWU
	LLWU->ME |= LLWU_ME_WUME4_MASK;
}

void TSI0_IRQHandler (void){
	touch_val_var = TOUCH_DATA - TOUCH_OFFSET;
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK ; 	//writing one to clear the flag
	readtouch_st = true;
}


int get_touch_val(void)
{
	if(readtouch_st == true)
	{
		readtouch_st = false;			/*reset the read touch TSI capacitive value only if the the read is executed*/
		return touch_val_var;			/*return touch val*/
	}
	return 0;							/*return an invalid touch value*/
}

