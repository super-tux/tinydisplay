/*
 * main.c
 *
 *  Created on: Apr 4, 2014
 *      Author: valentin
 */


/*
 * pin config:
 *
 * Dig.1 Vcc -- PD5
 * Dig.2 Vcc -- PD3
 * Dig.3 Vcc -- PD1
 * Dig.4 Vcc -- PD0
 *
 * Seg.a -- PB5
 * Seg.b -- PB7
 * Seg.c -- PB4
 * Seg.d -- PA1
 * Seg.e -- PD2
 * Seg.f -- PB2
 * Seg.g -- PB6
 * DP	 --	PD4
 *
 * middle Vcc -- PA0
 * middle Gnd -- PB3
 *
 * top right Vcc -- PB1
 * top right Gnd -- PB0
 *
 *   __
 *  |a |
 * f|__|b
 *  |g |
 * e|__|c
 *   d
 */

#include<avr/io.h>
#include<util/delay.h>

// output values for different characters
// port A
const uint8_t charactersA[16] = {
	0b00000000,	// 0
	0b00000010,	// 1
	0b00000000,	// 2
	0b00000000,	// 3
	0b00000010,	// 4
	0b00000000,	// 5
	0b00000000,	// 6
	0b00000010,	// 7
	0b00000000,	// 8
	0b00000000,	// 9
	0b00000010,	// A
	0b00000000,	// b
	0b00000000,	// C
	0b00000000,	// d
	0b00000000,	// E
	0b00000010	// F
};
// port B
const uint8_t charactersB[16] = {
	0b01000000,	// 0
	0b01100100,	// 1
	0b00010100,	// 2
	0b00000100, 	// 3
	0b00100000,	// 4
	0b10000000,	// 5
	0b10000000,	// 6
	0b01000100,	// 7
	0b00000000,	// 8
	0b00000000,	// 9
	0b00000000,	// A
	0b10100000,	// b
	0b11010000,	// C
	0b00100100,	// d
	0b10010000,	// E
	0b10010000	// F
};
// port D
const uint8_t charactersD[16] = {
	0b00010000,	// 0
	0b00010100,	// 1
	0b00010000,	// 2
	0b00010100,	// 3
	0b00010100,	// 4
	0b00010100,	// 5
	0b00010000,	// 6
	0b00010100,	// 7
	0b00010000,	// 8
	0b00010100,	// 9
	0b00010000,	// A
	0b00010000,	// b
	0b00010000,	// C
	0b00010000,	// d
	0b00010000,	// E
	0b00010000	// F
};

// the 4 digits of the display
// this is want actually will be displayed
int digit_arr[4] = {0x0,0x0,0x0,0x0};

void disp(){
	// each member of the array represents one pin on
	// wich is connected to one of the common cathodes/anodes of the digits
	pin_arr[4] = {
		0b00000001,	// pin 0
		0b00000010,	// pin 1
		0b00001000,	// pin 3
		0b00100000	// pin 5
	};
	for(int i = 0; i < 4; i++){
		PORTD = 0x00;
		PORTA = charactersA[digit_arr[i]];
		PORTB = charactersB[digit_arr[i]];
		PORTD = charactersD[digit_arr[i]];
		// turn on each one of the common pins one after another
		PORTD |= pin_arr[i];
	}
}

uint8_t key_pressed (volatile uint8_t *inputreg, uint8_t inputbit)
{
	static uint8_t last_state = 0;

	if (last_state == (*inputreg & (1<<inputbit)))
		return 0; // no change

	// remeber state for next call of the function
	last_state = *inputreg & (1<<inputbit);

	// return proper keypress
	return *inputreg & (1<<inputbit);
}

// turn a given value into a certain format (e.g. format(42, 2) -> 10101) and write it into an array
void format(uint16_t value, int format){
	int r = 0,
		c = 3;
	while(value > 0){
		r = value % format;
		value = (int)value / format;
		digit_arr[c] = r;
		c--;
	}
}

int main(void){
	// eneable output pins
	DDRB = 0b11110100;
	DDRA = 0b00000010;
	DDRD = 0b00111111;


	// sample program counting clicks in decimal format

	uint16_t count = 0;

	while(1){
		// check if Pin D6 is pressed
		if(key_pressed(&PIND, PD6)){
			count++;
		}
		// turn the counter into the right format
		format(count, 10);
		// refresh the display
		disp();
		_delay_ms(10);
	}
}
