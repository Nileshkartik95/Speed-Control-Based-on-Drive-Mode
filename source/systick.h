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
​ ​*​ ​@file​ ​systick.h
​ ​*​ ​@brief​ ​includes the functions to support tick time at various intervals
​ ​*
​ ​*​ ​This​ ​header​ ​file​ ​provides​ various API supported to check the Current Timer value and refernce tick times since reset
​ ​*
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
#include "MKL25Z4.h"
#include <core_cm0plus.h>




typedef uint32_t ticktime_t;  /* time since boot, in sixteenths of a second*/

/** @brief Abstracts the Systick Initialization
 *
 * initialize the timing system
 */
void init_systick(void);

/** @brief Abstracts the Systick time since initialization
 *
 * initialize the timing system
 * 	@return ticktime_t ticktime since the initialization
 */
ticktime_t now();

/** @brief Abstracts the reset operation of the systick timer which is called sequentially with the get_timer() to get the current refernce time since the reset
 *
 * reset the timer refernce value for relative time calculation
 */
void reset_timer();

/** @brief Abstracts the reset operation of the systick timer which is called sequentially with the get_timer() to get the current refernce time since the reset
 *
 * 	@return ticktime_t ticktime since the reset
 */
ticktime_t get_timer();  // returns ticks since the last call to reset_timer()
