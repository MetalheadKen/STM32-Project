#include "LCD_BMP.h"

void LCD_ReadBMP(uint16_t x, uint16_t y,unsigned, uint8_t *pic_name)
{
	int i, j, k;
	int width, height, l_width;

	BYTE red, green, blue;
	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;

	WORD fileType;

	unsigned int read_num;
	unsigned char tmp_name[20];

	if ( f_mount( 0, &fs ) != FR_OK )
	{
		LCD_DisplayStringLine(Line0, (uint8_t *)"Can not mount SD CARD !!!");
	}	

	bmpres = f_open( &file , (char *)pic_name, FA_OPEN_EXISTING | FA_READ);

	if(bmpres == FR_OK)
	{
		/*讀取位圖文件訊息*/
		f_read(&file, &fileType, sizeof(WORD), &read_num);

		if(fileType != BMP_CHECK)
		{
			LCD_DisplayStringLine(Line0, (uint8_t *)"File is not BMP file !!!");
		}

		f_read(&file, &bitHead, sizeof(tagBITMAPFILEHEADER), &read_num);

		showBmpHead(&bitHead);

		BMP_DEBUG_PRINTF(" ");

		//?????????

		f_read(&file,&bitInfoHead,sizeof(BITMAPINFOHEADER),&read_num);

		showBmpInforHead(&bitInfoHead);

		BMP_DEBUG_PRINTF(" ");

		}

		else

		{

		BMP_DEBUG_PRINTF("file open fail! ");

		return;
	}

	width = bitInfoHead.biWidth;

	height = bitInfoHead.biHeight;

	l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);        //??????????????32???

	if(l_width>960)

	{

	BMP_DEBUG_PRINTF(" SORRY, PIC IS TOO BIG (<=320) ");

	return;

	}

	if(bitInfoHead.biBitCount>=24)

	{

	bmp_lcd(x,240-y-height,width, height);    //LCD??????

	for(i=0;i<height+1; i++)

	{

	for(j=0; j<l_width; j++)  //?????????

	{

	f_read(&file,pColorData+j,1,&read_num);

	}

	for(j=0;j<width;j++)//??????

	{

	k = j*3;                                                                                //????K??????

	red = pColorData[k+2];

	green = pColorData[k+1];

	blue =  pColorData[k];

	LCD_WR_Data(RGB24TORGB16(red,green,blue));      //??LCD-GRAM

	}

	}

	bmp_lcd_reset();   //lcd??????

	}

	else

	{

	BMP_DEBUG_PRINTF("SORRY, THIS PIC IS NOT A 24BITS REAL COLOR");

	return ;
}

	f_close(&file);
}
