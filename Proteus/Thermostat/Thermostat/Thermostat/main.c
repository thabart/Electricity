/*
 * Thermostat.c
 *
 * Created: 21-03-19 20:14:42
 * Author : Thierry
 */ 

#include <avr/io.h>

int main(void)
{
	glcd_init();
	glcd_led_on();
	
    /* Replace with your application code */
    while (1)
    {		
		char *str = "lili is sexy";
		glcd_string(str, 0, 0, 2, 0x07E0);
		// glcd_string(str,180,0,2,green);
    }
}

