#include <atmel_start.h>
#include <util/delay.h>
#include <rgb_led_ctrl.h>
#include <avr/sleep.h>
#include <adc_basic.h>
#include <driver_init.h>
#include <compiler.h>
#include <stdlib.h>

#define DELAY 20
#define BLINK_COUNT 3

void goto_sleep(void);

volatile static uint8_t go_sleep_flag = 0;
volatile static uint8_t Blink, BlinkCount = 0;
volatile static uint8_t number = 1;

ISR(TCA0_CMP0_vect)
{
	/* Insert your TCA Compare 0 Interrupt handling code here */
	static uint16_t t1 = 0;
	static uint16_t t2 = 0;
	static uint8_t in_seq = 0;
	static uint8_t delay = DELAY;
	
	if (BTN_get_level() == 1 && in_seq == 0)
	{
		t2 = 0;
		if (++t1 >= 9){
			
			t1 = 0;
			
			if (++number > MAX_DIE_NUMBER){
				number = 1;
			}
			
			switch(get_die_color()){
				case 'r':
				set_die_color('g');
				break;
				case 'g':
				set_die_color('b');
				break;
				case 'b':
				set_die_color('r');
				break;
			}
			set_die_number(number);
		}
	}
	else if (BTN_get_level() == 0)
	{
		t1 = 0;
		// when button is released, start slowing down then blink the resulting number
		if (++t2 >= delay)
		{
			t2 = 0;

			if (delay <= (50 - DELAY))
			{
				if (++number > MAX_DIE_NUMBER){
					number = 1;
				}
				set_die_number(number);
				delay += 5;
			}
			else
			{
				if (++BlinkCount < (BLINK_COUNT * 2))
				{
					Blink ^= 1;
				}
				else
				{
					TCB0.CTRLA &= ~ (1 << TCB_ENABLE_bp);
					PORTB.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm;
					delay = DELAY;
					go_sleep_flag = 1;
				}
			}
		}
	}
	
	/* The interrupt flag has to be cleared manually */
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

ISR(TCB0_INT_vect)
{
	if (Blink == 0)
	{
		die_timer_routine();
	} 
	else {
		PORTB.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm;
	}
	
	TCB0.INTFLAGS = TCB_CAPT_bm;
}

ISR(PORTB_PORT_vect)
{
	/* Insert your PORTB interrupt handling code here */

	/* Clear interrupt flags */
	VPORTB_INTFLAGS = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	number = rand()%MAX_DIE_NUMBER+1;
	
	/* Replace with your application code */
	while (1) {
		if (go_sleep_flag)
		{
			goto_sleep();
		}
	}
}

void goto_sleep(void){
	
	ADC_0_disable();
	
	/* PORT setting on PA0 */
	PA0_set_level(false);
	PA0_set_dir(PORT_DIR_IN);
	PA0_set_pull_mode(PORT_PULL_UP);
	PA0_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PA1 */
	ADC_set_level(false);
	ADC_set_dir(PORT_DIR_IN);
	ADC_set_pull_mode(PORT_PULL_UP);
	ADC_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PA2 */
	ADC_EN_set_level(false);
	ADC_EN_set_dir(PORT_DIR_IN);
	ADC_EN_set_pull_mode(PORT_PULL_UP);
	ADC_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PA4 */
	LED1_EN_set_level(false);
	LED1_EN_set_dir(PORT_DIR_IN);
	LED1_EN_set_pull_mode(PORT_PULL_UP);
	LED1_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PA5 */
	LED2_EN_set_level(false);
	LED2_EN_set_dir(PORT_DIR_IN);
	LED2_EN_set_pull_mode(PORT_PULL_UP);
	LED2_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PA6 */
	LED3_EN_set_level(false);
	LED3_EN_set_dir(PORT_DIR_IN);
	LED3_EN_set_pull_mode(PORT_PULL_UP);
	LED3_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PA7 */
	LED4_EN_set_level(false);
	LED4_EN_set_dir(PORT_DIR_IN);
	LED4_EN_set_pull_mode(PORT_PULL_UP);
	LED4_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PB3 */
	LED9_EN_set_level(false);
	LED9_EN_set_dir(PORT_DIR_IN);
	LED9_EN_set_pull_mode(PORT_PULL_UP);
	LED9_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PB4 */
	PB4_set_level(false);
	PB4_set_dir(PORT_DIR_IN);
	PB4_set_pull_mode(PORT_PULL_UP);
	PB4_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PB5 */
	BTN_set_level(false);
	BTN_set_dir(PORT_DIR_IN);
	BTN_set_pull_mode(PORT_PULL_OFF);
	BTN_set_isc(PORT_ISC_BOTHEDGES_gc);

	/* PORT setting on PC0 */
	LED5_EN_set_level(false);
	LED5_EN_set_dir(PORT_DIR_IN);
	LED5_EN_set_pull_mode(PORT_PULL_UP);
	LED5_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PC1 */
	LED6_EN_set_level(false);
	LED6_EN_set_dir(PORT_DIR_IN);
	LED6_EN_set_pull_mode(PORT_PULL_UP);
	LED6_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PC2 */
	LED7_EN_set_level(false);
	LED7_EN_set_dir(PORT_DIR_IN);
	LED7_EN_set_pull_mode(PORT_PULL_UP);
	LED7_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PC3 */
	LED8_EN_set_level(false);
	LED8_EN_set_dir(PORT_DIR_IN);
	LED8_EN_set_pull_mode(PORT_PULL_UP);
	LED8_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PB0 */
	R_EN_set_level(false);
	R_EN_set_dir(PORT_DIR_IN);
	R_EN_set_pull_mode(PORT_PULL_UP);
	R_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PB1 */
	G_EN_set_level(false);
	G_EN_set_dir(PORT_DIR_IN);
	G_EN_set_pull_mode(PORT_PULL_UP);
	G_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PB2 */
	B_EN_set_level(false);
	B_EN_set_dir(PORT_DIR_IN);
	B_EN_set_pull_mode(PORT_PULL_UP);
	B_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);
	
	sleep_cpu();
	
	go_sleep_flag = 0;
	atmel_start_init();
	
	/************************************************************************/
	/* TODO: set random seed, read adc input on unused pins                 */
	/************************************************************************/
	number = rand() % MAX_DIE_NUMBER + 1;
	Blink = 0;
	BlinkCount = 0;
}

