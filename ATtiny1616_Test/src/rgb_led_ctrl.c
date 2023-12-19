/*
 * rgb_led_ctrl.c
 *
 * Created: 17.11.2023 16:51:53
 *  Author: Florian
 */ 
#include <rgb_led_ctrl.h>

void clear_all(void){
	PORTB.OUTCLR = PIN3_bm;
	PORTC.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
	PORTA.OUTCLR = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
}

void set_single_rgb_led(uint8_t led_index, char rgb){
	switch (rgb)
	{
		case 'r':
		PORTB.OUTSET = PIN1_bm | PIN2_bm;
		PORTB.OUTCLR = PIN0_bm;
		break;
		
		case 'g':
		PORTB.OUTSET = PIN0_bm | PIN2_bm;
		PORTB.OUTCLR = PIN1_bm;
		break;
		
		case 'b':
		PORTB.OUTSET = PIN0_bm | PIN1_bm;
		PORTB.OUTCLR = PIN2_bm;
		break;
	}
	
	clear_all();
	
	switch (led_index)
	{
		case 1:
		LED1_EN_set_level(1);
		break;
		
		case 2:
		LED2_EN_set_level(1);
		break;
		
		case 3:
		LED3_EN_set_level(1);
		break;
		
		case 4:
		LED4_EN_set_level(1);
		break;
		
		case 5:
		LED5_EN_set_level(1);
		break;
		
		case 6:
		LED6_EN_set_level(1);
		break;
		
		case 7:
		LED7_EN_set_level(1);
		break;
		
		case 8:
		LED8_EN_set_level(1);
		break;
		
		case 9:
		LED9_EN_set_level(1);
		break;
	}
}