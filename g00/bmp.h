#pragma once 
#pragma pack(1)
#include <stdint.h>

#define MAGICNUM 0xFEDCBA98
#define NULLNUM 0xFFFFFFFF

typedef struct tagBITMAPFILEHEADER 
{  
uint16_t bfType;    
uint32_t bfSize; 
uint16_t bfReserved1; 
uint16_t bfReserved2; 
uint32_t bfOffBits;
} BITMAPFILEHEADER; 

typedef struct tagBITMAPINFOHEADER{
	uint32_t      biSize;
	uint32_t       biWidth;
	uint32_t       biHeight;
	uint16_t       biPlanes;
	uint16_t       biBitCount;
	uint32_t      biCompression;
	uint32_t      biSizeImage;
	uint32_t       biXPelsPerMeter;
	uint32_t       biYPelsPerMeter;
	uint32_t      biClrUsed;
	uint32_t      biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagBITMAPPOSITION{
	// for merge usage
	uint32_t MAGIC;
	uint32_t bgx_start;
	uint32_t bgy_start;
	uint32_t bgx_end;
	uint32_t bgy_end;
	uint32_t sub_x;
	uint32_t sub_y;
	uint32_t sub_w;
	uint32_t sub_h;
	uint32_t params[2];
}BITMAPPOSITION;

void writebmp(uint8_t* src,uint32_t w, uint32_t h, uint8_t flg, char* dst_path,BITMAPPOSITION* p);
void fix_alpha(uint8_t* src, uint32_t len);