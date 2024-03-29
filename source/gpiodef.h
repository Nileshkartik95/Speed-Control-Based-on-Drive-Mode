/*
 * gpiodef.h
 *
 *  Created on: 06-Nov-2022
 *      Author: Nileshkartik
 */

#ifndef GPIODEF_H_
#define GPIODEF_H_


// basic light switch
#define LED1_POS (1)	// on port A
#define LED2_POS (2)	// on port A
#define SW1_POS (5)		// on port A

#define MASK(x) (1UL << (x))

// Speaker output
#define SPKR_POS (0) 	// on port C

#endif /* GPIODEF_H_ */
