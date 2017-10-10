/*
 * Base Sensor Board.c
 *
 * Created     : 10/03/2017 3:24:51 PM
 * Author      : Andrew Bennett
 * Device      : ATXMEGA32A4U
 * Description :
 *		This code will configure the ATXMEGA for communication with the BMX055 and BMP280, and then poll these sensors for data and forward
 *		it to MAX3227 for transmission to the primary telemetry computer. While this system will never fly in this configuration, this code
 *		will serve as the base upon which the unique sensor clusters are developed.
 */ 

#include <avr/io.h>

void BMP280_Write(uint8_t address, uint8_t data);
void BMP280_Read(uint8_t address, uint8_t *data);
void BMP280_MultiRead(uint8_t startAddress, uint8_t numRegisters, uint8_t data[]);

void BMX055_Write(uint8_t sensor, uint8_t address, uint8_t data);
void BMX055_Read(uint8_t sensor, uint8_t address, uint8_t *data);
void BMX055_MultiRead(uint8_t sensor, uint8_t startAddress, uint8_t numRegisters, uint8_t data[]);

void RS232_Send(uint8_t source, uint8_t length, uint8_t data[]);


int main(void)
{
    // --------------------- Clock Configuration ---------------------
	// Enable 32 MHz clock
	OSC.CTRL = OSC.CTRL | OSC_RC32MEN_bm;
	// Wait for 32 MHz clock to stabilize
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));
	// Set CPU clock to 32 MHz clock
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
	// Disable 2 MHz clock
	OSC.CTRL = OSC.CTRL & ~OSC_RC2MEN_bm;

	// --------------------- I/O Pins Configuration ---------------------
	// Setup SPI SS pins on port A
	PORTA.DIRSET = 0x1E;
	PORTA.OUTSET = 0x1E;
	// Setup USARTC0 and SPIC pins on port C
	PORTC.DIRSET = 0xB8;
	PORTC.OUTSET = 0x38;

	// --------------------- PMIC Configuration ---------------------
	// Enable medium and low priority interrupts
	PMIC.CTRL = PMIC_LOLVLEX_bm | PMIC_MEDLVLEN_bm;

	// --------------------- SPI Configuration ---------------------
	// Enable SPIC as master, mode 0, and set speed to 4 MHz
	SPIC.CTRL = SPI_CLK2X_bm | SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESCALER0_bm;

	// --------------------- USART Configuration ---------------------
	// Set USCARTC0 receive interrupt to medium priority
	USARTC0.CTRLA = USART_RXCINTLVL_MED_gc;
	// Set packet size to 8 bits
	USARTC0.CTRLC = USART_CHSIZE_8BIT_gc;
	// Set baud rate to 115.2 kbs
	USARTC0.BAUDCTRLA = 0x83;
	USARTC0.BAUDCTRLB = 0x0D << 4;
	// Enable transmit and receive pins
	USARTC0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
	// Note that the USART defaults to no parity bit and 1 stop bit
	
	// --------------------- BMP280 Configuration ---------------------
	// Set barometer oversampling mode to high resolution (x8) and enable temperature sensor with no oversampling
	BMP280_Write(0xF4,0x33);

	// --------------------- BMX055 Configuration ---------------------
	// Set accelerometer bandwidth to 125 Hz (probably ok?)
	BMX055_Write(0,0x10,0x0C);
	// Set accelerometer range to +/- 16 g
	BMX055_Write(0,0x0F,0x0C);
	// Shut off accelerometer slow calibration in the z-axis because this definitely won't like constant acceleration
	BMX055_Write(0,0x36,0x03);
	// Set gyroscope output data rate to 200 Hz and filter bandwidth to 64 Hz (probably ok?)
	BMX055_Write(1,0x10,0x06);
	// Set gyroscope range to 500 degrees/s
	BMX055_Write(1,0x0F,0x02);
	// Set magnetometer x/y repetition to 47 and z repetition to 83 
	BMX055_Write(2,0x51,0x1B);
	BMX055_Write(2,0x52,0x29);
	// Set magnetometer output data rate to 20 Hz
	BMX055_Write(2,0x4C,0x23);

    while(1)
	{

    }
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

// Transmits a message to the main telemetry computer
void RS232_Send(uint8_t source, uint8_t length, uint8_t data[])
{
	// Transmit message source
	USARTC0.DATA = source;

	// Wait for buffer to clear
	while(!(USARTC0.STATUS & 0x10));

	// Transmit message length
	USARTC0.DATA = length;

	// Transmit message data
	for(uint8_t i = 0; i < length; i++)
	{
		while(!(USARTC0.STATUS & 0x10));
		USARTC0.DATA = data[i];
	}
}