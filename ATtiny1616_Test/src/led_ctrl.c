/*
 * led_ctrl.c
 *
 * Created: 16.10.2023 22:20:23
 *  Author: flori
 */ 
#include <led_ctrl.h>

static uint8_t die_number = 1;

#if MAX_DIE_NUMBER == 7
	static uint8_t die_matrix[7][7] = { // [NUMBER][LED_INDEX]
		{5, 0, 0, 0, 0, 0, 0},
		{2, 6, 0, 0, 0, 0, 0},
		{1, 7, 5, 0, 0, 0, 0},
		{1, 2, 6, 7, 0, 0, 0},
		{1, 2, 5, 6, 7, 0, 0},
		{1, 2, 3, 4, 6, 7, 0},
		{1, 2, 3, 4, 5, 6, 7}
	};
#elif MAX_DIE_NUMBER == 6
	static uint8_t die_matrix[6][6] = { // [NUMBER][LED_INDEX]
		{5, 0, 0, 0, 0, 0},
		{2, 6, 0, 0, 0, 0},
		{1, 7, 5, 0, 0, 0},
		{1, 2, 6, 7, 0, 0},
		{1, 2, 5, 6, 7, 0},
		{1, 2, 3, 4, 6, 7}
	};
#else
	#error Unsupported MAX_DIE_NUMBER
#endif

void set_die_number(uint8_t number){
	die_number = number;
}

void set_single_led(uint8_t led_index)
{
	switch (led_index)
	{
		case 0:
		PORTA.DIRCLR = LED_A | LED_B;
		PORTB.DIRCLR = LED_C | LED_D;
		break;

		case 1:
		PORTB.DIRCLR = LED_C | LED_D;

		// D2 ON
		PORTA.DIRSET = LED_A | LED_B;
		PORTA.OUTCLR = LED_A;
		PORTA.OUTSET = LED_B;
		break;

		case 2:
		PORTB.DIRCLR = LED_C | LED_D;

		// D3 ON
		PORTA.DIRSET = LED_A | LED_B;
		PORTA.OUTCLR = LED_B;
		PORTA.OUTSET = LED_A;
		break;

		case 3:
		PORTA.DIRCLR = LED_A;
		PORTB.DIRCLR = LED_D;

		// D4 ON
		PORTA.DIRSET = LED_B;
		PORTB.DIRSET = LED_C;
		PORTA.OUTCLR = LED_B;
		PORTB.OUTSET = LED_C;
		break;

		case 4:
		PORTA.DIRCLR = LED_A;
		PORTB.DIRCLR = LED_D;

		// D5 ON
		PORTA.DIRSET = LED_B;
		PORTB.DIRSET = LED_C;
		PORTB.OUTCLR = LED_C;
		PORTA.OUTSET = LED_B;
		break;

		case 5:
		PORTA.DIRCLR = LED_B;
		PORTB.DIRCLR = LED_C;

		// D6 ON
		PORTA.DIRSET = LED_A;
		PORTB.DIRSET = LED_D;
		PORTB.OUTCLR = LED_D;
		PORTA.OUTSET = LED_A;
		break;

		case 6:
		PORTA.DIRCLR = LED_A | LED_B;

		// D7 ON
		PORTB.DIRSET = LED_D | LED_C;
		PORTB.OUTCLR = LED_D;
		PORTB.OUTSET = LED_C;
		break;

		case 7:
		PORTA.DIRCLR = LED_A | LED_B;

		// D8 ON
		PORTB.DIRSET = LED_D | LED_C;
		PORTB.OUTCLR = LED_C;
		PORTB.OUTSET = LED_D;
		break;
	}
}

void die_timer_routine(void){
	static uint8_t index = 0;
	
	set_single_led(die_matrix[die_number - 1][index]);
	if (++index > (MAX_DIE_NUMBER - 1))
		index = 0;
}