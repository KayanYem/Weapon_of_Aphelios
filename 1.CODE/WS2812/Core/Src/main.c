/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "WS2812B.h"
#include "stmflash.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define SAVE_MARK_ADDR 0x0800F000		//fresh 更新标记
#define SAVE_ADDR 0x0800F200				//fresh 写入

#define LED_NUM		5
/* USER CODE END PTD */	

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t t;
uint16_t i,j,temp[3], flag=0;

uint16_t flash_init_temp[2]={0};
uint16_t SAVE_MARK[2] = {11,22};
uint16_t SAVE_BUFFER[24];

//自定义rgb颜色数组			
/****RGB颜色表****
//uint8_t white[3] = {255,255,255};
//uint8_t red[3] = {255,0,0};
//uint8_t green[3] = {0,255,0};
//uint8_t blue[3] = {0,0,255};
//uint8_t cyan[3] = {0,255,255};
//uint8_t purple[3] = {255,0,255};
//uint8_t yellow[3] = {255,255,0};
//uint8_t orange[3] = {255,97,0};
//uint8_t brown[3] = {128,42,42};
//uint8_t pink[3] = {255,192,203};
//uint8_t D_green[3] = {48,128,20};
//uint8_t D_blue[3] = {25,25,112};
*/

uint8_t change_rgb_init[LED_NUM][3] = {
	{50,225,50},
	{250,50,125},
	{131,71,205},
	{68,117,249},
	{214,235,255}
};

uint8_t change_rgb[LED_NUM][3];

uint8_t rgb0[][3] = {0,0,0};

//static uint16_t fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

//   printf("串口打印正常!\r\n");

	//将初始化数据赋值 PWM输出数组 和 flash存储数组
	for(i=0;i<LED_NUM;i++)
	{
		for(j=0;j<3;j++)
		{
			change_rgb[i][j] = change_rgb_init[i][j];
			SAVE_BUFFER[i*3+j] = change_rgb_init[i][j];
		}
	}
	
	//读取判断位
	STMFLASH_Read(SAVE_MARK_ADDR, flash_init_temp, 2);
//	printf("%d\t",flash_init_temp[0]);
//	printf("%d\r\n",flash_init_temp[1]);
	
	
	//如果写入，则读取flash数据
	if(flash_init_temp[0] == SAVE_MARK[0] && flash_init_temp[1] == SAVE_MARK[1])
	{
//		printf("read from flash\r\n");
		STMFLASH_Read(SAVE_ADDR, SAVE_BUFFER, LED_NUM*3);
//		for(i=0;i<LED_NUM*3;i++)
//		{
//			printf("%d ",SAVE_BUFFER[i]);
//		}
//		printf("\r\n");
		
		for(i=0;i<LED_NUM;i++)
		{
			for(j=0;j<3;j++)
			{
				change_rgb[i][j] = SAVE_BUFFER[i*3+j];
			}
		}
	}
	//如果未写入，则写出初始化数据
	else
	{
//		printf("init flash\r\n");
		STMFLASH_Write(SAVE_MARK_ADDR, SAVE_MARK, 2);				//FLASH更新标志位写入
//		STMFLASH_Read(SAVE_MARK_ADDR, SAVE_MARK, 2);
//		for(i=0;i<2;i++)
//		{
//			printf("%d ",SAVE_MARK[i]);
//		}
//		printf("\r\n");
		
		STMFLASH_Write(SAVE_ADDR, SAVE_BUFFER, LED_NUM*3);	//flash颜色数据写入
//		STMFLASH_Read(SAVE_ADDR, SAVE_BUFFER, LED_NUM*3);
//		for(i=0;i<LED_NUM*3;i++)
//		{
//			printf("%d ",SAVE_BUFFER[i]);
//		}
//		printf("\r\n");
	}
	
	//setoff all light
	WS2812_led_send(rgb0,8);
	HAL_Delay(5);
	
	WS2812_ledsingle_send(change_rgb,LED_NUM);
	HAL_Delay(5);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  { 
		if(flag == 1)
		{
			for(i=0;i+1<LED_NUM;i++)
			{
				for(j=0;j<3;j++)
				{
					temp[j] = change_rgb[i][j];
					change_rgb[i][j] = change_rgb[i+1][j];
					change_rgb[i+1][j] = temp[j];
				}
			}
			
			WS2812_ledsingle_send(change_rgb,LED_NUM);
			HAL_Delay(200);
			
			//write flash
			for(i=0;i<LED_NUM;i++)
			{
				for(j=0;j<3;j++)
				{
					SAVE_BUFFER[i*3+j] = change_rgb[i][j];
				}
			}
//			printf("key1 press \r\n");
			STMFLASH_Write(SAVE_ADDR, SAVE_BUFFER, LED_NUM*3);
//			STMFLASH_Read(SAVE_ADDR, SAVE_BUFFER, LED_NUM*3);
//			for(i=0;i<LED_NUM*3;i++)
//			{
//				printf("%d ",SAVE_BUFFER[i]);
//			}
//			printf("\r\n");
			
			//重置按键标志位
			flag = 0;
		}
		
		if(flag == 2)
		{
			for(i=1;i+1<LED_NUM;i++)
			{
				for(j=0;j<3;j++)
				{
					temp[j] = change_rgb[i][j];
					change_rgb[i][j] = change_rgb[i+1][j];
					change_rgb[i+1][j] = temp[j];
				}
			}
			WS2812_ledsingle_send(change_rgb,LED_NUM);
			HAL_Delay(200);
			
			//write flash
			for(i=0;i<LED_NUM;i++)
			{
				for(j=0;j<3;j++)
				{
					SAVE_BUFFER[i*3+j] = change_rgb[i][j];
				}
			}
//			printf("key2 press \r\n");
			STMFLASH_Write(SAVE_ADDR, SAVE_BUFFER, LED_NUM*3);
//			STMFLASH_Read(SAVE_ADDR, SAVE_BUFFER, LED_NUM*3);
//			for(i=0;i<LED_NUM*3;i++)
//			{
//				printf("%d ",SAVE_BUFFER[i]);
//			}
//			printf("\r\n");
			
			//重置按键标志位
			flag = 0;
		}
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

//exterinal break callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_Delay(10);
	//key1 pressed
	if(GPIO_Pin == GPIO_PIN_1){
		flag = 1;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
	
	//key2 pressed
	if(GPIO_Pin == GPIO_PIN_2){
		flag = 2;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	}
}

//PWM callback
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
		HAL_TIM_PWM_Stop_DMA(&htim2,TIM_CHANNEL_1);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
