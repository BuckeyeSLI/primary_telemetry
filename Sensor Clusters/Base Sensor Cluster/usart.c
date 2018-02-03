#include "usart.h"

// Define USART data buffers
static IOBuffer C0buffer;
static IOBuffer C1buffer;
static IOBuffer D0buffer;
static IOBuffer D1buffer;
static IOBuffer E0buffer;

uint8_t USART_OutputBufferAdd(uint8_t buffer, uint8_t data[], uint8_t data_size)
{
	switch(buffer)
	{
	case 1:
		if(128 - (C0buffer.input_index - 1) <= data_size) return 2;
		for(uint8_t i = 0; i < data_size; i++)
		{
			C0buffer.input_index[C0buffer.input_index + i] = data[i];
			C0buffer.input_index++;
		}
		return 0;
	};



	return 1;
};