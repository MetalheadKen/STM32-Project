/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ff.h"
#include "diskio.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup SDIO_uSDCard
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BLOCK_SIZE            512 /* Block Size in Bytes */

#define NUMBER_OF_BLOCKS      100  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)

#define SD_OPERATION_ERASE          0
#define SD_OPERATION_BLOCK          1
#define SD_OPERATION_MULTI_BLOCK    2 
#define SD_OPERATION_END            3

#define IMAGE_LENGTH 400
#define IMAGE_WIDTH	 240
#define BLOCK_SIZE 512

#define MESSAGE				" SD CARD READY!!!  "
#define MESSAGE_OK		" SD CARD GOOD!!!  "
#define MESSAGE_ERROR " SD CARD ERROR!!!  "

#define SYSTEMTICK_PERIOD_MS 	10

#define NULL 0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t aBuffer_Block_Tx[BLOCK_SIZE]; 
uint8_t aBuffer_Block_Rx[BLOCK_SIZE];
uint8_t aBuffer_MultiBlock_Tx[MULTI_BUFFER_SIZE];
uint8_t aBuffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];

__IO TestStatus EraseStatus = FAILED;
__IO TestStatus TransferStatus1 = FAILED;
__IO TestStatus TransferStatus2 = FAILED;

SD_Error Status = SD_OK;
__IO uint32_t uwSDCardOperation = SD_OPERATION_ERASE;

static __IO uint32_t uwTimingDelay;
__IO uint32_t LocalTime = 0;

/* Private function prototypes -----------------------------------------------*/
static void NVIC_Configuration(void);
void Delay(volatile uint32_t);

/* Private functions ---------------------------------------------------------*/

int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	USART_InitTypeDef USART_InitStructure;
	
	/* SysTick end of count event each 10ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
	RNG_Cmd(ENABLE);

  /* Initialize LEDs available on EVAL board */
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
	
	//LCD_Init();
	STM324xG_LCD_Init();
	
	 /* Clear the LCD */
  LCD_Clear(Black);

  /* Set the LCD Back Color */
  LCD_SetBackColor(Grey);

  /* Set the LCD Text Color */
  LCD_SetTextColor(Blue);

  /* Display message on the LCD*/
  LCD_DisplayStringLine(Line0, (uint8_t*)MESSAGE);
	
  /* NVIC Configuration */
  NVIC_Configuration();

  /*------------------------------ SD Init ---------------------------------- */
  if((Status = SD_Init()) != SD_OK)
  {
    STM_EVAL_LEDOn(LED4); 
  }
	
	disk_initialize(0);
	
	FATFS fs;
	FIL fsrc;
	FRESULT ptr;
	
	ptr = f_open(&fsrc, "0:/test.txt", FA_CREATE_ALWAYS | FA_WRITE);
	
	UINT bw;
	BYTE buffer[] = {"CREATE SUCCESSFUL!!!"};
	
	f_mount(&fs, "0:/", 0);
	
	if(ptr != FR_OK)
	{
		LCD_DisplayStringLine(Line2, (uint8_t *)"CREATE FILE ERROR.");
	}
	else
	{
		LCD_DisplayStringLine(Line2, (uint8_t *)"CREATE SUCCESSFUL!!!");
		ptr = f_write(&fsrc, (const void *)buffer, sizeof(buffer), &bw);
		
		if(ptr == FR_OK)
		{
			LCD_DisplayStringLine(Line3, (uint8_t *)"WRITE SUCCESSFUL!!!");
		}
		else
		{
			LCD_DisplayStringLine(Line3, (uint8_t *)"WRITE FAIL!!!");
		}
	}
	
	f_close(&fsrc);
	f_mount(NULL, "0:/", 0);
		
	//uint8_t readbuffer[IMAGE_LENGTH * IMAGE_WIDTH] = {0};
	//uint64_t i, j;
  
  /* Infinite loop */
	/*SD_PowerON() != SD_OK || SD_InitializeCards() != SD_OK*/
#if 0
  while (1)
  {
		Delay(5);
		if(Status != SD_OK)
		{
			LCD_DisplayStringLine(Line0, (uint8_t *)MESSAGE_ERROR);
			STM_EVAL_LEDToggle(LED4);
			continue;
		}
				
		if(SD_GetStatus() == SD_TRANSFER_OK && SD_GetTransferState() == SD_TRANSFER_OK)
		{
			LCD_DisplayStringLine(Line0, (uint8_t *)MESSAGE_OK);
			
					
		}
  }
#endif
}

/**
  * @brief  Configures SDIO IRQ channel.
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);  
}

#ifdef  USE_FULL_ASSERT
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

void Time_Update(void)
{
	LocalTime += SYSTEMTICK_PERIOD_MS;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
