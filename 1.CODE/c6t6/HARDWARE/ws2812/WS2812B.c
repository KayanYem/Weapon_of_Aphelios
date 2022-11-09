#include "WS2812B.h"
#include "tim.h"
/* Buffer that holds one complete DMA transmission
 * 
 * Ensure that this buffer is big enough to hold
 * all data bytes that need to be sent
 * 
 * The buffer size can be calculated as follows:
 * number of LEDs * 24 bytes + 42 bytes
 * 
 * This leaves us with a maximum string length of
 * (2^16 bytes per DMA stream - 42 bytes)/24 bytes per LED = 2728 LEDs
 */

//n0 = 27, n1 = 61, n = 90
#define TIMING_ONE  61
#define TIMING_ZERO 27
#define MAXNUM			20
uint16_t LED_BYTE_Buffer[24*MAXNUM+43] = {0};


//---------------------------------------------------------------//

/* This function sends data bytes out to a string of WS2812s
 * The first argument is a pointer to the first RGB triplet to be sent
 * The seconds argument is the number of LEDs in the chain
 * 
 * This will result in the RGB triplet passed by argument 1 being sent to 
 * the LED that is the furthest away from the controller (the point where
 * data is injected into the chain)
 */
void WS2812_led_send(uint8_t (*color)[3], uint16_t len)
{
	uint8_t i;
	uint16_t memaddr;
	uint16_t buffersize;
	buffersize = (len*24)+80;	// number of bytes needed is #LEDs * 24 bytes + 42 trailing bytes
	memaddr = 0;				// reset buffer memory index

		while(memaddr < 20)
		{
			LED_BYTE_Buffer[memaddr] = 0;
			memaddr++;
		}
		
	while (len)
	{	
		for(i=0; i<8; i++) // GREEN data
		{
			LED_BYTE_Buffer[memaddr] = ((color[0][1]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
		for(i=0; i<8; i++) // RED
		{
			LED_BYTE_Buffer[memaddr] = ((color[0][0]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
		for(i=0; i<8; i++) // BLUE
		{
			LED_BYTE_Buffer[memaddr] = ((color[0][2]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
		len--;
	}
		while(memaddr < buffersize)
		{
			LED_BYTE_Buffer[memaddr] = 0;
			memaddr++;
		}
		HAL_TIM_PWM_Start_DMA( &htim2 , TIM_CHANNEL_1, (uint32_t *)LED_BYTE_Buffer, buffersize);
}
	

/*
实现单独控制rgb颜色和亮度，color-》存储rgb颜色数据的数组，len-》要显示的rgb个数
*/

void WS2812_ledsingle_send(uint8_t color[][3], uint16_t len)
{
	uint8_t i;
	uint16_t memaddr;
	uint16_t buffersize;
	buffersize = (len*24)+80;	// number of bytes needed is #LEDs * 24 bytes + 42 trailing bytes
	memaddr = 0;				// reset buffer memory index

		while(memaddr < 20)
		{
			LED_BYTE_Buffer[memaddr] = 0;
			memaddr++;
		}
		
	while (len)
	{	
		len--;
		for(i=0; i<8; i++) // GREEN data
		{
			LED_BYTE_Buffer[memaddr] = ((color[len][1]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
		for(i=0; i<8; i++) // RED
		{
			LED_BYTE_Buffer[memaddr] = ((color[len][0]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
		for(i=0; i<8; i++) // BLUE
		{
			LED_BYTE_Buffer[memaddr] = ((color[len][2]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
	}
		while(memaddr < buffersize)
		{
			LED_BYTE_Buffer[memaddr] = 0;
			memaddr++;
		}
		HAL_TIM_PWM_Start_DMA( &htim2 , TIM_CHANNEL_1, (uint32_t *)LED_BYTE_Buffer, buffersize);
}



