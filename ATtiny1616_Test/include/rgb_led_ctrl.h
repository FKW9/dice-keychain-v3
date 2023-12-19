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

void clear_all(void);
void set_single_rgb_led(uint8_t led_index, char rgb);

#endif /* RGB_LED_CTRL_H_ */