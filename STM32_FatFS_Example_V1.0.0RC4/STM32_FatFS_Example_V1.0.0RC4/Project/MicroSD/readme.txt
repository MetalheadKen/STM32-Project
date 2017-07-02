/**
  @page MicroSD  How to use the SDIO to drive an SD card memory
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0RC4
  * @date    21-December-2012
  * @brief   Description of the MicroSD Example.
  ******************************************************************************
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
  @endverbatim

@par Example Description 

This example demonstrates how to use the SDIO firmware library and an associate
driver to perform read/write operations on the SD Card memory that could be mounted
on the STM324xG_EVAL board.

This example supports SD Standard (SDSC), High capacity (SDHC) and High Speed (SDHS) cards. 

This example operates as below:
  - After reset, if an SD card is detected it displays its capacity on the EVAL board LCD
  - Performs read/write operations on the SD card
      - if SDHS card is detected, the switch to high-speed mode will be performed automatically.
      - if SDSC card is detected the read/write operation will be performed starting
        from address 0x040000000 (1st 1 GBytes)
      - if SDHC card is detected the read/write operation will be performed starting
        from address 0x140000000 (5 GBytes)   
  - finally it displays the results of read/write operations.

    @Note: For SDIO High Speed mode the SDIO clock (coming from the PLL through Q divider)
           is set to 48 MHz, but this is not functional. As workaround we have to
           configure the SDIO clock to 67.2 MHz and disable the bypass mode to output
           33.6 MHz for SD card. To output 67.2 MHz, the PLL_Q value should be set 
           to 5 into system_stm32f4xx.c file.
     
@par Hardware and Software environment

  -  This example is based and has been tested on the STM32F2xx and STM32F4xx devices 
     on STM322xG-EVAL (MB786 RevB) and STM324xG-EVAL (MB786 RevC) boards, and can
     be tailored to any other development board.    

  - STM322xG-EVAL and STM324xG-EVAL Set-up
   - Connect an uSD Card (maximum 4 GB) to the MSD connector (CN6).
         @note Make sure that jumper JP16 is set to position 1-2 (MSD).

@par How to use it ? 
  - Open the STM32_FATFS_USBH_SD.eww workspace.
  - In the workspace toolbar select the project config:
   - STM3210E-EVAL: to configure the project for STM32F10x High Density devices   
   - STM322xG-EVAL: to configure the project for STM32F2xx devices
   - STM324xG-EVAL: to configure the project for STM32F4xx devices
  - Rebuild all files: Project->Rebuild all
  - Load project image: Project->Debug
  - Run program: Debug->Go(F5)    


    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
