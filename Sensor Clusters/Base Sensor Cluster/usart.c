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
			if(128 - (C0_buffer.buffer_index - 1) <= data_size) return 2;  // Return 2 if not enough room in buffer
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				C0_buffer.buffer[C0_buffer.buffer_index + i] = data[i];
				C0_buffer.buffer_index++;
			}
			if((C0_buffer.buffer_index) == data_size)  // Start shifting data into hardware buffers if hardware buffers are empty
			{
				USART_SendByte(1);
			}
			return 0;
		case 2:
			if(128 - (C1_buffer.buffer_index - 1) <= data_size) return 2;  // Return 2 if not enough room in buffer
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				C1_buffer.buffer[C1_buffer.buffer_index + i] = data[i];
				C1_buffer.buffer_index++;
			}
			if((C0_buffer.buffer_index) == data_size)  // Start shifting data into hardware buffers if hardware buffers are empty
			{
				USART_SendByte(1);
			}
			return 0;
		case 3:
			if(128 - (D0_buffer.buffer_index - 1) <= data_size) return 2;  // Return 2 if not enough room in buffer
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
			D0_buffer.buffer[D0_buffer.buffer_index + i] = data[i];
			D0_buffer.buffer_index++;
			}
			if((C0_buffer.buffer_index) == data_size)  // Start shifting data into hardware buffers if hardware buffers are empty
			{
				USART_SendByte(1);
			}
			return 0;
		case 4:
			if(128 - (D1_buffer.buffer_index - 1) <= data_size) return 2;  // Return 2 if not enough room in buffer
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				D1_buffer.buffer[D1_buffer.buffer_index + i] = data[i];
				D1_buffer.buffer_index++;
			}
			if((C0_buffer.buffer_index) == data_size)  // Start shifting data into hardware buffers if hardware buffers are empty
			{
				USART_SendByte(1);
			}
			return 0;
		case 5:
			if(128 - (E0_buffer.buffer_index - 1) <= data_size) return 2  // Return 2 if not enough room in buffer
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				E0_buffer.buffer[E0_buffer.buffer_index + i] = data[i];
				E0_buffer.buffer_index++;
			}
			if((C0_buffer.buffer_index) == data_size)  // Start shifting data into hardware buffers if hardware buffers are empty
			{
				USART_SendByte(1);
			}
		return 0;
	}
	return 1;  // Return 1 if 'usart' is invalid
}

uint8_t USART_InputBufferAdd(uint8_t data)
{
	if(128 - (IN_buffer.buffer_index - 1) <= 1) return 1;  // Return 1 if not enough room in buffer
	IN_buffer.buffer[IN_buffer.buffer_index] = data;
	IN_buffer.buffer_index++;
	return 0;
}

uint8_t USART_SendByte(uint8_t usart)
{
	switch(usart)
	{
		case 1:
			if (!(USARTC0.STATUS & USART_DREIF_bm)) return 2;  // Return 2 if USART is not ready
			if (C0_buffer.buffer_index == 0) return 3;         // Return 3 if buffer is empty
			USARTC0.DATA = C0_buffer.buffer[C0_buffer.buffer_index - 1];
			C0_buffer.buffer_index--;
			return 0;
		case 2:
			if (!(USARTC1.STATUS & USART_DREIF_bm)) return 2;  // Return 2 if USART is not ready
			if (C1_buffer.buffer_index == 0) return 3;         // Return 3 if buffer is empty
			USARTC1.DATA = C1_buffer.buffer[C1_buffer.buffer_index - 1];
			C1_buffer.buffer_index--;
			return 0;
		case 3:
			if (!(USARTD0.STATUS & USART_DREIF_bm)) return 2;  // Return 2 if USART is not ready
			if (D0_buffer.buffer_index == 0) return 3;         // Return 3 if buffer is empty
			USARTC0.DATA = D0_buffer.buffer[C0_buffer.buffer_index - 1];
			D0_buffer.buffer_index--;
			return 0;
		case 4:
			if (!(USARTD1.STATUS & USART_DREIF_bm)) return 2;  // Return 2 if USART is not ready
			if (D1_buffer.buffer_index == 0) return 3;         // Return 3 if buffer is empty
			USARTC0.DATA = D1_buffer.buffer[D1_buffer.buffer_index - 1];
			D1_buffer.buffer_index--;
			return 0;
		case 5:
			if (!(USARTE0.STATUS & USART_DREIF_bm)) return 2;  // Return 2 if USART is not ready
			if (E0_buffer.buffer_index == 0) return 3;         // Return 3 if buffer is empty
			USARTC0.DATA = E0_buffer.buffer[E0_buffer.buffer_index - 1];
			E0_buffer.buffer_index--;
			return 0;
	}
	return 1;  // Return 1 if 'usart' invalid
}