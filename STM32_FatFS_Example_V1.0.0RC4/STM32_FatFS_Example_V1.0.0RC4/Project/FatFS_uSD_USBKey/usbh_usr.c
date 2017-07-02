/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V1.0.0RC4
  * @date    21-December-2012
  * @brief   This file includes the usb host library user callbacks
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
#include "usbh_usr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USBH_USR_cb =
  {
    USBH_USR_Init,
    USBH_USR_DeInit,
    USBH_USR_DeviceAttached,
    USBH_USR_ResetDevice,
    USBH_USR_DeviceDisconnected,
    USBH_USR_OverCurrentDetected,
    USBH_USR_DeviceSpeedDetected,
    USBH_USR_Device_DescAvailable,
    USBH_USR_DeviceAddressAssigned,
    USBH_USR_Configuration_DescAvailable,
    USBH_USR_Manufacturer_String,
    USBH_USR_Product_String,
    USBH_USR_SerialNum_String,
    USBH_USR_EnumerationDone,
    USBH_USR_UserInput,
    USBH_USR_MSC_Application,
    USBH_USR_DeviceNotSupported,
    USBH_USR_UnrecoveredError
  };

uint8_t USB_Host_Application_Ready;

/*--------------- LCD Messages ---------------*/
const uint8_t MSG_HOST_INIT[]        = "[USB] Host Library Initialized\n";
const uint8_t MSG_DEV_ATTACHED[]     = "[USB] Device Attached\n";
const uint8_t MSG_DEV_DISCONNECTED[] = "[USB] Device Disconnected\n";
const uint8_t MSG_DEV_ENUMERATED[]   = "[USB] Enumeration completed\n";
const uint8_t MSG_DEV_HIGHSPEED[]    = "[USB] High speed device detected\n";
const uint8_t MSG_DEV_FULLSPEED[]    = "[USB] Full speed device detected\n";
const uint8_t MSG_DEV_LOWSPEED[]     = "[USB] Low speed device detected\n";
const uint8_t MSG_DEV_ERROR[]        = "[USB] Device fault\n";

const uint8_t MSG_MSC_CLASS[]        = "[USB] Mass storage device connected\n";
const uint8_t MSG_HID_CLASS[]        = "[USB] HID device connected\n";
const uint8_t MSG_ROOT_CONT[]        = "[USB] Exploring disk flash ...\n";
const uint8_t MSG_WR_PROTECT[]       = "[USB] The disk is write protected\n";
const uint8_t MSG_UNREC_ERROR[]      = "[USB] UNRECOVERED ERROR STATE\n";


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
* @brief  USBH_USR_Init
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
  LCD_UsrLog ("[USB] Host Initialized.\n" );
  USB_Host_Application_Ready = 0;
}

/**
* @brief  USBH_USR_DeviceAttached
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
  LCD_UsrLog((void *)MSG_DEV_ATTACHED);
}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{

  /* Set default screen color*/
  LCD_UsrLog((void *)MSG_UNREC_ERROR);
}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Staus
*/
void USBH_USR_DeviceDisconnected (void)
{
  LCD_UsrLog((void *)MSG_DEV_DISCONNECTED);
  USB_Host_Application_Ready = 0;
}
/**
* @brief  USBH_USR_ResetUSBDevice
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* callback for USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected
*         Displays the message on LCD for device speed
* @param  Device speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if (DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
    LCD_UsrLog((void *)MSG_DEV_HIGHSPEED);
  }
  else if (DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
    LCD_UsrLog((void *)MSG_DEV_FULLSPEED);
  }
  else if (DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
    LCD_UsrLog((void *)MSG_DEV_LOWSPEED);
  }
  else
  {
    LCD_UsrLog((void *)MSG_DEV_ERROR);
  }
}

/**
* @brief  USBH_USR_Device_DescAvailable
*         Displays the message on LCD for device descriptor
* @param  device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
  USBH_DevDesc_TypeDef *hs = DeviceDesc;
  char temp[40];

  sprintf (temp, "[USB] VID : %04Xh\n" , (uint32_t)(*hs).idVendor);
  LCD_UsrLog(temp);
  sprintf (temp, "[USB] PID : %04Xh\n" , (uint32_t)(*hs).idProduct);
  LCD_UsrLog(temp);
}

/**
* @brief  USBH_USR_DeviceAddressAssigned
*         USB device is successfully assigned the Address
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{
}

/**
* @brief  USBH_USR_Conf_Desc
*         Displays the message on LCD for configuration descriptor
* @param  Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
    USBH_InterfaceDesc_TypeDef *itfDesc,
    USBH_EpDesc_TypeDef *epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;

  id = itfDesc;

  if ((*id).bInterfaceClass  == 0x08)
  {
    LCD_UsrLog((void *)MSG_MSC_CLASS);
  }
  else if ((*id).bInterfaceClass  == 0x03)
  {
    LCD_UsrLog((void *)MSG_HID_CLASS);
  }
}

/**
* @brief  USBH_USR_Manufacturer_String
*         Displays the message on LCD for Manufacturer String
* @param  Manufacturer String
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
  char temp[40];
  sprintf (temp, "[USB] Manufacturer : %s\n", (char *)ManufacturerString);
  LCD_UsrLog(temp);
}

/**
* @brief  USBH_USR_Product_String
*         Displays the message on LCD for Product String
* @param  Product String
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{

  char temp[40];
  sprintf (temp, "[USB] Product : %s\n", (char *)ProductString);
  LCD_UsrLog(temp);
}

/**
* @brief  USBH_USR_SerialNum_String
*         Displays the message on LCD for SerialNum_String
* @param  SerialNum_String
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{

  char temp[40];
  sprintf (temp, "[USB] S\\N : %s\n", (char *)SerialNumString);
  LCD_UsrLog(temp);
}



/**
* @brief  EnumerationDone
*         User response request is displayed to ask application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{

  /* Enumeration complete */
  LCD_UsrLog((void *)MSG_DEV_ENUMERATED);
}

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
  LCD_UsrLog ("[USB] Device not supported.\n");
}

/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{
  //return USBH_USR_NO_RESP;
  return USBH_USR_RESP_OK;
}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Over Current Detected on VBUS
* @param  None
* @retval Staus
*/
void USBH_USR_OverCurrentDetected (void)
{
  LCD_UsrLog ("[USB] Overcurrent detected.\n");
}


/**
* @brief  USBH_USR_MSC_Application
*         Demo application for mass storage
* @param  None
* @retval Staus
*/
int USBH_USR_MSC_Application(void)
{

  char temp[40];
  
  if(USB_Host_Application_Ready == 0)
  {
    sprintf (temp, "[FS] USB Disk capacity:%d MB\n", (USBH_MSC_Param.MSCapacity * \
      USBH_MSC_Param.MSPageLength) / 1024 / 1024);
    LCD_UsrLog(temp);
    
    USB_Host_Application_Ready = 1;
  }
  return(0);
}
/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

