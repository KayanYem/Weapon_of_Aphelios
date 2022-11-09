#ifndef __WS2812B_H
#define	__WS2812B_H

#include "stm32f1xx_hal.h"
#include "tim.h"

//#define WS2812_IN_PIN	PA0

void ws2812b_show_dot(uint8_t dot_index,uint32_t color);
void WS2812_led_send(uint8_t (*color)[3], uint16_t len);
void WS2812_ledsingle_send(uint8_t color[][3], uint16_t len);
#endif /* __LED_H */
