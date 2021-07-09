/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_hid.h"
#include "oled.h"
#include "bmp.h"
#include "as608.h"
#ifdef __GNUC__
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
		HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
		return ch;
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t KeyBoard[8] = {0,0,4,0,0,0,0,0};
uint8_t KeyBoard01[8] = {0,0,0,0,0,0,0,0};
extern USBD_HandleTypeDef hUsbDeviceFS;
uint8_t flag;
uint8_t aRxBuffer[1];
uint8_t KeyBuff[4]={0x27,0x1e,0x1f,0x20};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

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
  MX_USB_DEVICE_Init();
  MX_SPI2_Init();
	MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1,aRxBuffer,1);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart3,rx_buffer_3,BUFFER_SIZE);
	ReadCustomData();
	OLED_Init();
	HAL_Delay(1000);
	OLED_Clear();
//	uint8_t t=' ';
	while(PS_HandShake(&AS608Addr))
	{
	  OLED_ShowString(0,2,(unsigned char*)" ShakHands....");
	}
	//握手通过  
	OLED_ShowString(0,2,(unsigned char*)" Ready To Work ");
	
	HAL_Delay(100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		press_FR();
		
		if(flag==1)
		{
			flag=0;
			KeyBoard[0] = 0;
			KeyBoard[2] = 0x2c;//space
			USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&KeyBoard,sizeof(KeyBoard01));
			HAL_Delay(15);
			USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&KeyBoard01,sizeof(KeyBoard));
			HAL_Delay(1000);
			uint8_t i=0;
			for(;i<4;i++)
			{
				KeyBoard[2] = KeyBuff[i];
				USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&KeyBoard,sizeof(KeyBoard));
				HAL_Delay(15);
				USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&KeyBoard01,sizeof(KeyBoard01));
				HAL_Delay(15);
			}
			OLED_Clear();
			OLED_ShowCHinese(48,3,7);
			OLED_ShowCHinese(48+16,3,8);
		}
		else if(flag==2)
		{
			flag=0;
			KeyBoard[0] = 8;//win
			KeyBoard[2] = 0xf;//l
			USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&KeyBoard,sizeof(KeyBoard01));
			HAL_Delay(15);
			USBD_HID_SendReport(&hUsbDeviceFS,(uint8_t*)&KeyBoard01,sizeof(KeyBoard));
			OLED_Clear();
			OLED_ShowCHinese(48,3,8);
			OLED_ShowCHinese(48+16,3,9);
		}
		
//		OLED_Clear();
//	//	LED_ON;
//		OLED_ShowCHinese(0,0,0);//中
//		OLED_ShowCHinese(18,0,1);//景
//		OLED_ShowCHinese(36,0,2);//园
//		OLED_ShowCHinese(54,0,3);//电
//		OLED_ShowCHinese(72,0,4);//子
//		OLED_ShowCHinese(90,0,5);//科
//		OLED_ShowCHinese(108,0,6);//技
//		OLED_ShowString(0,3,"1.3' OLED TEST");
//		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
//	 //	OLED_ShowString(20,4,"2014/05/01");  
//		OLED_ShowString(0,6,"ASCII:");  
//		OLED_ShowString(63,6,"CODE:");  
//		OLED_ShowChar(48,6,t);//显示ASCII字符	   
//		t++;
//		if(t>'~')t=' ';
//		OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	
//			
//	
//		HAL_Delay(8000);
//		OLED_Clear();
//		HAL_Delay(8000);
//	//		LED_OFF;
//		OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
//		HAL_Delay(8000);
//		OLED_DrawBMP(0,0,128,8,BMP2);
//		HAL_Delay(8000);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==SW1_Pin)
  {
		if(HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin) == GPIO_PIN_SET)
		{
			
		}
		else if(HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin) == GPIO_PIN_RESET)
		{
			flag=1;
		}
	}
	else if(GPIO_Pin==SW2_Pin)
  {
		if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == GPIO_PIN_SET)
		{
			
		}
		else if(HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin) == GPIO_PIN_RESET)
		{
			flag=2;
		}
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle == &huart1)
	{
		HAL_UART_Receive_IT(&huart1,aRxBuffer,1);
		CmdHandle();
	}
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
