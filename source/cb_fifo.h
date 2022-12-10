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
​ ​*​ ​@brief​ ​An​ ​abstraction​ ​for​ various CBfifo operations
 * file initially created by Author: Howdy Pierce, howdy.pierce@colorado.edu and references taken from Alexander G. Dean
​ ​*
​ ​*​ ​This​ ​header​ ​file​ ​provides​ interfaces and prototype for Command handing features such as author,hexdump and help .
​ ​*
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​Nov ​10​ ​2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#ifndef _CBFIFO_H_
#define _CBFIFO_H_

#include <stdlib.h>
#include <stdint.h>
#include <MKL25Z4.h>

/*defined max capacity for each cbfifo*/
#define QUEUE_MAXCAPACITY (256)

/* Structure defining various parameters of the cbfifo including the read pointer, write pointer and length and data of each cbfifo element*/
typedef struct cbfifo_s
{
	unsigned int read;         				/* read pointer to the array*/
	unsigned int write;       				/* write pointer to the array */
	unsigned int length;       				/* length value of the fifo*/
	char cbuff_data[QUEUE_MAXCAPACITY];		/* cbfifo data buffer*/
}volatile cbfifo_t;

/** @brief queue_empty check and returns a value of the queue is empty or not
 *
 *	function checks if the length is 0
 *	@param cbfifo the instance of the cbfifo, could be either Tx buffer or Rx Buffer
 *  @return val 0 : when the queue is not empty and 1 : when the queue is empty
 */
extern int queue_empty(cbfifo_t * cbfifo);

/** @brief queue_full check and returns a value of the queue is full or not
 *
 *	function checks if the length is equal to the max capacity
 *	@param cbfifo the instance of the cbfifo, could be either Tx buffer or Rx Buffer
 *  @return val 0 : when the queue is not full and 1 : when the queue is full
 */
extern int queue_full(cbfifo_t * cbfifo);

/** @brief queue_size returns the current length of the cbfifo
 *
 *  @param cbfifo the instance of the cbfifo, could be either Tx buffer or Rx Buffer
 *  @return retval the current length of the queue
 */
extern int queue_size(cbfifo_t * cbfifo);

/** @brief cbfifo_enqueue enqueue the data into the cbfifo
 *
 *  @param cbfifo the instance of the cbfifo, could be either Tx buffer or Rx Buffer
 *  @return retval the status of the enqueue
 */
extern size_t cbfifo_enqueue(cbfifo_t * cbfifo, void *buf, size_t nbyte);

/** @brief cbfifo_dequeue dequeues the data from the cbfifo
 *
 *  @param cbfifo the instance of the cbfifo, could be either Tx buffer or Rx Buffer
 *  @return retval the dequeued element of the cbfifo
 */
extern size_t cbfifo_dequeue(cbfifo_t * cbfifo, void *buf, size_t nbyte);

/** @brief queue_init performs the initialization of the cbfifo
 *
 *  @param cbfifo the instance of the cbfifo, could be either Tx buffer or Rx Buffer
 */
extern void queue_init(cbfifo_t * cbfifo);

/** @brief test_cbfifo performs the testing of cbfifo at the initialization
 *
 */
void test_cbfifo(void);

#endif // _CBFIFO_H_
