/*
 * rgb_led_ctrl.h
 *
 * Created: 17.11.2023 16:52:05
 *  Author: Florian
 */ 
#include <inttypes.h>
#include <atmel_start_pins.h>

#ifndef RGB_LED_CTRL_H_
#define RGB_LED_CTRL_H_

#define MAX_DIE_NUMBER 9

char get_die_color(void);
void set_die_color(char color);
uint8_t get_die_number(void);
void set_die_number(uint8_t number);
void die_timer_routine(void);
void clear_all(void);
void set_single_rgb_led(uint8_t led_index, char rgb);

#endif /* RGB_LED_CTRL_H_ */