#include "usart.h"

// Define USART data buffers
static volatile FIFOBuffer C0_out_buffer;
static volatile FIFOBuffer C1_out_buffer;
static volatile FIFOBuffer D0_out_buffer;
static volatile FIFOBuffer D1_out_buffer;
static volatile FIFOBuffer E0_out_buffer;
static volatile FIFOBuffer C0_in_buffer;
static volatile FIFOBuffer C1_in_buffer;
static volatile FIFOBuffer D0_in_buffer;
static volatile FIFOBuffer D1_in_buffer;
static volatile FIFOBuffer E0_in_buffer;

// Internal function to write to FIFO buffer. Returns 1 if buffer is full.
static uint8_t FIFOWrite(volatile FIFOBuffer *buffer, uint8_t byte)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) // Ensure buffer is accessed atomically
	{
		if((*buffer).buffer_head + 1 == (*buffer).buffer_tail || ((*buffer).buffer_head == USART_BUFFER_SIZE - 1 && (*buffer).buffer_tail == 0)) return 1; // No room in buffer
		(*buffer).buffer[(*buffer).buffer_head] = byte;
		if((*buffer).buffer_head == USART_BUFFER_SIZE - 1) (*buffer).buffer_head = 0; // Handle buffer wrap-around
		else (*buffer).buffer_head++;
		(*buffer).buffer_size++;
	}
	return 0;
}

// Internal function to read from FIFO buffer. Returns 1 if buffer is empty.
static uint8_t FIFORead(volatile FIFOBuffer *buffer, uint8_t *byte)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) // Ensure buffer is accessed atomically
	{
		if((*buffer).buffer_head == (*buffer).buffer_tail) return 1; // Buffer empty
		*byte = (*buffer).buffer[(*buffer).buffer_tail];
		if((*buffer).buffer_tail == USART_BUFFER_SIZE - 1) (*buffer).buffer_tail = 0; // Handle buffer wrap-around
		else (*buffer).buffer_tail++;
		(*buffer).buffer_size--;
	}
	return 0;
}

uint8_t USART_OutputBufferAdd(uint8_t usart, uint8_t data[], uint8_t data_size)
{
	switch(usart)
	{
		uint8_t error = 0;
		case 0:
			if(USART_BUFFER_SIZE - C0_out_buffer.buffer_size < data_size) return 2; // Return 2 if not enough room in buffer for 'data[]'
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				error = FIFOWrite(&C0_out_buffer, data[i]);
				if(error == 1) break; // Stop filling buffer if full
			}
			if(USARTC0.STATUS & USART_DREIF_bm)  // Start shifting data into hardware buffers if hardware buffers are empty
			{
				USART_SendByte(0);
			}
			if(error == 1) return 3; // Return 3 if buffer fills unexpectedly
			return 0;
		case 1:
			if(USART_BUFFER_SIZE - C1_out_buffer.buffer_size < data_size) return 2; // Return 2 if not enough room in buffer for 'data[]'
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				error = FIFOWrite(&C1_out_buffer, data[i]);
				if(error == 1) break; // Stop filling buffer if full
			}
			if(USARTC1.STATUS & USART_DREIF_bm)  // Start shifting data into hardware buffers if hardware buffers are empty
			{
				USART_SendByte(1);
			}
			if(error == 1) return 3; // Return 3 if buffer fills unexpectedly
			return 0;
		case 2:
			if(USART_BUFFER_SIZE - D0_out_buffer.buffer_size < data_size) return 2; // Return 2 if not enough room in buffer for 'data[]'
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				error = FIFOWrite(&D0_out_buffer, data[i]);
				if(error == 1) break; // Stop filling buffer if full
			}
			if(USARTD0.STATUS & USART_DREIF_bm)  // Start shifting data into hardware buffers if hardware buffers are empty
			{
				USART_SendByte(2);
			}
			if(error == 1) return 3; // Return 3 if buffer fills unexpectedly
			return 0;
		case 3:
			if(USART_BUFFER_SIZE - D1_out_buffer.buffer_size < data_size) return 2; // Return 2 if not enough room in buffer for 'data[]'
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				error = FIFOWrite(&D1_out_buffer, data[i]);
				if(error == 1) break; // Stop filling buffer if full
			}
			if(USARTD1.STATUS & USART_DREIF_bm)  // Start shifting data into hardware buffers if hardware buffers are empty
			{
				USART_SendByte(3);
			}
			if(error == 1) return 3; // Return 3 if buffer fills unexpectedly
			return 0;
		case 4:
			if(USART_BUFFER_SIZE - E0_out_buffer.buffer_size < data_size) return 2; // Return 2 if not enough room in buffer for 'data[]'
			for(uint8_t i = 0; i < data_size; i++)  // Add data to buffer
			{
				error = FIFOWrite(&E0_out_buffer, data[i]);
				if(error == 1) break; // Stop filling buffer if full
			}
			if(USARTE0.STATUS & USART_DREIF_bm)  // Start shifting data into hardware buffers if hardware buffers are empty
			{
				USART_SendByte(4);
			}
			if(error == 1) return 3; // Return 3 if buffer fills unexpectedly
			return 0;
	}
	return 1;  // Return 1 if 'usart' is invalid
}

uint8_t USART_InputBufferAdd(uint8_t usart, uint8_t data)
{
	switch(usart)
	{
		uint8_t error = 0;
		case 0:
			error = FIFOWrite(&C0_in_buffer, data);
			if(error == 1) return 2; // Return 2 if buffer full
			return 0;
		case 1:
			error = FIFOWrite(&C1_in_buffer, data);
			if(error == 1) return 2; // Return 2 if buffer full
			return 0;
		case 2:
			error = FIFOWrite(&D0_in_buffer, data);
			if(error == 1) return 2; // Return 2 if buffer full
			return 0;
		case 3:
			error = FIFOWrite(&D1_in_buffer, data);
			if(error == 1) return 2; // Return 2 if buffer full
			return 0;
		case 4:
			error = FIFOWrite(&E0_in_buffer, data);
			if(error == 1) return 2; // Return 2 if buffer full
			return 0;
	}
	return 1;  // Return 1 if 'usart' is invalid
}

uint8_t USART_SendByte(uint8_t usart)
{
	switch(usart)
	{
		uint8_t error = 0;
		uint8_t data = 0x00;
		case 0:
			if (!(USARTC0.STATUS & USART_DREIF_bm)) return 2;  // Return 2 if USART is not ready
			error = FIFORead(&C0_out_buffer, &data);
			if (error == 1) return 3; // Return 3 if buffer is empty
			USARTC0.DATA = data;
			return 0;
		case 1:
			if (!(USARTC0.STATUS & USART_DREIF_bm)) return 2;  // Return 2 if USART is not ready
			error = FIFORead(&C1_out_buffer, &data);
			if (error == 1) return 3; // Return 3 if buffer is empty
			USARTC0.DATA = data;
			return 0;
		case 2:
			if (!(USARTC0.STATUS & USART_DREIF_bm)) return 2;  // Return 2 if USART is not ready
			error = FIFORead(&D0_out_buffer, &data);
			if (error == 1) return 3; // Return 3 if buffer is empty
			USARTC0.DATA = data;
			return 0;
		case 3:
			if (!(USARTC0.STATUS & USART_DREIF_bm)) return 2;  // Return 2 if USART is not ready
			error = FIFORead(&D1_out_buffer, &data);
			if (error == 1) return 3; // Return 3 if buffer is empty
			USARTC0.DATA = data;
			return 0;
		case 4:
			if (!(USARTC0.STATUS & USART_DREIF_bm)) return 2;  // Return 2 if USART is not ready
			error = FIFORead(&E0_out_buffer, &data);
			if (error == 1) return 3; // Return 3 if buffer is empty
			USARTC0.DATA = data;
			return 0;
	}
	return 1;  // Return 1 if 'usart' is invalid
}