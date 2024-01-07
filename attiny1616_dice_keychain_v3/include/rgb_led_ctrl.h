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

#define MAX_DICE_NUMBER 9

char get_dice_color(void);
void set_dice_color(char color);
void next_color(void);
uint8_t get_dice_number(void);
void set_dice_number(uint8_t number);
void dice_timer_routine(void);
void leds_off(void);
void clear_all_led_pins(void);
void set_single_rgb_led(uint8_t led_index, char rgb);

#endif /* RGB_LED_CTRL_H_ */