/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0RC4
  * @date    21-December-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */  

  /* SysTick end of count event each 10ms */
  SysTick_Config(SystemCoreClock / 100);

  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
              
  /* SDIO SD card driver test *************************************************/
  if (SDIO_SD_Test_Init() == 0)
  {
    SDIO_SD_Test();

    while (1)
    {
      /* Toggle LD4 */
      STM_EVAL_LEDToggle(LED4);

      /* Insert 50 ms delay */
      Delay(5);

      /* Toggle LD4 */
      STM_EVAL_LEDToggle(LED4);

     /* Insert 100 ms delay */
     Delay(10);
    }  
  }
  else
  {
   /* Toggle LD3 */
    STM_EVAL_LEDOn(LED3);

    while (1)
    {
    }
  }
}
																 
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/**
  * @brief  Compares two 8-bit buffers and returns the comparison result
  * @param  pBuffer: pointer to the source buffer to be compared to.
  * @param  pBuffer1: pointer to the second source buffer to be compared to the first.
  * @retval
  *    - 0: Comparison is OK (the two Buffers are identical)
  *    - Value different from 0: Comparison is NOK (Buffers are different)
  */
TestStatus bBufferCmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if ((uint8_t)(*pBuffer) != (uint8_t)(*pBuffer1))
    {
      return FAILED;
    }

    pBuffer++;
    pBuffer1++;
  }

  return PASSED;
}

/**
  * @brief  Compares two 16-bit buffers and returns the comparison result
  * @param  pBuffer: pointer to the source buffer to be compared to.
  * @param  pBuffer1: pointer to the second source buffer to be compared to the first.
  * @retval
  *    - 0: Comparison is OK (the two Buffers are identical)
  *    - Value different from 0: Comparison is NOK (Buffers are different)  
  */
TestStatus hBufferCmp(uint16_t* pBuffer, uint16_t* pBuffer1, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != *pBuffer1)
    {
      return FAILED;
    }

    pBuffer++;
    pBuffer1++;
  }

  return PASSED;
}

/**
  * @brief  Compares two 24/32-bit buffers and returns the comparison result
  * @param  pBuffer: pointer to the source buffer to be compared to.
  * @param  pBuffer1: pointer to the second source buffer to be compared to the first.
  * @retval
  *    - 0: Comparison is OK (the two Buffers are identical)
  *    - Value different from 0: Comparison is NOK (Buffers are different)
  */
TestStatus wBufferCmp(uint32_t* pBuffer, uint32_t* pBuffer1, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != *pBuffer1)
    {
      return FAILED;
    }

    pBuffer++;
    pBuffer1++;
  }

  return PASSED;
}

/**
  * @brief  Fill a gloal 8-bit buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferSize: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer  
  * @retval None
  */
void Fill_bBuffer(uint8_t *pBuffer, uint32_t BufferLenght, uint8_t Offset)
{
  uint32_t IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++)
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

/**
  * @brief  Fill a gloal 16-bit buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferSize: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer  
  * @retval None
  */
void Fill_hBuffer(uint16_t *pBuffer, uint32_t BufferLenght, uint16_t Offset)
{
  uint32_t IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++)
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

/**
  * @brief  Fill a gloal 32-bit buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferSize: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer  
  * @retval None
  */
void Fill_wBuffer(uint32_t *pBuffer, uint32_t BufferLenght, uint32_t Offset)
{
  uint32_t IndexTmp;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++)
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

   LCD_DisplayStringLine(Line0, "assert_param error!!");

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
