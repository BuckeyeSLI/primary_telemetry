/* main.c
 *
 * Created     : 10/03/2017 3:24:51 PM
 * Author      : Andrew Bennett
 * Device      : ATXMEGA32A4U
 * Description :
 *		This code will configure the ATXMEGA for communication with the BMX055 and BMP280, and then poll these sensors for data and forward
 *		it to MAX3227 for transmission to the primary telemetry computer. While this system will never fly in this configuration, this code
 *		will serve as the base upon which the unique sensor clusters are developed.
 */ 

#define F_CPU 32000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"
#include "message_encoder_decoder.h"

// BMP280 I/O functions
void BMP280_Write(uint8_t address, uint8_t data);
uint8_t BMP280_Read(uint8_t address);
void BMP280_MultiRead(uint8_t startAddress, uint8_t numRegisters, uint8_t data[]);

// BMP055 I/O functions
void BMX055_Write(uint8_t sensor, uint8_t address, uint8_t data);
uint8_t BMX055_Read(uint8_t sensor, uint8_t address);
void BMX055_MultiRead(uint8_t sensor, uint8_t startAddress, uint8_t numRegisters, uint8_t data[]);
/*
// Primary Telemetry Computer RS232 I/O functions
void RS232_InputBuffer_Add(uint8_t data);
void RS232_OutputBuffer_Add(uint8_t data);
void RS232_OutputBuffer_MultiAdd(uint8_t data[], uint8_t size);
void RS232_OutputBuffer_Send();
*/
int main(void)
{
    // --------------------- Clock Configuration ---------------------
	// Enable 32 MHz clock and 32 kHz clock
	OSC.CTRL = OSC.CTRL | OSC_RC32MEN_bm | OSC_RC32KEN_bm;
	// Wait for clocks to stabilize
	while(OSC.STATUS != (OSC.STATUS | OSC_RC32MRDY_bm | OSC_RC32KRDY_bm));
	// Set CPU clock to 32 MHz clock
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
	// Enable 32 MHz clock DFLL
	DFLLRC32M.CTRL = DFLL_ENABLE_bm;
	// Disable 2 MHz clock
	OSC.CTRL = OSC.CTRL & ~OSC_RC2MEN_bm;

	// --------------------- I/O Pins Configuration ---------------------
	// Setup SPI SS pins on port A
	PORTA.DIRSET = 0x1E;
	PORTA.OUTSET = 0x1E;
	// Setup USARTC0 and SPIC pins on port C
	PORTC.DIRSET = 0xB8;
	PORTC.OUTSET = 0x38;

	/* --------------------- PMIC Configuration --------------------- */
	// Enable medium and low priority interrupts
	PMIC.CTRL = PMIC_LOLVLEX_bm | PMIC_MEDLVLEN_bm;

	/* --------------------- SPI Configuration --------------------- */
	// Enable SPIC as master, mode 0, and set speed to 4 MHz
	SPIC.CTRL = SPI_CLK2X_bm | SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESCALER0_bm;

	/* --------------------- USART Configuration --------------------- */
	// Set USARTC0 receive interrupt to medium priority and USARTC0 transmit interrupt to low priority
	USARTC0.CTRLA = USART_RXCINTLVL_MED_gc | USART_TXCINTLVL_LO_gc;
	// Set packet size to 8 bits
	USARTC0.CTRLC = USART_CHSIZE_8BIT_gc;
	// Set baud rate to 115.2 kbs
	USARTC0.BAUDCTRLA = 0x83;
	USARTC0.BAUDCTRLB = 0x0D << 4;
	// Enable transmit and receive pins
	USARTC0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	// Note that the USART defaults to no parity bit and 1 stop bit
	
	/* --------------------- Sensor Presence Tests --------------------- */
	// In order to check that each sensor is active and not dead/malfunctioning, verify the chip id of each sensor
	uint8_t BMP280_alive = 0;
	uint8_t	BMX055_accel_alive = 0;
	uint8_t BMX055_gyro_alive = 0;
	uint8_t BMX055_magnt_alive = 0;
	// Test BMP280
	if(BMP280_Read(0xD0) == 0x58) BMP280_alive = 1;
	// Test BMX055 Accelerometer
	if(BMX055_Read(0,0x00) == 0xFA) BMX055_accel_alive = 1;
	// Test BMX055 Gyroscope
	if(BMX055_Read(1,0x00) == 0x0F) BMX055_gyro_alive = 1;
	// Test BMX055 Magnetometer
	if(BMX055_Read(2,0x40) == 0x32) BMX055_magnt_alive = 1;
	// Report status to Primary Telemetry Computer
	//RS232_OutputBuffer_Add(0x00);
	//RS232_OutputBuffer_Add(BMP280_alive | (BMX055_accel_alive << 1) | (BMX055_gyro_alive << 2) | (BMX055_magnt_alive << 3));

	/* --------------------- BMP280 Configuration --------------------- */
	if(BMP280_alive)
	{
		// Set barometer oversampling mode to high resolution (x8) and enable temperature sensor with no oversampling
		BMP280_Write(0xF4,0x33);
	}

	/* --------------------- BMX055 Configuration --------------------- */
	if(BMX055_accel_alive)
	{
		// Set accelerometer bandwidth to 125 Hz (probably ok?)
		BMX055_Write(0,0x10,0x0C);
		// Set accelerometer range to +/- 16 g
		BMX055_Write(0,0x0F,0x0C);
		// Shut off accelerometer slow calibration in the z-axis because this definitely won't like constant acceleration
		BMX055_Write(0,0x36,0x03);
	}
	if(BMX055_gyro_alive)
	{
		// Set gyroscope output data rate to 200 Hz and filter bandwidth to 64 Hz (probably ok?)
		BMX055_Write(1,0x10,0x06);
		// Set gyroscope range to 500 degrees/s
		BMX055_Write(1,0x0F,0x02);
	}
	if(BMX055_magnt_alive)
	{
		// Set magnetometer x/y repetition to 47 and z repetition to 83 
		BMX055_Write(2,0x51,0x1B);
		BMX055_Write(2,0x52,0x29);
		// Set magnetometer output data rate to 20 Hz
		BMX055_Write(2,0x4C,0x23);
	}

	/* --------------------- Program State Definitions --------------------- */
	// These variables are used to control what the microcontroller will do in the main loop
	uint8_t global_en = 0;
	uint8_t	BMP280_en = BMP280_alive;
	uint8_t BMX055_accel_en = BMX055_accel_alive;
	uint8_t BMX055_gyro_en = BMX055_gyro_alive;
	uint8_t BMX055_magnt_en = BMX055_magnt_alive;

	// Enable interrupts
	sei();

	while(1)
	{
		global_en = 1;
	}

	// Begin normal operation
    while(1)
	{
		if(global_en && BMP280_en)
		{
			uint8_t pressure_data[6];
			BMP280_MultiRead(0xF7,6,pressure_data);
			//RS232_OutputBuffer_Add(0x01);
			//RS232_OutputBuffer_MultiAdd(pressure_data,6);
		}
		if(global_en && BMX055_accel_en)
		{
			uint8_t acceleromter_data[7];
			BMX055_MultiRead(0,0x02,7,acceleromter_data);
			acceleromter_data[0] &= ~0x01;
			acceleromter_data[2] &= ~0x01;
			acceleromter_data[4] &= ~0x01;
			//RS232_OutputBuffer_Add(0x02);
			//RS232_OutputBuffer_MultiAdd(acceleromter_data,7);
		}
		if(global_en && BMX055_gyro_en)
		{
			uint8_t gyroscope_data[6];
			BMX055_MultiRead(1,0x02,6,gyroscope_data);
			//RS232_OutputBuffer_Add(0x03);
			//RS232_OutputBuffer_MultiAdd(gyroscope_data,6);
		}
		if(global_en && BMX055_magnt_en)
		{
			uint8_t magnetometer_data[8];
			BMX055_MultiRead(1,0x42,8,magnetometer_data);
			magnetometer_data[0] &= ~0x01;
			magnetometer_data[2] &= ~0x01;
			magnetometer_data[4] &= ~0x01;
			magnetometer_data[6] &= ~0x01;
			//RS232_OutputBuffer_Add(0x04);
			//RS232_OutputBuffer_MultiAdd(magnetometer_data,8);
		}
		//RS232_OutputBuffer_Send();
    }
	
}

// USART0 receive complete interrupt subroutine
ISR(USARTC0_RXC_vect)
{
	uint8_t data = USARTC0.DATA;
	USART_InputBufferAdd(0,data);
	//TODO: Error handling for empty buffer
}

// USART0 transmit complete interrupt subroutine
ISR(USARTC0_TXC_vect)
{
	USART_SendByte(0);
	//TODO: Error handling for failed send (can an error even be reported if the send is FUBAR'd?)
}

// Writes one byte to the specified register of the BMP280
void BMP280_Write(uint8_t address, uint8_t data)
{
	// Enable BMP280 SS
	PORTA.OUTCLR = 1 << 1;
	// Send register address
	SPIC.DATA = address;
	// Wait for send to complete
	while(!(SPIC.STATUS & 0x80));
	// Send data
	SPIC.DATA = data;
	// Wait for send to complete
	while(!(SPIC.STATUS & 0x80));
	// Disable BMX055 SS
	PORTA.OUTSET = 1 << 1;

	return;
}

// Reads the specified register of the BMP280
uint8_t BMP280_Read(uint8_t address)
{
	uint8_t data;

	// Enable BMP280 SS
	PORTA.OUTCLR = 1 << 1;
	// Send register address and read bit
	SPIC.DATA = address | 0x80;
	// Wait for send to complete
	while(!(SPIC.STATUS & 0x80));
	// Receive data
	SPIC.DATA = 0x00;
	// Wait for receive to complete
	while(!(SPIC.STATUS & 0x80));
	// Read received data
	data = SPIC.DATA;
	// Disable BMX055 SS
	PORTA.OUTSET = 1 << 1;

	return data;
}

// Reads the specified registers of the BMP280 in a single read cycle
void BMP280_MultiRead(uint8_t startAddress, uint8_t numRegisters, uint8_t data[])
{
	// Enable BMP280 SS
	PORTA.OUTCLR = 1 << 1;
	// Send register address and read bit
	SPIC.DATA = startAddress | 0x80;
	// Wait for send to complete
	while(!(SPIC.STATUS & 0x80));
	// Begin multiple register read
	for(uint8_t i = 0; i < numRegisters; i++)
	{
		SPIC.DATA = 0x00;
		while(!(SPIC.STATUS & 0x80));
		data[i] = SPIC.DATA;
	}
	// Disable BMP280 SS
	PORTA.OUTSET = 1 << 1;

	return;
}

// Writes one byte to the specified sensor and register of the BMX055 (0 = accel, 1 = gyro, 2 = magnt)
void BMX055_Write(uint8_t sensor, uint8_t address, uint8_t data)
{
	// Enable BMX055 SS
	if(sensor == 0) PORTA.OUTCLR = 1 << 2;
	else if(sensor == 1) PORTA.OUTCLR = 1 << 3;
	else if(sensor == 2) PORTA.OUTCLR = 1 << 4;
	else return;
	// Send register address
	SPIC.DATA = address;
	// Wait for send to complete
	while(!(SPIC.STATUS & 0x80));
	// Send data
	SPIC.DATA = data;
	// Wait for send to complete
	while(!(SPIC.STATUS & 0x80));
	// Disable BMX055 SS
	PORTA.OUTSET = 0x1C;

	return;
}

// Reads the specified register from the specified sensor of the BMX055 (0 = accel, 1 = gyro, 2 = magnt)
uint8_t BMX055_Read(uint8_t sensor, uint8_t address)
{
	uint8_t data;

	// Enable BMX055 SS
	if(sensor == 0) PORTA.OUTCLR = 1 << 2;
	else if(sensor == 1) PORTA.OUTCLR = 1 << 3;
	else if(sensor == 2) PORTA.OUTCLR = 1 << 4;
	else return 0;
	// Send register address and read bit
	SPIC.DATA = address | 0x80;
	// Wait for send to complete
	while(!(SPIC.STATUS & 0x80));
	// Receive data
	SPIC.DATA = 0x00;
	// Wait for receive to complete
	while(!(SPIC.STATUS & 0x80));
	// Read received data
	data = SPIC.DATA;
	// Disable BMX055 SS
	PORTA.OUTSET = 0x1C;

	return data;
}

// Reads the specified registers of the BMX055 in a single read cycle (0 = accel, 1 = gyro, 2 = magnt)
void BMX055_MultiRead(uint8_t sensor, uint8_t startAddress, uint8_t numRegisters, uint8_t data[])
{
	// Enable BMX055 SS
	if(sensor == 0) PORTA.OUTCLR = 1 << 2;
	else if(sensor == 1) PORTA.OUTCLR = 1 << 3;
	else if(sensor == 2) PORTA.OUTCLR = 1 << 4;
	else return;
	// Send register address and read bit
	SPIC.DATA = startAddress | 0x80;
	// Wait for send to complete
	while(!(SPIC.STATUS & 0x80));
	// Begin multiple register read
	for(uint8_t i = 0; i < numRegisters; i++)
	{
		SPIC.DATA = 0x00;
		while(!(SPIC.STATUS & 0x80));
		data[i] = SPIC.DATA;
	}
	// Disable BMX055 SS
	PORTA.OUTSET = 0x1C;

	return;
}
/*
// Adds one item to the RS232 receive buffer
void RS232_InputBuffer_Add(uint8_t data)
{
	RS232_input_index++;
	RS232_input[RS232_input_index] = data;
	return;
}

// Adds one item to the RS232 transmit buffer
void RS232_OutputBuffer_Add(uint8_t data)
{
	RS232_output_index++;
	RS232_output[RS232_output_index] = data;
	return;
}

// Adds multiple items to the RS232 transmit buffer
void RS232_OutputBuffer_MultiAdd(uint8_t data[], uint8_t size)
{
	for(uint8_t i = 0; i < size; i++)
	{
		RS232_OutputBuffer_Add(data[i]);
	}
	return;
}

// Transmits all items in the RS232 transmit buffer via USARTC0
void RS232_OutputBuffer_Send()
{
	for(uint8_t i = 0; i <= RS232_output_index; i++)
	{
		while(!(USARTC0.STATUS & 0x10));
		USARTC0.DATA = RS232_output[i];
	}
	RS232_output_index = 0;
	return;
}
*/