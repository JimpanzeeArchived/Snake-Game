#ifndef MAX7219_LIB_H
#define MAX7219_LIB_H

void setup_7219();
void SPI_setup();
void disable_decoder_7219();
void scan_list_7219();
void display_on();
void intensity_7219(unsigned char value);
void write_7219(unsigned char reg, unsigned char data);
void update_7219();
void write_w_delay_7219(unsigned char rows, unsigned char columns);
void test_individual_7219();
void write_horizontal_test_7219(unsigned char rows);
void test_horizontal_7219();
void write_vertical_test_7219(unsigned char column);
void test_vertical_7219();

#endif

//#include <util/delay.h>
//#ifndef DELAY_H
//#define <util/delay.h>
//endif
