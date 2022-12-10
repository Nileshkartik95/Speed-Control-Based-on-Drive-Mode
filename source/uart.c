/******************************************************************************
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
​ ​*​ ​@file​ ​uart.c
​ ​*​ ​@brief​ ​An​ ​abstraction​ ​for​ uart implementation
 * references taken from Alexander G. Dean chapter 8
​ ​*
​ ​*​ ​This​ source file provides .
​ ​*
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​Nov ​10​ ​2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#include "UART.h"
#include <stdio.h>
#include "sysclock.h"

cbfifo_t TxQ, RxQ;

struct __FILE
{
	int handle;
};

FILE __stdout;  //Use with printf
FILE __stdin;		//use with fget/sscanf, or scanf


//Retarget the fputc method to use the UART0
int fputc(int ch, FILE *f){
	while(!(UART0->S1 & UART_S1_TDRE_MASK) && !(UART0->S1 & UART_S1_TC_MASK));
	UART0->D = ch;
	return ch;
}

//Retarget the fgetc method to use the UART0
int fgetc(FILE *f){
	while(!(UART0->S1 & UART_S1_RDRF_MASK));
	return UART0->D;
}

void uart0_init(uint32_t baud_rate)

{
	uint16_t sbr;
	// Enable clock gating for UARTO and Port A

	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;//Enable clock for UART0
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;//Port A clock enable
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK; //RE and TE Disable
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);//Clock source set to MCGFLLCLK or  MCGPLLCLK/2
	SIM->SOPT2 |= 0x00U;//Clock MCGFLLCLK is set
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2]=  PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx
	sbr = (uint16_t) ((SYSCLOCK_FREQUENCY)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;	/* setting the baud rate*/
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);	/*setting the baud rate value*/
	UART0->BDL = UART0_BDL_SBR(sbr);		/*setting the baud rate value*/
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);	/*setting the oversampling rate*/
	UART0->BDH |= UART0_BDH_RXEDGIE(0)|UART0_BDH_SBNS(STOP_BIT)|UART0_BDH_LBKDIE(0);
	UART0->C1= UART0_C1_LOOPS(0) | UART0_C1_M(DATA_SIZE) |UART0_C1_PE(PARITY_BIT);// Loops= 0 means seperate Rx and Tx. M 0.Start, 8 data bits, stop/ Parity bit disabled
	UART0->C3 = UART0_C3_TXINV (0) | UART0_C3_ORIE (1)| UART0_C3_NEIE (1) | UART0_C3_FEIE(1)| UART0_C3_PEIE (1);
	UART0->S1=UART0_S1_OR(1) |UART0_S1_NF(1)|UART0_S1_FE(1)|UART0_S1_PF(1);	/*setting the parity configuration*/
	UART0->S2 = UART0_S2_MSBF(0)|UART0_S2_RXINV(0);	/*setting non inverted data conf*/
	NVIC_SetPriority(UART0_IRQn,2);			/*setting the priority level*/
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);				/*enable the iRQ */
	UART0->C2 |= UART_C2_RIE(1);			/*setting the receive interrupt*/
	UART0->C2 |= UART0_C2_TE(1)|UART0_C2_RE(1);	/*setting transmit enable and receive enable*/
	//Clear
	UART0->S1 &=~UART0_S1_RDRF_MASK;			/* clearing the data to be read from buffer flag*/

}


int __sys_readc(void)
{
	while (queue_empty(&RxQ) == 1); 				/* wait for character to arrive*/
	char char_retval;
	cbfifo_dequeue(&RxQ,&char_retval,1);			/* dequeue the character and return the character*/
	return char_retval;
}

int __sys_write(int handle, char *str, int size)
{
	int retval = 0;
	while (*str != '\0')
	{
		/* Poll if the queue is full */
		while (queue_full(&TxQ));
		cbfifo_enqueue(&TxQ,str,1);					/* if the queue is not full enqueue into the buffer untill a null character is found*/
		str++;
	}
	UART0->C2 |= UART0_C2_TIE(1);					/* transmit interrupt is enabled*/
	return retval;
}

void UART0_IRQHandler(void) {
	uint8_t ch;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
			UART_S1_FE_MASK | UART_S1_PF_MASK)) {
		// clear the error flags
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
				UART0_S1_FE_MASK | UART0_S1_PF_MASK;
		// read the data register to clear RDRF
		ch = UART0->D;
	}
	if (UART0->S1 & UART0_S1_RDRF_MASK) {
		// received a character
		ch = UART0->D;
		if (!queue_full(&RxQ)) {
			cbfifo_enqueue(&RxQ,&ch,1);
		} else {
			// error - queue full.
			// discard character
		}
	}
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && // transmitter interrupt enabled
			(UART0->S1 & UART0_S1_TDRE_MASK) ) { // tx buffer empty
		// can send another character
		if (!queue_empty(&TxQ)) {
			char char_retval;
			cbfifo_dequeue(&TxQ,&char_retval,1);
			UART0->D = char_retval;
			//UART0->C2 &= ~UART0_C2_TIE_MASK;
		} else {
			// queue is empty so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}
