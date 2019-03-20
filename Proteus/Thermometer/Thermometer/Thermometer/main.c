/*
 * Thermometer.c
 *
 * Created: 19-03-19 13:23:56
 * Author : Thierry
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd.h"

#define RS 6
#define E  5

float firstAdc;
float secondAdc;
float firstAdcVolt;
float secondAdcVolt;
float temp = 0;
float previousTemp = 0;
char buffer[3];
char line[16];

void init_lcd() {	
	DDRC =  0xFF; /* PORTB output */
	DDRD = 0xFF; // DDRD = 0xE0;  /* LCD signals (RS, RW, E) */
}

void init_adc() 
{	
	ADCSRA = ( 1 << ADEN ) | /* ADC is turned on by setting this bit to logic 1 and turned off by writing logic 0. */
		// ( 1 << ADATE ) |
		( 1 << ADPS2) | ( 1 << ADPS1 ) | ( 1 << ADPS0 ); /* bits are used to set circuit clock frequency. = 128 */
	ADMUX = ( 1 << REFS0 );
}

float get_adc(uint8_t channel)
{
	ADMUX = ( 1 << REFS0 ) | channel;
	ADCSRA |= ( 1 << ADSC );
	while (!(ADCSRA & (1<<ADIF)));
	ADCSRA |= (1<<ADIF);
	return ADCW;
}

void send_command (unsigned char command)
{
	PORTD = 0x02;
	PORTC = command;
	_delay_ms(1);
	PORTD = 0x00;
	_delay_ms(100);
}

void send_character (unsigned char character) {
	PORTD = 0x03;
	PORTC = character;
	_delay_ms(1);
	PORTD = 0x01;
}

void disp_line(unsigned char* line) {
	for(unsigned int j = 0; line[j] != 0; j++) {
		send_character(line[j]);
	}
}

int main(void)
{
	init_lcd();
	init_adc();
	
	/* More information here : http://www.picaxe.com/docs/led008.pdf */	
	_delay_ms(50);
	
	// send_command(0x01); /* Clear command */
	send_command(0x38); /* 16 * 2 line LCD */
	send_command(0x0E); /* screen and cursor on */
	
	disp_line("Temperature");
	
    while (1) 
    {
		firstAdc = get_adc(PA3);
		secondAdc = get_adc(PA4);
		
		firstAdcVolt = (firstAdc * 5) / 1024;
		secondAdcVolt = (secondAdc * 5) / 1024;
		
		temp = (firstAdcVolt - secondAdcVolt) * 100;
		
		 if (previousTemp != temp) {			 
			 send_command(0x01);
			 
			 disp_line("Temperature");
			 send_command(0xC0); /* Second line */
			 
			 sprintf(line,"%.2f C", temp);
			 
			 disp_line(line);
			 
			 previousTemp = temp;
			 
			 _delay_ms(100);
		}
    }
}