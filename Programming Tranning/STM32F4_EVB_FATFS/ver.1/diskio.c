/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "stm32f4xx.h"
#include "stm32f4_evb_sdio_sd.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#define BLOCK_SIZE 512


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
		return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
		SD_Error Status;
		/* Supports only single drive */
		if(pdrv)
		{
			return STA_NOINIT;
		}
	/*-------------------------- SD Init ----------------------------- */
		Status = SD_Init();
		if (Status != SD_OK )
		{
			return STA_NOINIT;
		}
		else
		{
			return RES_OK;
		}
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
		if(count > 1)
		{
			SD_ReadMultiBlocks(buff, sector * BLOCK_SIZE, BLOCK_SIZE, count);
		}
		else
		{
			SD_ReadBlock(buff, sector * BLOCK_SIZE, BLOCK_SIZE);
		}
		return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _FS_READONLY == 0
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
		if (count > 1)
		{
			SD_WriteMultiBlocks((uint8_t *)buff, sector * BLOCK_SIZE, BLOCK_SIZE, count);
		}
		else
		{
			SD_WriteBlock((uint8_t *)buff, sector * BLOCK_SIZE, BLOCK_SIZE);
		}
		return RES_OK;
}
#endif /*_FS_READONLY*/



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
		return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Get current time                                                      */
/*-----------------------------------------------------------------------*/ 
DWORD get_fattime(void)
{
return ((2011UL-1980) << 25)       // Year = 2011
| (3UL << 21)       // Month = Mar
| (26UL << 16)       // Day = 26
| (13U << 11)       // Hour = 22
| (19U << 5)       // Min = 30
| (0U >> 1)       // Sec = 0
;
} 
