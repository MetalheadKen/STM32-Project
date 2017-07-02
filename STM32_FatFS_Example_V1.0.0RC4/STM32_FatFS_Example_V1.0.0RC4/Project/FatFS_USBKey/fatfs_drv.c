/**
  ******************************************************************************
  * @file    fatfs_drv.c
  * @author  MCD Application Team
  * @version V1.0.0RC4
  * @date    21-December-2012
  * @brief   diskio interface 
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
#include "diskio.h"

#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */


BYTE status = USBH_MSC_OK;
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
   * @brief  Initialize Disk Drive  
   * @param   drv : driver index
   * @retval DSTATUS : operation status
  */

DSTATUS disk_initialize ( BYTE drv )
{

  Stat = STA_NOINIT;
  
  if(( drv == 0) && (USB_Host_Application_Ready))
  {  
    Stat &= ~STA_NOINIT;
  }
  
  return Stat;
}

/**
   * @brief  Get Disk Status  
   * @param   drv : driver index
   * @retval DSTATUS : operation status
  */
DSTATUS disk_status ( BYTE drv	)
{  
  Stat = STA_NOINIT;
  
  if(( drv == 0) && (USB_Host_Application_Ready))
  {  
    Stat &= ~STA_NOINIT;
  }
  return Stat;  
}

/**
   * @brief  Read Sector(s) 
   * @param   drv : driver index
   * @retval DSTATUS : operation status
  */
DRESULT disk_read (
                   BYTE drv,			/* Physical drive number (0) */
                   BYTE *buff,			/* Pointer to the data buffer to store read data */
                   DWORD sector,		/* Start sector number (LBA) */
                   BYTE count			/* Sector count (1..255) */
                     )
{
  
  uint32_t timeout = 0x1FFFF;
  
  if(( drv == 0) && (USB_Host_Application_Ready))
  {  
    while ((status != USBH_MSC_OK) && (timeout -- > 0))
    {
      ;
    }
    do
    {
      status = USBH_MSC_Read10(&USB_OTG_Core, (uint8_t *)buff,sector,512 * count);
      USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
      
      if(!HCD_IsDeviceConnected(&USB_OTG_Core))
      { 
        return RES_ERROR;
      }      
    }
    while(status == USBH_MSC_BUSY );
    
    if(status == USBH_MSC_OK)
      return RES_OK;
  }
  return RES_ERROR;
}
/**
   * @brief  write Sector(s) 
   * @param   drv : driver index
   * @retval DSTATUS : operation status
  */

#if _READONLY == 0
DRESULT disk_write (
                    BYTE drv,			/* Physical drive number (0) */
                    const BYTE *buff,	/* Pointer to the data to be written */
                    DWORD sector,		/* Start sector number (LBA) */
                    BYTE count			/* Sector count (1..255) */
                      )
{
  
  uint32_t timeout = 0x1FFFF;
  
  if(( drv == 0) && (USB_Host_Application_Ready))
  {  
    while ((status != USBH_MSC_OK) && (timeout -- > 0))
    {
      ;
    }        
    do
    {
      status = USBH_MSC_Write10(&USB_OTG_Core, (uint8_t *)buff,sector,512 * count);
      USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
      
      if(!HCD_IsDeviceConnected(&USB_OTG_Core))
      { 
        return RES_ERROR;
      }      
    }
    while(status == USBH_MSC_BUSY );
    
    
    
    if(status == USBH_MSC_OK)
      return RES_OK;
  }
  return RES_ERROR;
}
#endif /* _READONLY == 0 */


/**
   * @brief  I/O control operation
   * @param   drv : driver index
   * @retval DSTATUS : operation status
  */


#if _USE_IOCTL != 0
DRESULT disk_ioctl (
                    BYTE drv,		/* Physical drive number (0) */
                    BYTE ctrl,		/* Control code */
                    void *buff		/* Buffer to send/receive control data */
                      )
{
  DRESULT res;
  SD_CardInfo SDCardInfo;    
  
  if (drv) return RES_PARERR;
  
  res = RES_ERROR;
  
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  
  switch (ctrl) {
  case CTRL_SYNC :		/* Make sure that no pending write process */
    
    res = RES_OK;
    break;
    
  case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
    
    *(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;
    res = RES_OK;
    break;
    
  case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
    *(WORD*)buff = 512;
    res = RES_OK;
    break;
    
  case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
    
    *(DWORD*)buff = 512;
    break;
    
    
  default:
    res = RES_PARERR;
  }
  
  
  
  return res;
}
#endif /* _USE_IOCTL != 0 */
/**
   * @brief  Get Time from RTC
   * @param   None
   * @retval Time in DWORD
  */

DWORD get_fattime (void)
{
  return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
