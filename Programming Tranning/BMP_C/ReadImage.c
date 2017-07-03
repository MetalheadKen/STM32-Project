#include <stdio.h>

#define LENGTH 320
#define WIDTH  240
#define RGB	   3
#define HEADER 54

int main(void) {
	int i, j, k;
	FILE *image_in, *image_out;
	unsigned char photo[LENGTH][WIDTH][RGB], header[HEADER];
	
	image_in = fopen("photo1.bmp", "rb");
	
	if(image_in == NULL)
		return 1;
	else
	{
		for(i = 0; i < HEADER; i++)
			header[i] = fgetc(image_in);
	}
	
	image_out = fopen("photo1.txt", "wb");
	
	if(image_out == NULL)
		return 1;
	else
	{
		fread(photo, sizeof(photo), 1, image_in);
		for(i = 0; i < WIDTH; i++)
		{
			for(j = 0; j < LENGTH; j++)
			{
				for(k = 0; k < RGB; k++)
				{
					//photo[i][j][k] = fgetc(image_in);
					//fputc(photo[i][j][k], image_out); /*會亂碼，須注意*/
					fprintf(image_out, "0x%X, ", photo[i][j][k]);
					printf("%3hhu ", photo[i][j][k]);					
				}
			}
			putchar('\n');
		}
	}
	
	fclose(image_in);
	fclose(image_out);
		
	return 0;
}