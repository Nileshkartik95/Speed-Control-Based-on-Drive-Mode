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
​ ​*​ ​@file​ ​cb_fifo.h
​ ​*​ ​@brief​ ​An​ ​abstraction​ ​for​ Circular buffer implementation
 * file initially created by Author: Howdy Pierce, howdy.pierce@colorado.edu and references taken from Alexander G. Dean
​ ​*
​ ​*​ ​This​ ​source​ ​file​ ​provides​ interfaces and prototype for circular buffer enqueu and dequeue and length api.
​ ​*
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​Nov ​10​ ​2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#include "cb_fifo.h"
#include "uart.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

void queue_init(cbfifo_t * cbfifo)
{
	cbfifo->write = 0;															/*initializing the write pointer to 0*/
	cbfifo->read = 0;															/*initializing the read pointer to 0*/
	cbfifo->length = 0;															/*initializing the length value to 0*/
	for(int i = 0 ; i < QUEUE_MAXCAPACITY; i++)
	{
		cbfifo->cbuff_data[i] = 0;												/*initializing the each element in cbfifo data  to 0*/
	}
}

int queue_empty(cbfifo_t * cbfifo)
{
	return cbfifo->length == 0;													/*return 1 if the cbfifo is empty*/
}

int queue_full(cbfifo_t * cbfifo)
{
	return cbfifo->length == QUEUE_MAXCAPACITY;									/*return 1 if the length is maximum and 0 if the length is less than max capacity*/
}

int queue_size(cbfifo_t * cbfifo)
{
	return cbfifo->length;														/*return the current length of the cbfifo*/
}

int cbfifo_capacity()
{
	return QUEUE_MAXCAPACITY;                          							/*return the maximum capacity*/
}


size_t cbfifo_enqueue(cbfifo_t * cbfifo,void *buf, size_t nbyte)
{
	uint32_t masking_state = __get_PRIMASK();
	__disable_irq();
	uint8_t *src_byte = (uint8_t *)buf;             			/* storing the address of the Input buffer to src byte*/
	uint8_t byte_wr = 0;                            			/* Temp variable to hold the bytes written*/

	int remain_size =  QUEUE_MAXCAPACITY - cbfifo->length;		/*store the remaining size present in the queue*/
	if((buf == NULL) || ((cbfifo->length == QUEUE_MAXCAPACITY) || (nbyte == 0)))
	{
		__set_PRIMASK(masking_state);
		return 0;												/* returns 0 if the buffer is null length and capacity is same or nbyte is 0*/
	}
	else
	{
		/* update the comparotor value based on the length and available size in the buffer and nbyte*/
		size_t comparator = (remain_size < nbyte)?remain_size:nbyte;
		for(byte_wr = 0;byte_wr < comparator; byte_wr++)
		{
			if (cbfifo->write==QUEUE_MAXCAPACITY)
			{
				cbfifo->write = 0;								/*reset the write pointer in case of write and max capacity is same*/
			}
			cbfifo->cbuff_data[cbfifo->write++] = src_byte[byte_wr];		/* update the data in the cb fifo buffer*/
		}
		cbfifo->length = (comparator < nbyte)?QUEUE_MAXCAPACITY:cbfifo->length+nbyte;		/* update the length */
		__set_PRIMASK(masking_state);
		return byte_wr;
	}
	__set_PRIMASK(masking_state);
	return byte_wr;
}

size_t cbfifo_dequeue(cbfifo_t * cbfifo,void *buf, size_t nbyte)
{
	uint32_t masking_state = __get_PRIMASK();
	__disable_irq();
	uint8_t *dest_byte = (uint8_t *)buf;
	int byte_rd = 0;

	if((cbfifo->length == 0) || (nbyte == 0) || (buf == NULL))
	{
		__set_PRIMASK(masking_state);
		return 0;															/* return 0 if the length is 0 and nothing to dequeu and nbyte is 0*/
	}
	else
	{
		size_t comparator = (cbfifo->length <nbyte)?cbfifo->length:nbyte;	/* update the comparotor value based on the length and available size in the buffer and nbyte*/
		for(byte_rd = 0;byte_rd < comparator; byte_rd++)
		{
			if (cbfifo->read==QUEUE_MAXCAPACITY)
			{
				cbfifo->read = 0;											/*reset the read pointer in case the read and maxcapacity is same*/
			}
			dest_byte[byte_rd] = cbfifo->cbuff_data[cbfifo->read++];		/* return the data from the cbfifo*/
		}
		cbfifo->length = (cbfifo->length <nbyte)?0:(cbfifo->length-nbyte);	/* update the length*/
		__set_PRIMASK(masking_state);
		return byte_rd;
	}
}

void test_cbfifo(void)
{
	char buf[1024];
	char *str ="Hello world how are you, This assignment describes to implement the UART based"
			"command processor tied to the Circular buffer along with the state machine"
			"to determine the state and call the command handler";
	const int cap = QUEUE_MAXCAPACITY;

	/* starting with the dequeue operation*/
	assert(queue_size(&TxQ) == 0);
	assert(cbfifo_dequeue(&TxQ, buf, cap) == 0);
	assert(cbfifo_dequeue(&TxQ,buf, 2) == 0);
	assert(cbfifo_enqueue(&TxQ, str,15) == 15);			/* enqueue 10 characters from the string*/
	assert(queue_size(&TxQ) == 15);						/* check if the size if 10*/
	assert(cbfifo_dequeue(&TxQ, buf,15) == 15);			/* dequeue 10 characters from the buffer*/
	assert(strncmp(buf, str, 10) == 0);					/* ensure if the buffer content matches */
	assert(queue_size(&TxQ) == 0);						/* ensure that length is 0 after dequeue*/

	assert(cbfifo_enqueue(&TxQ, str, 30) == 30);		/* enqueue 30 characters */
	assert(queue_size(&TxQ) == 30);						/* check the size of the queue*/
	assert(cbfifo_dequeue(&TxQ,buf, 5) == 5);			/* dequeu 5 characters*/
	assert(queue_size(&TxQ)== 25);						/*check if the buffer size is 25*/
	queue_init(&TxQ);									/* Re initialize the queue */

	assert(cbfifo_enqueue(&TxQ,str,30) == 30);			/* enque 30 characters*/
	assert(queue_size(&TxQ)== 30);						/* ensuring that length is 30*/
	assert(cbfifo_dequeue(&TxQ,buf,30)== 30);			/* dequeu 30 characters*/
	assert(queue_size(&TxQ)== 0);						/* ensuring that size is 0*/
	assert(strncmp(buf, str, 30)== 0);					/* comapring the buff content*/

	assert(cbfifo_enqueue(&TxQ,str,20)== 20);			/* enque 20 characters*/
	assert(queue_size(&TxQ)== 20);						/* check the size of buffer*/
	assert(cbfifo_dequeue(&TxQ,buf, 15)== 15);			/* dequeue 15 characters*/
	assert(queue_size(&TxQ)== 5);						/* check the size of buffer*/
	assert(strncmp(buf, str, 15) == 0);					/* Comparing the buffer content*/
	queue_init(&TxQ);									/* reinitialize the queue*/
}
