/*
 * led_ctrl.h
 *
 * Created: 16.10.2023 22:20:07
 *  Author: flori
 */ 

#ifndef LED_CTRL_H_
#define LED_CTRL_H_

#include <inttypes.h>
#include <atmel_start_pins.h>

#define LED_A PIN4_bm
#define LED_B PIN5_bm
#define LED_C PIN0_bm
#define LED_D PIN1_bm

#define MAX_DIE_NUMBER 7

void set_single_led(uint8_t led_index);
void set_die_number(uint8_t number);
void die_timer_routine(void);

#endif /* LED_CTRL_H_ */