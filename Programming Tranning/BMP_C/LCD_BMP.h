#ifndef __LCD_BMP_H
#define __LCD_BMP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm324xg_eval.h"

/*0到1字節*/
#define BM_B 	  0x42
#define BM_M 	  0x4D
#define BMP_CHECK 0x4D42

/*3到14字節*/
typedef struct tagBITMAPFILEHEADER {	
	//attention: sizeof(DWORD)=4 sizeof(WORD)=2
	DWORD bfSize;      /*文件大小*/
	WORD bfReserved1;  /*保留字，不考慮*/
	WORD bfReserved2;  /*保留字，不考慮*/
	DWORD bfOffBits;   /*實際位圖數據的偏移字節數，及前三個部分長度之何*/
} BITMAPFILEHEADER,tagBITMAPFILEHEADER;

/*14到53字節*/
typedef struct tagBITMAPINFOHEADER {
	//attention: sizeof(DWORD)=4 sizeof(WORD)=2
	DWORD biSize;          	/*指定此結構的寬度，為40*/
	LONG biWidth;          	/*位圖寬，說明本圖的寬度，像素為單位*/
	LONG biHeight;         	/*位圖高，說明本圖的高度，像素為單位*/
	WORD biPlanes;         	/*平面數，為1*/
	WORD biBitCount;      	/*採用顏色位數，可以是1、2、4、8、16、24，新的可以是32*/
	DWORD biCompression;    /*壓縮方式，可以是0、1、2，其中0表示不壓縮*/
	DWORD biSizeImage;      /*實際位圖數據占用的字節數*/
	LONG biXPelsPerMeter;   /*X方向方辨率*/
	LONG biYPelsPerMeter;   /*Y方向分辨率*/
	DWORD biClrUsed;        /*使用的顏色數，如果為0，則表示默認值(2 ^ 顏色位數)*/
	DWORD biClrImportant;   /*重要的顏色數，如果為0，則表示所有顏色都是重要的*/
} BITMAPINFOHEADER,tagBITMAPINFOHEADER;

void LCD_ReadBMP(uint16_t, uint16_t, uint8_t);

#ifdef __cplusplus
}
#endif
  
#endif /* __LCD_BMP_H */
 