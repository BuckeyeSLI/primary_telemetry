#include "usart.h"

// Define USART data buffers
static IOBuffer C0_buffer;
static IOBuffer C1_buffer;
static IOBuffer D0_buffer;
static IOBuffer D1_buffer;
static IOBuffer E0_buffer;
static IOBuffer IN_buffer;

uint8_t USART_OutputBufferAdd(uint8_t usart, uint8_t data[], uint8_t data_size)
{
	switch(usart)
	{
		case 1:
			if(128 - (C0_buffer.buffer_index - 1) <= data_size) return 2;
			for(uint8_t i = 0; i < data_size; i++)
			{
				C0_buffer.buffer[C0_buffer.buffer_index + i] = data[i];
				C0_buffer.buffer_index++;
			}
			return 0;
		case 2:
			if(128 - (C1_buffer.buffer_index - 1) <= data_size) return 2;
			for(uint8_t i = 0; i < data_size; i++)
			{
				C1_buffer.buffer[C1_buffer.buffer_index + i] = data[i];
				C1_buffer.buffer_index++;
			}
			return 0;
		case 3:
			if(128 - (D0_buffer.buffer_index - 1) <= data_size) return 2;
			for(uint8_t i = 0; i < data_size; i++)
			{
			D0_buffer.buffer[D0_buffer.buffer_index + i] = data[i];
			D0_buffer.buffer_index++;
			}
			return 0;
		case 4:
			if(128 - (D1_buffer.buffer_index - 1) <= data_size) return 2;
			for(uint8_t i = 0; i < data_size; i++)
			{
				D1_buffer.buffer[D1_buffer.buffer_index + i] = data[i];
				D1_buffer.buffer_index++;
			}
			return 0;
		case 5:
			if(128 - (E0_buffer.buffer_index - 1) <= data_size) return 2;
			for(uint8_t i = 0; i < data_size; i++)
			{
				E0_buffer.buffer[E0_buffer.buffer_index + i] = data[i];
				E0_buffer.buffer_index++;
			}
		return 0;
	}
	return 1;
}

uint8_t USART_InputBufferAdd(uint8_t data)
{
	if(128 - (IN_buffer.buffer_index - 1) <= 1) return 1;
	IN_buffer.buffer[IN_buffer.buffer_index] = data;
	IN_buffer.buffer_index++;
	return 0;
}

uint8_t USART_SendByte(uint8_t usart)
{
	switch(usart)
	{
		case 1:
			if (!(USARTC0.STATUS & USART_DREIF_bm)) return 2;
			if (C0_buffer.buffer_index == 0) return 3;
			USARTC0.DATA = C0_buffer.buffer[C0_buffer.buffer_index - 1];
			return 0;
		case 2:
			return 0;
		case 3:
			return 0;
		case 4:
			return 0;
		case 5:
			return 0;
	}
	return 1;
}