#include "bmp.h"
#include <iostream>

void writebmp(uint8_t* src,uint32_t w, uint32_t h, uint8_t flg,char* dst_path){
	// src = byte data
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;

	fh.bfType = (uint16_t)0x4d42;
	fh.bfSize = (uint32_t)(sizeof(BITMAPINFOHEADER)+ sizeof(BITMAPFILEHEADER)+w*h*4);
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfOffBits = (uint32_t)(sizeof(BITMAPINFOHEADER)+ sizeof(BITMAPFILEHEADER));

	ih.biSize = sizeof(BITMAPINFOHEADER);
	ih.biHeight = h;
	ih.biWidth = w;
	ih.biPlanes = 1;
	ih.biBitCount = 32;
	ih.biCompression = 0;
	ih.biSizeImage = 4*w*h;
	ih.biXPelsPerMeter = 0;
	ih.biYPelsPerMeter = 0;
	ih.biClrUsed = 0;
	ih.biClrImportant = 0;

	FILE * pFile;
    pFile = fopen (dst_path, "wb+");
	fwrite(&fh,1,sizeof(BITMAPFILEHEADER),pFile);
	fwrite(&ih,1,sizeof(BITMAPINFOHEADER),pFile);
	uint8_t* pstart;
	if(flg == 1){
		pstart = src + 4*w*h -4;
		for(int i = 0 ; i < w*h ; i ++){
			fwrite((uint32_t*)pstart, 1, 4, pFile);
			pstart -= 4;
		}
		return ;
	}
}