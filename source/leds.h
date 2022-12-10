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
​ ​*​ ​@file​ ​leds.h
​ ​*​ ​@brief​  function prototype for various API used to set the LED according to various drive modes
​ ​*
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​Dec ​9 ​2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#ifndef LEDS_H_
#define LEDS_H_

#include "stdio.h"
#include "stdbool.h"

// Freedom KL25Z LEDs
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS (1)		// on port D
#define DEG_5 				(5)					/*5 degree equivalent*/
#define DEG_80				(80)				/*80 degree equivalent*/
#define DEG_10				(10)				/*10 degree equivalent*/
#define DEG_20				(20)				/*20 degree equivalent*/
#define TIME_PERIOD_1SEC (16)					/*Timer count for 1sec in debug mode*/


// function prototypes

/** @brief Initialization for the RGB led
 *
 */
void Init_RGB_LEDs(int period);

/** @brief Set the LED PWM for UPhill Drive mode and polls for the TSI module
 *
 */
bool setledpwm_uphill(void);

/** @brief Set the LED PWM for Down hill Drive mode and polls for the TSI module
 *
 */
bool setledpwm_downhill(void);

/** @brief Set the LED PWM for parkBrake mode and polls for the TSI module
 *
 */
bool setledpwm_parkbrake(void);

/** @brief reset the LED PWM for parkBrake mode and polls for the TSI module
 *
 */
bool setledpwm_plainterrain(void);

#endif
