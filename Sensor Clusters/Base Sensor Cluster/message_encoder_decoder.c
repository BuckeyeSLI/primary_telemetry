#include "message_encoder_decoder.h"

uint8_t Message_Encode(uint8_t message[], uint8_t message_size, uint8_t output[], uint8_t *output_size)
{
	*output_size = 1;
	output[0] = START_FLAG; // Add start flag

	for(uint8_t i = 0; i < message_size; i++) // Check each byte and add to output
	{
		if(message[i] == START_FLAG || message[i] == STOP_FLAG || message[i] == ESCAPE_FLAG) // Insert escape flag if necessary
		{
			output[*output_size] = ESCAPE_FLAG;
			(*output_size)++;
		}
		output[*output_size] = message[i];
		(*output_size)++;
	}

	output[*output_size] = STOP_FLAG; // Add end flag
	(*output_size)++;

	return 0;
}

uint8_t Message_Decode(uint8_t message[], uint8_t message_size, uint8_t output[], uint8_t *output_size)
{
	*output_size = 0;

	if(message[0] != START_FLAG) return 1; // Check for presence of start flag and return with error if not present

	if(message[message_size - 1] != STOP_FLAG) return 2; // Check for presence of stop flag and return with error if present

	for(uint8_t i = 1; i < (message_size - 1); i++) // Parse message content to output and remove escape flags
	{
		if(message[i] == START_FLAG || message[i] == STOP_FLAG) return 3; // Check for unescaped start or stop flag and return with error if present
		if(message[i] == ESCAPE_FLAG) // Handle escape flag
		{
			if(i == message_size - 2) return 4; // Check for escape flag escaping the message ending stop flag and return with error if present
			if(message[i + 1] != START_FLAG && message[i + 1] != STOP_FLAG && message[i + 1] != ESCAPE_FLAG) return 5; // Check for escape flag not escaping a special flag and return error if present
			i++; // Skip escape flag
		}
		output[*output_size] = message[i];
		(*output_size)++;
	}

	return 0;
}