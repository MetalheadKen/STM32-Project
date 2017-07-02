/**
  ******************************************************************************
  * @file    sdio_sd_test.c
  * @author  MCD Application Team
  * @version V1.0.0RC4
  * @date    21-December-2012
  * @brief   This file contains function to test SDIO_SD available on STM32 EVAL 
  *          boards.
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
#include "sdio_sd_test.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SD_BLOCK_SIZE      512 /* Block Size in Bytes */
#define NUMBEROFBLOCKS     50 /* For Multi Blocks operation (Read/Write) */
#define MULTIBUFFERSIZE    (SD_BLOCK_SIZE * NUMBEROFBLOCKS)
#define SDHC_RW_ADDR       0x140000000 /* Read/write address starting from 5 GBytes
                                          For SDHC card the address should be multiple of 512 */
#define SDSC_RW_ADDR       0x040000000   /* Read/write address starting from 1 GBytes */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern SD_CardInfo SDCardInfo;
uint8_t Buffer_Block_Tx[SD_BLOCK_SIZE]={0}, Buffer_Block_Rx[SD_BLOCK_SIZE]={0};
uint8_t Buffer_MultiBlock_Tx[MULTIBUFFERSIZE], Buffer_MultiBlock_Rx[MULTIBUFFERSIZE], str[20];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;
uint64_t SDAddr = SDSC_RW_ADDR;

/* Private function prototypes -----------------------------------------------*/
TestStatus eBuffercmp(uint8_t* pBuffer, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the SDIO_SD test.
  * @param  None
  * @retval 0: card initialization passed
  *         1: card initialization failed
  */
uint32_t SDIO_SD_Test_Init(void)
{
  /* Initialize the LCD */
#if defined (USE_STM322xG_EVAL)
  STM322xG_LCD_Init();
#elif defined(USE_STM324xG_EVAL)
  STM324xG_LCD_Init();
#endif
  
  LCD_LOG_Init();
  LCD_LOG_SetHeader(" SDIO uSD Driver Example ");
  LCD_LOG_SetFooter("   Copyright (c) STMicroelectronics" );
  
#if defined (SD_POLLING_MODE)
  LCD_UsrLog("\n****** Test using SD_POLLING_MODE *****\n\n");
#elif defined (SD_DMA_MODE)
  LCD_UsrLog("\n****** Test using SD_DMA_MODE *****\n\n");
#else
  LCD_ErrLog("need to define SD_POLLING_MODE or SD_DMA_MODE\n");
#endif /* SD_POLLING_MODE */

  /* SD Init */
  Status = SD_Init();

  /* Verify the SDIO SD */
  if(Status == SD_OK)
  {
    LCD_UsrLog("SDIO SD intialized\n");

#ifndef USE_USB /* USE_USB should be defined in main.h when using USB applications */
    /* Check if the SD card supports high-speed mode */
    Status = SD_HighSpeed();
    if (Status == SD_OK)
    {
      /* For SDIO High Speed mode the SDIO clock (coming from the PLL through Q divider)
         is set to 48 MHz, but this is not functional (silicon bug). As workaround we have 
         to configure the SDIO clock to 67.2 MHz and disable the bypass mode to output 
         33.6 MHz for SD card. for this configuration, the PLL_Q value should be set 
         to 5.

         For SDIO High Speed mode, the normal config opeartion consist to enable the
         bypass mode (call SDIO_Init() function w/ SDIO_ClockBypass member of SDIO_InitStruct
         set to SDIO_ClockBypass_Enable) to drive directly the SDIO_CK output signal (48 MHz).
         Due to this limitation, the bypass mode should be disabled and the clock is 
         divided according to the CLKDIV value before driving the SDIO_CK output signal
         (67.2 / 2 = 33.6 MHz).
      */ 
      /* Clear PLLQ bits */
      RCC->PLLCFGR &= ~ RCC_PLLCFGR_PLLQ;

      /* Set PLLQ bits to 5 */
      RCC->PLLCFGR |= 5 << 24;

      LCD_UsrLog("Test_SD_High_Speed \n");
    }
    else
#endif
    {
      LCD_UsrLog("Test_SD_Standard_Speed\n");
      Status =SD_OK ;
    }
  }
  else
  {
    LCD_ErrLog(" SDIO SD init failed\n");
    return (1);
  } 
  sprintf((char*)str, "Capacity:%llu", SDCardInfo.CardCapacity);

  if (SDCardInfo.CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    LCD_UsrLog("Test_SD_High_Capacity \n");

    /* Update read/write address */
    SDAddr = SDHC_RW_ADDR;    
  }
  else if ( (SDCardInfo.CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1)||(SDCardInfo.CardType ==  SDIO_STD_CAPACITY_SD_CARD_V2_0))
  {
    LCD_UsrLog("Test_SD_Standard_Capacity \n");
  }

  /* Display the card Capacity */
  LCD_UsrLog(" %s\n", str);  

  /* Display the read/write address used in the example */
  sprintf((char*)str, "Example read/write @ :%llu", SDAddr);  
  LCD_UsrLog(" %s\n\n", str);  

  return (0);
}

/**
  * @brief  Test for SDIO_SD mounted on STM32 EVAL boards.
  * @param  None
  * @retval None
  */
void SDIO_SD_Test(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
#if defined (SD_DMA_MODE)
  /*------------------- Block Erase -------------------------------*/
  if (Status == SD_OK)
  {
    /* Erase NUMBEROFBLOCKS Blocks of WRITE_BL_LEN(512 Bytes) */
    Status = SD_Erase(SDAddr, SDAddr + (SD_BLOCK_SIZE * NUMBEROFBLOCKS));
  }

  if (Status == SD_OK)
  {
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, SDAddr, SD_BLOCK_SIZE, NUMBEROFBLOCKS);
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTIBUFFERSIZE);
  }

  if (EraseStatus != FAILED)
  {
    /* OK */
    LCD_UsrLog("SDCard Erase Success\n");
  }
  else
  {
    /* KO */
    LCD_ErrLog("SDCard Erase Failed\n");
  }
#endif /* SD_DMA_MODE */

  /*------------------- Block Read/Write --------------------------*/
  /* Fill the buffer to send */
  Fill_bBuffer(Buffer_Block_Tx, SD_BLOCK_SIZE, 0xC3);

  if (Status == SD_OK)
  {
    /* Write block of 512 bytes on address 0 */
    Status = SD_WriteBlock(Buffer_Block_Tx, SDAddr, SD_BLOCK_SIZE);
    /* this function insure that the DMA  controller has finished all data transfer */
#if defined (SD_DMA_MODE)
    Status = SD_WaitReadOperation();
#endif /* SD_DMA_MODE */
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    /* Read block of 512 bytes from address 0 */
    Status = SD_ReadBlock(Buffer_Block_Rx, SDAddr, SD_BLOCK_SIZE);
    /* this function insure that the DMA  controller has finished all data transfer */
#if defined (SD_DMA_MODE)
    Status = SD_WaitWriteOperation();
#endif /* SD_DMA_MODE */
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    /* Check the corectness of written dada */
    TransferStatus1 = bBufferCmp(Buffer_Block_Tx, Buffer_Block_Rx, SD_BLOCK_SIZE);
  }

  if (TransferStatus1 != FAILED)
  {
    /* OK */
    LCD_UsrLog("SDCard Block Success\n");
  }
  else
  {
    /* KO */
    LCD_ErrLog("SDCard Block Failed\n");
  }
    
  /*--------------- Multiple Block Read/Write ---------------------*/
#if defined (SD_DMA_MODE)
  /* Fill the buffer to send */
  Fill_bBuffer(Buffer_MultiBlock_Tx, MULTIBUFFERSIZE, 0xA5);

  if (Status == SD_OK)
  {
    /* Write multiple block of many bytes on address 0 */
    Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, SDAddr, SD_BLOCK_SIZE, NUMBEROFBLOCKS);
    Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    /* Read block of many bytes from address 0 */
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, SDAddr, SD_BLOCK_SIZE, NUMBEROFBLOCKS);
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    /* Check the corectness of written dada */
    TransferStatus2 = bBufferCmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTIBUFFERSIZE);
  }

  if (TransferStatus2 != FAILED)
  {
    /* OK */
    LCD_UsrLog("SDCard MBlock Succes\n");
  }
  else
  {
    /* KO */
    LCD_ErrLog("SDCard MBlock Failed\n");
  }
#endif /* SD_DMA_MODE */
  /* Test end */
  LCD_UsrLog("\n****** END *****\n");
}

/**
  *   Function name : eBuffercmp
  * @brief  Checks if a buffer has all its values are equal to zero.
  * @param  pBuffer: buffer to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer values are zero
  *   FAILED: At least one value from pBuffer buffer is diffrent 
  *   from zero.
  */
TestStatus eBuffercmp(uint8_t* pBuffer, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    /* In some SD Cards the erased state is 0xFF, in others it's 0x00 */
    if ((*pBuffer != 0xFF) && (*pBuffer != 0x00))
    {
      return FAILED;
    }

    pBuffer++;
  }

  return PASSED;
}

/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

/**
  * @brief  This function handles DMA2 global interrupt request.
  * @param  None
  * @retval None
  */
void SD_SDIO_DMA_IRQHANDLER(void)
{
  /* Process DMA2 Stream3 or DMA2 Stream6 Interrupt Sources */
  SD_ProcessDMAIRQ();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
