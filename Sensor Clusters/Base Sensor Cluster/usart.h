#ifndef USART_H
#define USART_H

// Data buffer struct definition
typedef struct
{
	uint8_t input[128];
	uint8_t input_index;
	uint8_t output[128];
	uint8_t output_index;
} IOBuffer;

/*
 *	Adds an array of data to the specified USART's output buffer.
 *  INPUTS:
 *		'buffer'      - USART buffer the data should be added to. Valid values are 1-5, representing USARTs C0, C1, D0, D1, and E0 in order.
 *		'data[]'      - Data to be added to the buffer
 *		'data_size'   - Length of the array passed to 'data[]'
 *  RETURN:
 *		0 - (NO ERROR) Data added to USART buffer successfully
 *		1 - (ERROR) Invalid value passed to 'buffer'
 *		2 - (ERROR) Not enough room in USART buffer to fit 'data[]'
*/
uint8_t USART_BufferAdd(uint8_t buffer, uint8_t data[], uint8_t data_size);

#endif