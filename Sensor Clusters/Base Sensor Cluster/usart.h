#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include <stdint.h>
#include <util/atomic.h>

// USART I/O buffer length - define in main.c to override
#ifndef USART_BUFFER_SIZE
#define USART_BUFFER_SIZE 128
#endif

// Data buffer struct definition
typedef struct
{
	uint8_t buffer[USART_BUFFER_SIZE];
	uint8_t buffer_head;
	uint8_t buffer_tail;
	uint8_t buffer_size;
} FIFOBuffer;

/*  Adds an array of data to the specified USART's output buffer.
 *  INPUTS:
 *		'usart'     - USART buffer the data should be added to. Valid values are 0-4, representing USARTs C0, C1, D0, D1, and E0 in order
 *		'data[]'    - Data to be added to the buffer
 *		'data_size' - Length of the array passed to 'data[]'
 *  RETURN:
 *		0 - (NO ERROR) Data added to USART buffer successfully
 *		1 - (ERROR) Invalid value passed to 'usart'
 *		2 - (ERROR) Not enough room in USART buffer to fit 'data[]'
 *		3 - (ERROR) Buffer unexpectedly filled while shifting 'data[]' into buffer (only part of 'data[]' shifted)
 */
uint8_t USART_OutputBufferAdd(uint8_t usart, uint8_t data[], uint8_t data_size);

/*	Adds a byte of data to the USART input buffer. Meant to be called during USART receive ISR.
 *  INPUTS:
 *		'usart' - USART buffer the data should be added to. Valid values are 0-4, representing USARTs C0, C1, D0, D1, and E0 in order
 *		'data'  - Data to be added to the buffer
 *  RETURN:
 *		0 - (NO ERROR) Data added to USART buffer successfully
 *      1 - (ERROR) Invalid value passed to 'usart'
 *		2 - (ERROR) USART buffer full
 */
uint8_t USART_InputBufferAdd(uint8_t usart, uint8_t data);

/*	Sends a byte of data from a USART software output buffer to a USART hardware output buffer. Meant to be called during USART send ISR.
 *  INPUTS:
 *		'usart' - USART buffer to shift data byte from software to hardware
 *  RETURN:
 *		0 - (NO ERROR) Data added to USART hardware buffer successfully
 *		1 - (ERROR) Invalid value passed to 'usart'
 *		2 - (ERROR) USART hardware buffer not ready
 *		3 - (ERROR) USART output buffer is empty
 */
uint8_t USART_SendByte(uint8_t usart);

#endif