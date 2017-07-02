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

#define SYSTEMTICK_PERIOD_MS 	10
#define AUDIO_FILE_SZE			990000
#define AUIDO_START_ADDRESS		58 /* Offset relative to audio file header size */

#define MESSAGE1   "     STM32F4x7      "

extern uint32_t cid, revid;
USB_OTG_CORE_HANDLE	USB_OTG_dev;
__IO uint32_t LocalTime = 0;
__IO uint32_t uwVolume = 70;
static __IO uint32_t uwTimingDelay;
uint32_t testdata[sEE_PAGESIZE / 4], readdata[sEE_PAGESIZE / 4];
uint16_t uhNumDataRead;

uint8_t Read_Gensor_RegValue[1];

char str[50];

int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	USART_InitTypeDef USART_InitStructure;

	/* SysTick end of count event each 10ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	RNG_Cmd(ENABLE);

	STM_EVAL_LEDInit(LED1);
	STM_EVAL_LEDInit(LED2);
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_PBInit(BUTTON_WAKEUP, BUTTON_MODE_GPIO);
	STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_GPIO);
	STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);

	/*STM_EVAL_LEDOn(LED1);
	STM_EVAL_LEDOn(LED2);
	STM_EVAL_LEDOn(LED3);
	STM_EVAL_LEDOn(LED4);*/
	
	uint8_t dir = 0, i = 0;
	uint32_t sel[] = {LED1, LED2, LED3, LED4};
	while (1)
	{
		if(STM_EVAL_PBGetState(Button_KEY) == 0)
		{
			STM_EVAL_LEDOff(sel[i]);
			
			if(i == 3)
				i = 0;
			else
				i++;			
		}
		
		STM_EVAL_LEDOn(sel[i]);
		Delay(30);	
		
		/*while(1)
		{
			i++;
			if(i > 3) i = 0; 
			
			Delay(30);
			STM_EVAL_LEDToggle(sel[i]);
			Delay(30);
			
			if(STM_EVAL_PBGetState(Button_KEY) == 0)
				break;
		}
		
		while(1)
		{
			if(i == 0) i = 4; 			
			i--;
			
			STM_EVAL_LEDToggle(sel[i]);
			Delay(30);
			
			if(STM_EVAL_PBGetState(Button_KEY) == 0)
				break;
		}*/
	}
}

void Delay(__IO uint32_t nTime)
{
	uwTimingDelay = nTime;

	while (uwTimingDelay != 0);
}

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
