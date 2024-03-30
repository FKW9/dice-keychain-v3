#include <atmel_start.h>
#include <rgb_led_ctrl.h>
#include <avr/sleep.h>
#include <adc_basic.h>
#include <driver_init.h>
#include <compiler.h>
#include <stdlib.h>

#define DELAY_SLOWDOWN		20	// 20
#define DELAY_SHUFFLE		8	// 8
#define BLINK_COUNT			3	// 3
#define TIMEOUT_SHUFFLE		400	// 500 = ~5sec
#define EXECS_TILL_NEW_SEED 10	// 5
#define BTN_HOLD_TIME		30

float read_cell_voltage(void);
void prepare_sleep(void);
void goto_sleep(void);
void reset_state(void);
void enable_rng_adc_channels(void);
void disable_rng_adc_channels(void);
void generate_new_seed(void);
void start_init_after_sleep(void);
void adc_disable();
void adc_enable();

volatile float		cell_voltage		= 0.0;
volatile uint16_t	_seed				= 0;
volatile uint16_t	_executions			= 0;
volatile uint16_t   _t_hold_key			= 0;
volatile uint8_t	blink_flag			= 0;
volatile uint8_t	blink_count			= 0;
volatile uint8_t	number				= 1;
volatile uint8_t	delay_slowdown		= DELAY_SLOWDOWN;
volatile uint8_t	current_dice_state	= 0;
volatile uint8_t	previous_dice_state	= 0;


// 10ms interrupt
ISR(TCA0_CMP0_vect)
{
	static uint16_t t_shuffle_number	= 0;
	static uint16_t t_slowdown_number	= 0;
	static uint16_t t_timeout			= 0;
	static uint8_t  shuffle_number		= 1;
	
	///////////////////////////////////////////////////////////
	if (current_dice_state == 1)
	{
		// wait if button is held long enough
		if(BTN_get_level() == 1)
		{
			_t_hold_key++;
			if(_t_hold_key >= BTN_HOLD_TIME)
			{
				_t_hold_key = 0;
				current_dice_state = 2;
				start_init_after_sleep();
			}
		}
		else {
			_t_hold_key = 0;
		}
	}
	
	///////////////////////////////////////////////////////////
	else if (current_dice_state == 2)
	{
		t_slowdown_number = 0;

		if (++t_shuffle_number >= DELAY_SHUFFLE){
			t_shuffle_number = 0;
			next_color();
			set_dice_number(shuffle_number);
			if (++shuffle_number > MAX_DICE_NUMBER){
				shuffle_number = 1;
			}
		}

		if (++t_timeout > TIMEOUT_SHUFFLE){
			current_dice_state = 3;
		}
	}

	///////////////////////////////////////////////////////////
	else if (current_dice_state == 3)
	{
		t_timeout = 0;
		t_shuffle_number = 0;

		// set number once entering slowdown
		if (t_slowdown_number == 0)
		{
			number = rand() % MAX_DICE_NUMBER + 1;
		}

		// when button is released, start slowing down then blink the resulting number
		if (++t_slowdown_number >= delay_slowdown)
		{
			t_slowdown_number = 1;

			if (delay_slowdown <= (50-DELAY_SLOWDOWN))
			{
				if (++number > MAX_DICE_NUMBER){
					number = 1;
				}
				next_color();
				set_dice_number(number);
				delay_slowdown += 4;
			}
			else
			{
				if (++blink_count < (BLINK_COUNT * 2))
				{
					blink_flag ^= 1;
				}
				else
				{
					if (BTN_get_level() == 0) {
						goto_sleep();
					}
				}
			}
		}
	}

	/* The interrupt flag has to be cleared manually */
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

// 10us interrupt
ISR(TCB0_INT_vect)
{
	if (current_dice_state == 2 || current_dice_state == 3)
	{
		if (blink_flag == 0)
		{
			dice_timer_routine();
		}
		else
		{
			leds_off();
		}
	}

	TCB0.INTFLAGS = TCB_CAPT_bm;
}

ISR(PORTA_PORT_vect)
{

	// be sure to wake up on rising edge, else go to sleep again
	if (current_dice_state == 0 && previous_dice_state == 0 && BTN_get_level() == 1)
	{
		// after startup (or coming back from sleep)
		previous_dice_state = current_dice_state;
		current_dice_state = 1; // wait if button is held long enough
		TCA0.SINGLE.CTRLA |= (1 << TCA_SINGLE_ENABLE_bp);
	}
	else if (current_dice_state == 1 && previous_dice_state == 0)
	{
		// when releasing button before min hold time is reached
		goto_sleep();
	}
	else if (current_dice_state == 2 && previous_dice_state == 0)
	{
		// when releasing button
		previous_dice_state = current_dice_state;
		current_dice_state = 3;
	}
	else if (current_dice_state == 3 && previous_dice_state == 2)
	{
		// pressing button before entering sleep mode (= reroll)
		reset_state();
		current_dice_state = 2;
	}
	// prevent from going to sleep while number is blinking (happens after timeout when button is released)
	else if (current_dice_state != 3)
	{
		goto_sleep();
	}

	/* Clear interrupt flags */
	VPORTA_INTFLAGS = (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
}

int main(void)
{
	atmel_start_init();
	goto_sleep();
	while (1) {}
}

void reset_state(void){
	_t_hold_key = 0;
	current_dice_state = 0;
	previous_dice_state = 0;
	blink_flag = 0;
	blink_count = 0;
	delay_slowdown = DELAY_SLOWDOWN;
}

void goto_sleep(void){

	// unload RC network from button
	BTN_set_dir(PORT_DIR_OUT);
	BTN_set_level(false);

	TCB0.CTRLA &= ~(1 << TCB_ENABLE_bp);
	TCA0.SINGLE.CTRLA &= ~(1 << TCA_SINGLE_ENABLE_bp);
	leds_off();
	reset_state();

	if (++_executions >= EXECS_TILL_NEW_SEED)
	{
		BTN_set_isc(PORT_ISC_INPUT_DISABLE_gc);
		generate_new_seed();
		BTN_set_isc(PORT_ISC_BOTHEDGES_gc);
		_executions = 0;
	}

	adc_disable();

	PORTA.OUT = 0;
	PORTA.DIR = 0;
	PORTB.OUT = 0;
	PORTB.DIR = 0;
	PORTC.OUT = 0;
	PORTC.DIR = 0;
	for (uint8_t i = 0; i < 8; i++) {
		*((uint8_t *)&PORTA + 0x10 + i) = 0b00001100;
		*((uint8_t *)&PORTB + 0x10 + i) = 0b00001100;
		*((uint8_t *)&PORTC + 0x10 + i) = 0b00001100;
	}
	BTN_set_dir(PORT_DIR_IN);
	BTN_set_pull_mode(PORT_PULL_OFF);
	BTN_set_isc(PORT_ISC_BOTHEDGES_gc);

	sleep_cpu();
}

void generate_new_seed(void){
	adc_enable();
	enable_rng_adc_channels();
	_seed = number;
	for (uint8_t i = 0; i<5; i++)
	{
		_seed ^= (((ADC_0_get_conversion(9) & 0x07) << 3) + (ADC_0_get_conversion(3) & 0x07));
		_seed = (_seed << 6) + ((ADC_0_get_conversion(3) & 0x07) << 3) + ((ADC_0_get_conversion(9) & 0x07));
		_seed ^= TCA0.SINGLE.CNT;
	}
	disable_rng_adc_channels();
	adc_disable();
	srand(_seed);
}

float read_cell_voltage(void){
	adc_enable();

	ADC_EN_set_level(false);
	ADC_EN_set_dir(PORT_DIR_OUT);

	// After the conversion is complete (RESRDY is '1'), the conversion result RES is available in the ADC Result Register (ADCn.RES). The result for a 10-bit conversion is given as:
	// RES = (1023 x VIN) / VREF, VREF = 2.5V
	// Multiply by 2 because of 50/50 voltage divider
	float voltage = (ADC_0_get_conversion(1)* 2.5 / 1023) * 2;

	ADC_EN_set_level(false);
	ADC_EN_set_dir(PORT_DIR_IN);
	ADC_EN_set_pull_mode(PORT_PULL_UP);
	ADC_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	adc_disable();

	return voltage;
}

void start_init_after_sleep(void){

	// cell_voltage = read_cell_voltage();

	/* PORT setting on PB5 */
	LED1_EN_set_level(false);
	LED1_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PA5 */
	LED2_EN_set_level(false);
	LED2_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PA6 */
	LED3_EN_set_level(false);
	LED3_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PA7 */
	LED4_EN_set_level(false);
	LED4_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PC0 */
	LED5_EN_set_level(false);
	LED5_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PC1 */
	LED6_EN_set_level(false);
	LED6_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PC2 */
	LED7_EN_set_level(false);
	LED7_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PC3 */
	LED8_EN_set_level(false);
	LED8_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PB3 */
	LED9_EN_set_level(false);
	LED9_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PB0 */
	R_EN_set_level(true);
	R_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PB1 */
	G_EN_set_level(true);
	G_EN_set_dir(PORT_DIR_OUT);

	/* PORT setting on PB2 */
	B_EN_set_level(true);
	B_EN_set_dir(PORT_DIR_OUT);

	TCB0.CTRLA |= (1 << TCB_ENABLE_bp);
}

void adc_enable()
{
	VREF_0_init();
	ADC_0_enable();
	ADC_set_isc(PORT_ISC_INPUT_DISABLE_gc);
	ADC_set_pull_mode(PORT_PULL_OFF);
}

void adc_disable()
{
	VREF_CTRLB = 0;
	ADC_0_disable();
	ADC_set_level(false);
	ADC_set_dir(PORT_DIR_IN);
	ADC_set_pull_mode(PORT_PULL_UP);
	ADC_set_isc(PORT_ISC_INPUT_DISABLE_gc);
}

void enable_rng_adc_channels(void){
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
}

void disable_rng_adc_channels(void){
	/* PORT setting on PB4 */
	PB4_set_level(false);
	PB4_set_dir(PORT_DIR_IN);
	PB4_set_pull_mode(PORT_PULL_UP);
	PB4_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PA0 */
	PA3_set_level(false);
	PA3_set_dir(PORT_DIR_IN);
	PA3_set_pull_mode(PORT_PULL_UP);
	PA3_set_isc(PORT_ISC_INPUT_DISABLE_gc);
}