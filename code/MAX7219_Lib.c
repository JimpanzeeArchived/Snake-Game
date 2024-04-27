/*This libary is to control the MAX7219 using the Atmega328p IC
Connect the MAX7219 according to the schematic
  

**** NOTE: This library is depended on the SPI_Lib library.
 Include the library in the same folder for it to work****
*/

#include "MAX7219_Lib.h"

void setup_7219(){
	PORTB &= ~(0x04); //set SS to low at the beginning
	SPI_setup();	 //set up SPI 
	disable_decoder_7219(); //disable all decoder for all 8 segments 
	scan_list_7219();
	display_on();
	intensity_7219(0x01);
}

void SPI_setup(){
	//default setting of SPI
	DDRB = (DDRB & 0xC3) | 0x2C | 0x1; //set MISO and SCK to be output
	//DDRB AND 1100_0011 (C3) --> xx00_00xx OR 0010_1100 (2C) --> xx10 11xx
	SPCR = (1 << SPE) | (1 << MSTR) | (0 << DORD) | (1 << SPR0);
	//enable SPI communication, master mode, and speed to be f/16
	SPSR = SPSR; //| (1 << SPI2X);
}

void disable_decoder_7219(){
	/*Disable decoder for all eight digits*/
	write_7219(0x9, 0b0); update_7219();
}
void scan_list_7219(){
	/*Set scan list to scan all eight digits*/
	write_7219(0xB, 0x7); update_7219();
}
void display_on(){
	/*Turn on the display*/
	write_7219(0xC, 0x1); update_7219();
}

void intensity_7219(unsigned char value){
	/*Take in intensity value for light
	There are 3 level of intensity:  LOW, MEDIUM, HIGH */
	write_7219(0xA, value); update_7219();
}

void write_7219(unsigned char reg, unsigned char data){
	/*First input is register
	Second input is data
	Function must be followed by 
	update_7219() for command to be loaded*/
	SPDR = reg;
	while(!(SPSR & (1 << SPIF))); //wait for transmission bit to be set
	unsigned char rData = SPDR;   //read from the data register to clear the bit
	//_delay_ms(5);
	SPDR = data;
	while(!(SPSR & (1 << SPIF))); //wait for transmission bit to be set
	rData = SPDR;   //read from the data register to clear the bit
	//PORTB |= 0x04;
}

void update_7219(){
	/*This command follows the write instruction
	Because MAX7219 is written on the positive edge*/ 
	PORTB |= 0x04;
	PORTB &= ~(0x04); 
}

void write_w_delay_7219(unsigned char rows, unsigned char columns){
	/*This is setup create a 135 ms delay*/
	//The clock is running as f/1024 speed
	TCCR0A = 0;  //stop the clock in the beginning
	TCNT0 = 0xBE;  //load 0 into timer
	TCCR0B = 0x5;  //choose f/1024 mode
	while((TIFR0 & (1<<TOV0)) == 0){ // while flag is zero
		write_7219(rows, columns); update_7219();
	}
	TCCR0B = 0b0;  //stop the clock
	TIFR0 = 0x01; //reset the flag

}

void test_individual_7219(){
	//test individual bits
	int temp = 0;
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			temp = 1 << j; //shift left by j bit
			write_7219(i+1, temp); update_7219();
			_delay_ms(7);
		}
		write_7219(i+1, 0x0); update_7219();
		_delay_ms(7);
	}
}
void write_horizontal_test_7219(unsigned char rows){
	//Test horizontal bits
	write_7219(rows, 0xff); update_7219();
	_delay_ms(7);
	write_7219(rows, 0x00); update_7219();
}


void test_horizontal_7219(){
	//test rows of led
	for(int i = 0; i < 8; i++){
		write_horizontal_test_7219(i + 1);
	}
	write_horizontal_test_7219(0x0);
}

void write_vertical_test_7219(unsigned char column){
		for(int i = 0; i < 8; i++){
			write_7219(i+1, column); update_7219();
			_delay_ms(7);
		}
}

void test_vertical_7219(){
	//test column of led
	for(int i = 0; i < 8; i++){
		write_vertical_test_7219(1<<i);
	}
	write_vertical_test_7219(0);
}

