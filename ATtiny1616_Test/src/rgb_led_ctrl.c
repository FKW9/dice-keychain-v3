/*
 * rgb_led_ctrl.c
 *
 * Created: 17.11.2023 16:51:53
 *  Author: Florian
 */ 
#include <rgb_led_ctrl.h>

static uint8_t dice_number = 1;
static char dice_color = 'r';

#if MAX_DICE_NUMBER == 9
static uint8_t dice_matrix[9][9] = { // [NUMBER][LED_INDEX]
	{5, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 9, 0, 0, 0, 0, 0, 0, 0},
	{3, 5, 7, 0, 0, 0, 0, 0, 0},
	{1, 3, 7, 9, 0, 0, 0, 0, 0},
	{1, 3, 5, 7, 9, 0, 0, 0, 0},
	{1, 3, 4, 6, 7, 9, 0, 0, 0},
	{1, 3, 4, 6, 7, 9, 5, 0, 0},
	{1, 2, 3, 4, 6, 7, 8, 9, 0},
	{1, 2, 3, 4, 5, 6, 7, 8, 9}
};
#elif MAX_DICE_NUMBER == 8
static uint8_t dice_matrix[8][8] = { // [NUMBER][LED_INDEX]
	{5, 0, 0, 0, 0, 0, 0, 0},
	{1, 9, 0, 0, 0, 0, 0, 0},
	{3, 5, 7, 0, 0, 0, 0, 0},
	{1, 3, 7, 9, 0, 0, 0, 0},
	{1, 3, 5, 7, 9, 0, 0, 0},
	{1, 3, 4, 6, 7, 9, 0, 0},
	{1, 3, 4, 6, 7, 9, 5, 0},
	{1, 2, 3, 4, 6, 7, 8, 9}
};
#elif MAX_DICE_NUMBER == 7
static uint8_t dice_matrix[7][7] = { // [NUMBER][LED_INDEX]
	{5, 0, 0, 0, 0, 0, 0},
	{1, 9, 0, 0, 0, 0, 0},
	{3, 5, 7, 0, 0, 0, 0},
	{1, 3, 7, 9, 0, 0, 0},
	{1, 3, 5, 7, 9, 0, 0},
	{1, 3, 4, 6, 7, 9, 0},
	{1, 3, 4, 6, 7, 9, 5}
};
#elif MAX_DICE_NUMBER == 6
static uint8_t dice_matrix[6][6] = { // [NUMBER][LED_INDEX]
	{5, 0, 0, 0, 0, 0},
	{1, 9, 0, 0, 0, 0},
	{3, 5, 7, 0, 0, 0},
	{1, 3, 7, 9, 0, 0},
	{1, 3, 5, 7, 9, 0},
	{1, 3, 4, 6, 7, 9}
};
#else
#error Unsupported MAX_DICE_NUMBER
#endif

uint8_t get_dice_number(void){
	return dice_number;
}

void set_dice_number(uint8_t number){
	dice_number = number;
}

char get_dice_color(void){
	return dice_color;
}

void set_dice_color(char color){
	dice_color = color;
}

void dice_timer_routine(void){
	static uint8_t index = 0;
	
	set_single_rgb_led(dice_matrix[dice_number - 1][index], dice_color);
	if (++index > (MAX_DICE_NUMBER - 1))
		index = 0;
}

void clear_all_led_pins(void){
	PORTB.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN5_bm;
	PORTC.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
	PORTA.OUTCLR = PIN5_bm | PIN6_bm | PIN7_bm;
}

void leds_off(void){
	PORTB.OUTSET = PIN1_bm | PIN2_bm | PIN0_bm;
}

void next_color(void){
	switch(dice_color){
		case 'r':
		set_dice_color('g');
		break;
		case 'g':
		set_dice_color('b');
		break;
		case 'b':
		set_dice_color('r');
		break;
	}
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
	
	PORTB.OUTCLR = PIN3_bm | PIN5_bm;
	PORTC.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
	PORTA.OUTCLR = PIN5_bm | PIN6_bm | PIN7_bm;
	
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