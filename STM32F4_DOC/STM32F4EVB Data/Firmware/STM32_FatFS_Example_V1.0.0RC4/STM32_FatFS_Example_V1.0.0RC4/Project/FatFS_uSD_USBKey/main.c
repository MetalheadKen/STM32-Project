/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.0RC4
  * @date    21-December-2012
  * @brief   This file provides main program functions
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
#include "global_includes.h"
#include <stdio.h>
#include <string.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE      USB_OTG_Core __ALIGN_END;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USBH_HOST                USB_Host __ALIGN_END;



uint32_t ret_length = 0;
FIL  USB_file;
FIL  SD_file;
FATFS USBH_fatfs, microSD_fatfs;
char MessageW1[] = "This is a FAT FS example on STM32 using USB Host interface.";
char MessageW2[] = "This is a FAT FS example on STM32 using microSD interface.";
char MessageR[256];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  __IO uint32_t i = 0;
  
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32fxxx_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32fxxx.c file
  */  
  
  memset (MessageR, 0, sizeof(MessageR));
  /* Init the hardware */
  BSP_Init();
  
  /* Init Host Library */
  USBH_Init(&USB_OTG_Core, 
#ifdef USE_USB_OTG_FS  
            USB_OTG_FS_CORE_ID,
#else 
            USB_OTG_HS_CORE_ID,
#endif 
            &USB_Host,
            &USBH_MSC_cb, 
            &USBH_USR_cb);


  
  while (USB_Host_Application_Ready == 0)
  {
    if (i++ == 0x10000)
    {
      STM_EVAL_LEDToggle(LED1);
      STM_EVAL_LEDToggle(LED2);
      STM_EVAL_LEDToggle(LED3);
      STM_EVAL_LEDToggle(LED4);
      i = 0;
    } 
    /* Host Task handler */
    USBH_Process(&USB_OTG_Core, &USB_Host);
  }
  
  /* Initialises the File System*/
  if ( f_mount( 0, &USBH_fatfs ) != FR_OK )
  {
    /* efs initialisation fails*/
    LCD_UsrLog("[FS] Cannot initialize FS on drive 0.\n");
    return -1;
  }
  LCD_UsrLog("[FS] FS on drive 0 initialized.\n");
  
  /* Start I/O operations */
  if (f_open(&USB_file, "0:/STM32_File.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
  {
    LCD_ErrLog("Cannot open 'STM32 File.txt' file\n");
  }
  else
  {
    LCD_DbgLog("'STM32 File.txt' file opened for write\n");
  }

  if (f_write(&USB_file, MessageW1, sizeof(MessageW1), &ret_length) != FR_OK)
  {
    LCD_ErrLog("Cannot write on 'STM32 File.txt' file\n");
  }
  else
  {
    LCD_DbgLog("'Data written in file\n");
  }  
  
  f_close(&USB_file);
  
  if (f_open(&USB_file, "0:/STM32_File.txt", FA_READ) != FR_OK)
  {
    LCD_ErrLog("Cannot open 'STM32 File.txt' file\n");
  }
  else
  {
    LCD_DbgLog("'STM32 File.txt' file opened for read\n");
  }

  if (f_read(&USB_file, MessageR, sizeof(MessageR), &ret_length) != FR_OK)
  {
    LCD_ErrLog("Cannot read from on 'STM32 File.txt' file\n");
  }
  else
  {
    LCD_UsrLog("Text : \n"); 
    strcat(MessageR, "\n");
    LCD_DbgLog(MessageR);
  } 
  f_close(&USB_file);
  
  memset (MessageR, 0 , sizeof(MessageR));
  
  /* Initialises the File System*/
  if ( f_mount( 1, &microSD_fatfs ) != FR_OK )
  {
    LCD_ErrLog("Cannot mount FATFS on Drive 1\n");
  }
  
  /* Start I/O operations */
  if (f_open(&SD_file, "1:/STM32_File.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
  {
    LCD_ErrLog("Cannot open 'STM32 File.txt' file\n");
  }
  else
  {
    LCD_DbgLog("'STM32 File.txt' file opened for write\n");
  }

  if (f_write(&SD_file, MessageW2, sizeof(MessageW2), &ret_length) != FR_OK)
  {
    LCD_ErrLog("Cannot write on 'STM32 File.txt' file\n");
  }
  else
  {
    LCD_DbgLog("'Data written in file\n");
  }  
  
  f_close(&SD_file);
  
  if (f_open(&SD_file, "1:/STM32_File.txt", FA_READ) != FR_OK)
  {
    LCD_ErrLog("Cannot open 'STM32 File.txt' file\n");
  }
  else
  {
    LCD_DbgLog("'STM32 File.txt' file opened for read\n");
  }

  if (f_read(&SD_file, MessageR, sizeof(MessageR), &ret_length) != FR_OK)
  {
    LCD_ErrLog("Cannot read from on 'STM32 File.txt' file\n");
  }
  else
  {
    LCD_UsrLog("Text : \n"); 
    strcat(MessageR, "\n");
    LCD_DbgLog(MessageR);
  } 
  
  f_close(&SD_file);
  
  
  while (1)
  {
    if (i++ == 0x100000)
    {
      STM_EVAL_LEDToggle(LED1);
      STM_EVAL_LEDToggle(LED2);
      STM_EVAL_LEDToggle(LED3);
      STM_EVAL_LEDToggle(LED4);
      i = 0;
    } 
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

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
