#ifndef MESSAGE_ENCODER_DECODER_H
#define MESSAGE_ENCODER_DECODER_H

// Start, stop, and escape flag definitions - define in main.c to override

#ifndef START_FLAG
#define START_FLAG 0x7E
#endif

#ifndef STOP_FLAG
#define STOP_FLAG 0x81
#endif

#ifndef ESCAPE_FLAG
#define ESCAPE_FLAG 0x99
#endif


#include <stdint.h>

/*	Encodes a message with start, stop, and escape bits.
 *  INPUTS:
 *		'message[]'    - Message to be encoded
 *		'message_size' - Length of the array passed to 'message[]'
 *		'output[]'     - Encoded message output
 *      '*output_size' - Pointer to an int storing the used size of 'output[]'
 *  RETURN:
 *		0 - (NO ERROR) Data encoded successfully
 */
uint8_t Message_Encode(uint8_t message[], uint8_t message_size, uint8_t output[], uint8_t *output_size);

/*	Strips start, stop, and escape bits from a message. Halts decoding if error encountered - calling program should
 *  discard any output if an error code is returned.
 *  INPUTS:
 *		'message[]'    - Message to be decoded
 *		'message_size' - Length of the array passed to 'message[]'
 *		'output[]'     - Decoded message output
 *      '*output_size' - Pointer to an int storing the used size of 'output[]'
 *  RETURN:
 *		0 - (NO ERROR) Data decoded successfully
 *      1 - (ERROR) No start flag
 *      2 - (ERROR) No stop flag
 *      3 - (ERROR) Unescaped start or stop flag
 *      4 - (ERROR) Orphaned escape flag at end of message
 *      5 - (ERROR) Orphaned escape flag in message
 */
 uint8_t Message_Decode(uint8_t message[], uint8_t message_size, uint8_t output[], uint8_t* output_size);

#endif