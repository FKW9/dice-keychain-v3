#include <atmel_start.h>
#include <util/delay.h>
#include <rgb_led_ctrl.h>
#include <avr/sleep.h>
#include <adc_basic.h>

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

	/* PORT setting on PA3 */
	FLASH_EN_set_level(false);
	FLASH_EN_set_dir(PORT_DIR_IN);
	FLASH_EN_set_pull_mode(PORT_PULL_UP);
	FLASH_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PA4 */
	LED1_EN_set_level(false);
	LED1_EN_set_dir(PORT_DIR_IN);
	LED1_EN_set_pull_mode(PORT_PULL_OFF);
	LED1_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PA5 */
	LED2_EN_set_level(false);
	LED2_EN_set_dir(PORT_DIR_IN);
	LED2_EN_set_pull_mode(PORT_PULL_OFF);
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

	/* PORT setting on PB0 */
	R_EN_set_level(false);
	R_EN_set_dir(PORT_DIR_IN);
	R_EN_set_pull_mode(PORT_PULL_OFF);
	R_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PB1 */
	G_EN_set_level(false);
	G_EN_set_dir(PORT_DIR_IN);
	G_EN_set_pull_mode(PORT_PULL_OFF);
	G_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PB2 */
	B_EN_set_level(false);
	B_EN_set_dir(PORT_DIR_IN);
	B_EN_set_pull_mode(PORT_PULL_OFF);
	B_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

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
	LED7_EN_set_pull_mode(PORT_PULL_OFF);
	LED7_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);

	/* PORT setting on PC3 */
	LED8_EN_set_level(false);
	LED8_EN_set_dir(PORT_DIR_IN);
	LED8_EN_set_pull_mode(PORT_PULL_OFF);
	LED8_EN_set_isc(PORT_ISC_INPUT_DISABLE_gc);
	
	sleep_cpu();
	
	atmel_start_init();
}

volatile float		  voltage = 0;
volatile adc_result_t ADC_0_measurement;
volatile bool         ADC_0_isr_executed = false;

void ADC_0_adc_handler_cb(void)
{
	ADC_0_measurement = ADC_0_get_conversion_result();
	
	// 31.3.2.5 ADC Conversion Result
	// After the conversion is complete (RESRDY is '1'), the conversion result RES is available in the ADC Result Register (ADCn.RES). The result for a 10-bit conversion is given as:
	// RES = (1023 x VIN) / VREF
	// VREF = 2.5V
	// Multiply by 2 because of 50/50 voltage divider
	voltage = (ADC_0_measurement * 2.5 / 1023) * 2;
	
	uint16_t voltage1000 = voltage * 1e3;
	while(voltage1000)
	{
		uint8_t digit = voltage1000 % 10;
		if (digit==0)
		{
			set_single_rgb_led(1, 'r');
		} 
		else
		{
			set_single_rgb_led(digit, 'b');
		}
		voltage1000 /= 10;
		_delay_ms(1000);
	}
	clear_all();
	ADC_0_isr_executed = true;
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	ADC_0_register_callback(ADC_0_adc_handler_cb);
			
	// 	PORTB.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
	// 	PORTB.OUTCLR = PIN3_bm;
	// 	PORTB.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm;
	// 	
	// 	PORTC.DIRSET = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
	// 	PORTC.OUTCLR = PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm;
	// 	
	// 	PORTA.DIRSET = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
	// 	PORTA.OUTCLR = PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;

	/* Replace with your application code */
	while (1) {
		

		// make sure flag is false
		ADC_0_isr_executed = false;

		// Start conversion from ADC CH0
		ADC_0_start_conversion(1);

		// Wait for ISR to be executed
		while (!ADC_0_isr_executed);
		
		goto_sleep();
		
		
		
		
		
/*		ADC_0_start_conversion(1);*/
// 		
// 		// Get conversion from specified ADC channel
// 		ADC_0_measurement = ADC_0_get_conversion(0);
// 
// 		// Get 8 MSB of conversion result
// 		ADC_0_measurement_normalized = ADC_0_measurement >> (ADC_0_get_resolution() - 8);
// 		
// 		float voltage = ADC_0_measurement * 2.5 / 1023;
// 		uint16_t voltage1000 = voltage * 1e3;
// 
// 		while(voltage1000)
// 		{
// 			uint8_t digit = voltage1000 % 10;
// 			if (digit==0)
// 			{
// 				set_single_rgb_led(1, 'r');
// 			} else {
// 				set_single_rgb_led(digit, 'b');
// 			}
// 			voltage1000 /= 10;
// 			_delay_ms(1000);
// 		}
// 		
// 		clear_all();
// 		_delay_ms(5000);
			
			
			
			
			
			
			
			
		// FLASH_EN_toggle_level();
		// _delay_ms(500);
// 		for (uint8_t loop = 0; loop<2; loop++)
// 		{
// 			for (uint8_t led = 1; led<10; led++)
// 			{
// 				for (uint8_t rgbi = 0; rgbi<3; rgbi++)
// 				{
// 					char rgb;
// 					switch (rgbi){
// 						case 0:
// 						rgb = 'r';
// 						break;
// 						case 1:
// 						rgb = 'g';
// 						break;
// 						case 2:
// 						rgb = 'b';
// 						break;
// 					}
// 					set_single_rgb_led(led, rgb);
// 					_delay_ms(80);
// 				}
// 			}
// 		}
		// goto_sleep();
	}
}
