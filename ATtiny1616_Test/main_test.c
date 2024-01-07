#include <atmel_start.h>
#include <util/delay.h>
#include <rgb_led_ctrl.h>
#include <avr/sleep.h>
#include <adc_basic.h>
#include <driver_init.h>
#include <compiler.h>
#include <stdlib.h>

void goto_sleep(void);

volatile uint8_t number = 1;
volatile uint16_t _seed = 0;
volatile uint8_t flag = 0;

ISR(TCA0_CMP0_vect)
{
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

ISR(TCB0_INT_vect)
{
	dice_timer_routine();
}

ISR(PORTA_PORT_vect)
{
	static uint8_t state = 0;
	
	if (state == 0)
	{
		state = 1;
	} 
	else if (state == 1)
	{
		state = 0;
		_seed += (TCA0.SINGLE.CNT >> 2) + (TCB0.CNT >>1);
		srand(TCA0.SINGLE.CNT);
		flag = 1;
	}
	
	VPORTA_INTFLAGS = (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	/* Replace with your application code */
	while (1) {
		if(flag){
			number = rand() % MAX_DICE_NUMBER + 1;
			next_color();
			set_dice_number(number);
			_delay_ms(5);
			goto_sleep();
			flag=0;
		}



	}
}

void goto_sleep(void){
	
	ADC_0_disable();
	
	/* PORT setting on PB4 */
	PB4_set_level(false);
	PB4_set_dir(PORT_DIR_IN);
	PB4_set_pull_mode(PORT_PULL_UP);
	PB4_set_isc(PORT_ISC_INPUT_DISABLE_gc);
	
	/* PORT setting on PA0 */
	PA0_set_level(false);
	PA0_set_dir(PORT_DIR_IN);
	PA0_set_pull_mode(PORT_PULL_UP);
	PA0_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PA0 */
	PA3_set_level(false);
	PA3_set_dir(PORT_DIR_IN);
	PA3_set_pull_mode(PORT_PULL_UP);
	PA3_set_isc(PORT_ISC_INPUT_DISABLE_gc);

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
	
	atmel_start_init();
	
	/* PORT setting on PB4 */
	PB4_set_level(false);
	PB4_set_dir(PORT_DIR_IN);
	PB4_set_pull_mode(PORT_PULL_OFF);
	PB4_set_isc(PORT_ISC_INTDISABLE_gc);
	
	/* PORT setting on PA0 */
	PA3_set_level(false);
	PA3_set_dir(PORT_DIR_IN);
	PA3_set_pull_mode(PORT_PULL_OFF);
	PA3_set_isc(PORT_ISC_INTDISABLE_gc);
	
	_seed = number;
	
	for (uint8_t i = 0; i < 5; i++)
	{
		_seed = _seed ^ (((ADC_0_get_conversion(9) & 0x07) << 3) + (ADC_0_get_conversion(3) & 0x07));
		_seed = (_seed << 6) + ((ADC_0_get_conversion(3) & 0x07) << 3) + (ADC_0_get_conversion(9) & 0x07);
		_seed = _seed ^ TCA0.SINGLE.CNT;
	}

	
}

