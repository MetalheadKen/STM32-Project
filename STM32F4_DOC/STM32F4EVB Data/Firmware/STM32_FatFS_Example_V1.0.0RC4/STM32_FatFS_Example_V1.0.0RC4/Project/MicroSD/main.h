/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @version V1.0.0RC4
  * @date    21-December-2012
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "global_includes.h"
#include "sdio_sd_test.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
TestStatus bBufferCmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint32_t BufferLength);
TestStatus hBufferCmp(uint16_t* pBuffer, uint16_t* pBuffer1, uint32_t BufferLength);
TestStatus wBufferCmp(uint32_t* pBuffer, uint32_t* pBuffer1, uint32_t BufferLength);
void Fill_bBuffer(uint8_t *pBuffer, uint32_t BufferLength, uint8_t Offset);
void Fill_hBuffer(uint16_t *pBuffer, uint32_t BufferLength, uint16_t Offset);
void Fill_wBuffer(uint32_t *pBuffer, uint32_t BufferLength, uint32_t Offset);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
