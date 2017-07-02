/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4x7_eth_bsp.h"
#include "stm32f4_evb.h"
#include "stm32f4_evb_audio_codec.h"
#include "STM32f4_evb_fsmc_psram.h"
#include "stm32f4_evb_i2c_ee.h"
#include "stm32f4_evb_i2c_gsensor.h"
#include "stm32f4_evb_i2c_tsc.h"
#include "stm324xg_eval_lcd.h"
#include "stm32f4_evb_sdio_sd.h"

#define USE_WWDG	 0

#define USE_LCD        /* enable LCD  */  
//#define USE_DHCP       /* enable DHCP, if disabled static address is used */\
   
#define DEST_IP_ADDR0   192
#define DEST_IP_ADDR1   168
#define DEST_IP_ADDR2   0
#define DEST_IP_ADDR3   11

#define DEST_PORT       7

/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   2
#define MAC_ADDR1   0
#define MAC_ADDR2   0
#define MAC_ADDR3   0
#define MAC_ADDR4   0
#define MAC_ADDR5   0
 
/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   10
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1  

/* MII and RMII mode selection, for STM324xG-EVAL Board(MB786) RevB ***********/
//#define RMII_MODE  // User have to provide the 50 MHz clock by soldering a 50 MHz
                     // oscillator (ref SM7745HEV-50.0M or equivalent) on the U3
                     // footprint located under CN3 and also removing jumper on JP5. 
                     // This oscillator is not provided with the board. 
                     // For more details, please refer to STM3240G-EVAL evaluation
                     // board User manual (UM1461).

                                     
#define MII_MODE

/* Uncomment the define below to clock the PHY from external 25MHz crystal (only for MII mode) */
#ifdef 	MII_MODE
 #define PHY_CLOCK_MCO
#endif

/* STM324xG-EVAL jumpers setting
    +==========================================================================================+
    +  Jumper |       MII mode configuration            |      RMII mode configuration         +
    +==========================================================================================+
    +  JP5    | 2-3 provide 25MHz clock by MCO(PA8)     |  Not fitted                          +
    +         | 1-2 provide 25MHz clock by ext. Crystal |                                      +
    + -----------------------------------------------------------------------------------------+
    +  JP6    |          2-3                            |  1-2                                 +
    + -----------------------------------------------------------------------------------------+
    +  JP8    |          Open                           |  Close                               +
    +==========================================================================================+
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern uint16_t AUDIO_SAMPLE[];
extern uint8_t in_out_flag;
extern uint8_t in_out_enable;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void Time_Update(void);
void Delay(uint32_t nCount);

extern void SDCard_Config(void);
extern void InOut_Write(uint8_t);
extern void RTC_Config(void);
extern void RTC_Setting(void);
extern void RTC_TimeShow(void);
extern void RTC_DateShow(void);
extern void RTC_AlarmShow(void);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/