/*
 * Thermostat.c
 *
 * Created: 21-03-19 20:14:42
 * Author : Thierry
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

int currentMenuItem = 0;

void init_lcd() {
	glcd_init();
	glcd_led_on();
	glcd_setOrientation(1);	
}

void init_keypad() {
	DDRC =  0b00000001;
}

void display_shell() {
	glcd_rectangle(10, 10, 300, 150, 0xFFFF); /* Top frame */
	glcd_rectangle(10, 170, 300, 50, 0xFFFF); /* Bottom frame */
}

void display_menu(int menuItem) {
	switch(menuItem) {
		case 0:		
		glcd_fillRectangle(15, 175, 60, 40, 0x07E0);
		glcd_fillRectangle(80, 175, 110, 40, 0x0000);
		glcd_fillRectangle(195, 175, 100, 40, 0x0000);
		break;
		case 1:
		glcd_fillRectangle(15, 175, 60, 40, 0x0000);
		glcd_fillRectangle(80, 175, 110, 40, 0x07E0);
		glcd_fillRectangle(195, 175, 100, 40, 0x0000);
		break;
		case 2:
		glcd_fillRectangle(15, 175, 60, 40, 0x0000);
		glcd_fillRectangle(80, 175, 110, 40, 0x0000);
		glcd_fillRectangle(195, 175, 100, 40, 0x07E0);
		break;
	}
	
	
	glcd_rectangle(15, 175, 60, 40, 0xFFFF); /* Home frame */
	glcd_rectangle(80, 175, 110, 40, 0xFFFF); /* Calendar frame */
	glcd_rectangle(195, 175, 100, 40, 0xFFFF); /* Launch frame */
	
	glcd_string("Home", 20, 180, 2, 0xFFFF); /* Home item */
	glcd_string("Calendar", 85, 180, 2, 0xFFFF); /* Calendar item */
	glcd_string("Launch", 200, 180, 2, 0xFFFF); /* Launch item */	
}

void display_home() {	
	glcd_string("Temperature : 25°C", 15, 15, 2, 0xFFFF);
}

void display_calendar() {
	
}

void display_launch() {
	
}

void display_current_menu() {
	display_menu(currentMenuItem);
	switch(currentMenuItem) {
		case 0:
			display_home();
		break;
		case 1:
			display_calendar();
		break;
		case 2:
			display_launch();
		break;
	}
}

void navigate_left() {
	if (currentMenuItem == 0) {
		currentMenuItem = 2;
	} else {
		currentMenuItem--;
	}
}

void navigate_right() {
	if (currentMenuItem == 2) {
		currentMenuItem = 0;
	} else {
		currentMenuItem++;
	}
}

int main(void)
{
	init_lcd();
	init_keypad();
		
	display_shell();
	display_current_menu();
	
    while (1)
    {	
		if ((PINC & (1 << PC1))) {
			_delay_ms(100);
			while(PINC & (1 << PC1));
			navigate_left();
			display_current_menu();
		} else if ((PINC & (1 << PC2))) {
			_delay_ms(100);
			while(PINC & (1 << PC2));
			navigate_right();
			display_current_menu();
		}	
		
		// Verifier si la temperature est < à attendue dans quel cas il faut lancer le boiler
		// 
    }
}