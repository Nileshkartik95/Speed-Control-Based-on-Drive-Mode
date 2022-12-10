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
​ ​*​ ​@file​ ​uart.h
​ ​*​ ​@brief​ ​An​ ​abstraction​ ​for​ uart implementation
 * references taken from Alexander G. Dean chapter 8
​ ​*
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​Nov ​10​ ​2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <MKL25Z4.H>
#include "cb_fifo.h"

#define USE_UART_INTERRUPTS 	(0) 		// 0 for polled UART communications, 1 for interrupt-driven
#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 				(24e6)
#define SYS_CLOCK				(48e6)


#define BAUD_RATE 	(38400)				/* setting the baud rate value*/
#define STOP_BIT   	(1) 				/* value 1 for stop bit 2 and value 0 for default stop bit */
#define DATA_SIZE   (0)					/* by default 0 means 8bit data size, specify 1 for 9 bit data*/
#define PARITY_BIT  (0)					/*parity check disabled, enable this bit to enable the parity check*/


/* initilization funtion for uart0*/
void uart0_init(uint32_t baud_rate);

extern cbfifo_t TxQ, RxQ;

#endif /* UART_H_ */
