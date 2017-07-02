/**
  ******************************************************************************
  * @file    Project/STM32F4_EVB_Demo/main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-March-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "stdio.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "stm32f4x7_eth.h"
#include "netconf.h"
#include "lwip/tcp.h"

/** @addtogroup STM32F4xx_StdPeriph_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS 	10
#define AUDIO_FILE_SZE			990000
#define AUIDO_START_ADDRESS		58 /* Offset relative to audio file header size */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint32_t cid, revid;
USB_OTG_CORE_HANDLE	USB_OTG_dev;
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
__IO uint32_t uwVolume = 70;
static __IO uint32_t uwTimingDelay;
uint32_t testdata[sEE_PAGESIZE / 4], readdata[sEE_PAGESIZE / 4];
uint16_t uhNumDataRead;

uint8_t Read_Gensor_RegValue[1];

char str[50];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	USART_InitTypeDef USART_InitStructure;
	int i;

	/* SysTick end of count event each 10ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	RNG_Cmd(ENABLE);

	/* Initialize LEDs and Button and UART1 on EVAL board *********************/
	STM_EVAL_LEDInit(LED1);
	STM_EVAL_LEDInit(LED2);
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO);
	STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO);
	STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);
	USART_InitStructure.USART_BaudRate				= 115200;
	USART_InitStructure.USART_WordLength			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits				= USART_StopBits_1;
	USART_InitStructure.USART_Parity				= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx;
	STM_EVAL_COMInit(COM1, &USART_InitStructure);
	/* Initialize the SRAM ****************************************************/
	PSRAM_Init();
	/* Initialize the LCD *****************************************************/
	LCD_Init();
	/* Initialize the USB *****************************************************/
	USBD_Init(&USB_OTG_dev, USB_OTG_HS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);
	/* Initialize EEPROM and I2C **********************************************/
	sEE_Init();
#if 0        
	// EEPROM test
	for (i = 0; i < sEE_PAGESIZE / 4; i++)
	{
		testdata[i] = RNG_GetRandomNumber();
		Delay(1);
	}
	sEE_WriteBuffer((uint8_t *)testdata, 0x00, sizeof(testdata));
	sEE_WaitEepromStandbyState();
	uhNumDataRead = sizeof(readdata);
	sEE_ReadBuffer((uint8_t *)readdata, 0x00, (uint16_t *)&uhNumDataRead);
	i = memcmp(readdata, testdata, sizeof(testdata));
	if (i == 0)
	{
		LCD_DisplayStringLine(LCD_LINE_16, (uint8_t *)" EEPROM TEST OK          ");
	}
	else
	{
		LCD_DisplayStringLine(LCD_LINE_16, (uint8_t *)" EEPROM TEST FAILED      ");
	}
#endif        
	/* Initialize GSensor******************************************************/
	GSensor_Init();
               
//        i2c_single_write(0x30, 0x20, 0x07);
#if 0       
         while(1)  
         {  
					 
                i2c_single_read(GSENSOR_I2C_ADDR, OUTX_L, Read_Gensor_RegValue[0]);
                sprintf(str, " GSENSOR OUTX_L :%d",Read_Gensor_RegValue[0] );             
		LCD_DisplayStringLine(LCD_LINE_15, (uint8_t *)str);
                
                i2c_single_read(GSENSOR_I2C_ADDR, OUTX_H, Read_Gensor_RegValue[0]);
                sprintf(str, " GSENSOR OUTX_H :%d",Read_Gensor_RegValue[0] );             
		LCD_DisplayStringLine(LCD_LINE_14, (uint8_t *)str);
                
                i2c_single_read(GSENSOR_I2C_ADDR, OUTY_L, Read_Gensor_RegValue[0]);
                sprintf(str, " GSENSOR OUTY_L :%d",Read_Gensor_RegValue[0] );             
		LCD_DisplayStringLine(LCD_LINE_13, (uint8_t *)str); 
                
                i2c_single_read(GSENSOR_I2C_ADDR, OUTY_H, Read_Gensor_RegValue[0]);
                sprintf(str, " GSENSOR OUTY_H :%d",Read_Gensor_RegValue[0] );             
		LCD_DisplayStringLine(LCD_LINE_12, (uint8_t *)str);  
                
                i2c_single_read(GSENSOR_I2C_ADDR, OUTZ_L, Read_Gensor_RegValue[0]);
                sprintf(str, " GSENSOR OUTZ_L :%d",Read_Gensor_RegValue[0] );             
		LCD_DisplayStringLine(LCD_LINE_11, (uint8_t *)str);  
                
                i2c_single_read(GSENSOR_I2C_ADDR, OUTZ_H, Read_Gensor_RegValue[0]);
                sprintf(str, " GSENSOR OUTZ_H :%d",Read_Gensor_RegValue[0] );             
		LCD_DisplayStringLine(LCD_LINE_10, (uint8_t *)str);  
                              
                STM_EVAL_LEDToggle(LED1);
								
		/* Insert 100 ms delay */
		//Delay(10);
     //     i2c_single_read(GSENSOR_I2C_ADDR, OUTX_H, Read_Gensor_RegValue);
      //  i2c_single_read(GSENSOR_I2C_ADDR, OUTX_L, Read_Gensor_RegValue);
         }
       
//sprintf(str, " TOUCH X:%d, Y:%d, Z:%d", tsc_info->X, tsc_info->Y, tsc_info->Z);
#endif         
	/* Initialize Touch *******************************************************/
	TSC_Config();
	/* Initialize SD **********************************************************/
	SD_Init();
	/* Initialize the Audio codec *********************************************/  
	if (EVAL_AUDIO_Init(OUTPUT_DEVICE_BOTH, uwVolume, I2S_AudioFreq_48k) == 0)
	{
		LCD_DisplayStringLine(LCD_LINE_13, (uint8_t *)" AUDIO CODEC OK");
		if (cid == 0x1C)
		{
			switch (revid)
			{
				case 0:
					LCD_DisplayStringLine(LCD_LINE_14, (uint8_t *)" CS43L22 Rev A0");
					break;
				case 1:
					LCD_DisplayStringLine(LCD_LINE_14, (uint8_t *)" CS43L22 Rev A1");
					break;
				case 2:
					LCD_DisplayStringLine(LCD_LINE_14, (uint8_t *)" CS43L22 Rev B0");
					break;
				case 3:
					LCD_DisplayStringLine(LCD_LINE_14, (uint8_t *)" CS43L22 Rev B1");
					break;
				default:
					break;
			}
		}
	}
	else
	{
		LCD_DisplayStringLine(LCD_LINE_13, (uint8_t *)" AUDIO CODEC FAIL");
	}
	/* Turn on LEDs *************************************************************/
	STM_EVAL_LEDOn(LED1);
	STM_EVAL_LEDOn(LED2);
	STM_EVAL_LEDOn(LED3);
	STM_EVAL_LEDOn(LED4);
	/* Add your application code here */
	/* Configure ethernet (GPIOs, clocks, MAC, DMA) */
	ETH_BSP_Config();
	/* Initilaize the LwIP stack */
	LwIP_Init();
	/* Main Loop */
	EVAL_AUDIO_Play((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS), (AUDIO_FILE_SZE - AUIDO_START_ADDRESS));
	while (1)
	{
		TSC_STATE *tsc_info;
		char str[50];

		/* check if any packet received */
		if (ETH_CheckFrameReceived())
		{
			/* process received ethernet packet */
			LwIP_Pkt_Handle();
		}
		/* handle periodic timers for LwIP */
		LwIP_Periodic_Handle(LocalTime);
		/* Toggle LD1 */
		STM_EVAL_LEDToggle(LED1);
		/* Insert 100 ms delay */
		Delay(10);
		if (STM_EVAL_PBGetState(BUTTON_WAKEUP) == Bit_SET)
		{
			STM_EVAL_LEDToggle(LED3);
		}
		if (STM_EVAL_PBGetState(BUTTON_TAMPER) == Bit_RESET)
		{
			STM_EVAL_LEDToggle(LED3);
		}
		if (STM_EVAL_PBGetState(BUTTON_KEY) == Bit_RESET)
		{
			STM_EVAL_LEDToggle(LED3);
		}
		tsc_info = TSC_TS_GetState();
		sprintf(str, " TOUCH X:%d, Y:%d, Z:%d", tsc_info->X, tsc_info->Y, tsc_info->Z);
                
 //               i2c_single_read(GSENSOR_I2C_ADDR, OUTX_L, Read_Gensor_RegValue);
                
		LCD_DisplayStringLine(LCD_LINE_18, (uint8_t *)str);
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
	uwTimingDelay = nTime;

	while (uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
	if (uwTimingDelay != 0x00)
	{
		uwTimingDelay--;
	}
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
	LocalTime += SYSTEMTICK_PERIOD_MS;
}

/*--------------------------------
       Callbacks implementation:
           the callbacks prototypes are defined in the stm32f4_evb_audio_codec.h file
           and their implementation should be done in the user coed if they are needed.
           Below some examples of callback implementations.
                                     --------------------------------------------------------*/
/**
  * @brief  Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
#ifdef AUDIO_MAL_MODE_NORMAL  
	/* Replay from the beginning */
	EVAL_AUDIO_Play((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS), (AUDIO_FILE_SZE - AUIDO_START_ADDRESS));
#else /* #ifdef AUDIO_MAL_MODE_CIRCULAR */
	/* Display message on the LCD screen */
	LCD_DisplayStringLine(Line8, " All Buffer Reached ");   
#endif /* AUDIO_MAL_MODE_CIRCULAR */
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size)
{  
#ifdef AUDIO_MAL_MODE_CIRCULAR
	/* Display message on the LCD screen */
	LCD_DisplayStringLine(Line8, " 1/2 Buffer Reached "); 
#endif /* AUDIO_MAL_MODE_CIRCULAR */
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void EVAL_AUDIO_Error_CallBack(void* pData)
{
	/* Display message on the LCD screen */
	LCD_SetBackColor(Red);
	LCD_DisplayStringLine(Line8, (uint8_t *)"     DMA  ERROR     ");
	/* Stop the program with an infinite loop */
	while (1) {};
}

/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t Codec_TIMEOUT_UserCallback(void)
{
	/* Display message on the LCD screen */
	LCD_DisplayStringLine(Line8, (uint8_t *)"  CODEC I2C  ERROR  ");  

	/* Block communication and all processes */
	while (1) {};
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
