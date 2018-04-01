/*
 * External Power Connector (EPC)
 * Created: 3/24/2018 7:51:11 PM
 * Author : Ethan Smith
 * Device: ATmega48A (with slight adjustments, can be used for any AT controller)
 * Description: This code sets a microcontroller 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// Interrupt Service Routine for INT0
ISR(INT0_vect) {
	_delay_ms(500) // Software debounce
	
	// Activate release of EPC
	PORTC |= 0xFF; //set output port high to activate release of EPC
}

int main(void)
{
    // Initialize pins
	DDRD |= 0b00000000; // Set port D pins as input
	DDRC |= 0b00000001; // Set port C pins as output
	
	EICRA |= 0b00000011; // Set External Interrupt Control Register A to allow INT0 to make an interrupt request on the rising edge of INT0
	// TODO does the mask need to be set?
	EIMSK |= 0b00000001; // Set External Interrupt Mask Register to enable the external pin interrupt

	sei(); // Enable global interrupt
	
	sleep_enable(); // Enter sleep, waiting for interrupt
}

