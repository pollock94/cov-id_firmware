/*
 * led.h
 *
 *  Created on: Jan 13, 2020
 *      Author: Pollock
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "driver/gpio.h"
#define LED_PIN (GPIO_NUM_27)

void led_off();
void led_on();
void led_init();

#endif /* INC_LED_H_ */
