#include "bmp.h"
#include <stdio.h>

void writebmp(uint8_t* src,uint32_t w, uint32_t h, uint8_t flg, char* dst_path, BITMAPPOSITION* p){
	// src = byte data
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;

	fh.bfType = (uint16_t)0x4d42;
	fh.bfSize = (uint32_t)(sizeof(BITMAPINFOHEADER)+ sizeof(BITMAPFILEHEADER)+ sizeof(BITMAPPOSITION)+ w*h*4);
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfOffBits = (uint32_t)(sizeof(BITMAPINFOHEADER)+ sizeof(BITMAPFILEHEADER) + sizeof(BITMAPPOSITION));

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
    pFile = fopen (dst_path, "wb");
	fwrite(&fh,1,sizeof(BITMAPFILEHEADER),pFile);
	fwrite(&ih,1,sizeof(BITMAPINFOHEADER),pFile);
	
	if(p == NULL){
		BITMAPPOSITION n = {MAGICNUM,NULLNUM};
		fwrite(&n,1,sizeof(BITMAPPOSITION),pFile);
	}else{
		p->MAGIC = 0xFEDCBA98;
		fwrite(p,1,sizeof(BITMAPPOSITION),pFile);
	}
	//fwrite(p,1,sizeof(uint32_t)*10,pFile);

	for(int i = h ; i > 0 ; i--){
		fwrite(src+4*(i-1)*w,1,4*w,pFile);
	}

	fflush(pFile);
	fclose(pFile);
}

void fix_alpha(uint8_t* src, uint32_t len){
	for(uint32_t i = 0; i < (len>>2) ; i++){
		uint32_t j = i << 2; 
		uint8_t alpha = *(src + j + 3);
		src[j] = (src[j] * alpha + 0x7F)/0xFF;
		src[j + 1] = (src[j + 1] * alpha + 0x7F)/0xFF;
		src[j + 2] = (src[j + 2] * alpha + 0x7F)/0xFF;
	}
}