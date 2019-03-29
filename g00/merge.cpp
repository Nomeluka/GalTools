#include "merge.h"
#include "trivial.h"
#include "bmp.h"
#include <iostream>
#include <string>
#include <stdint.h>

void _merge(uint8_t* p1, uint8_t* p2, uint32_t cgw, uint32_t maskw, uint32_t maskh){
	
	for(uint32_t i = 0; i< maskh; i++){
		for(uint32_t j = 0; j<maskw; j++){
			uint32_t v = *(uint32_t*)(p1+4*i*cgw+4*j);
			uint32_t u = *(uint32_t*)(p2 + 4*i*maskw + 4*j);
			if( u >= 0xFF000000 ){
				*(uint32_t*)(p1+4*i*cgw+4*j) = u;
			}
		}
	}
}

int mergebmp(std::string &cg_path,std::string &mask_path,std::string &dir){

	int len1,len2;
	uint8_t* src1 = readFileUchar(cg_path,&len1,0);
	uint8_t* src2 = readFileUchar(mask_path,&len2,0);
	if(src1 == NULL || src2 == NULL)
		return -1; // file error

	BITMAPFILEHEADER* f1 = (BITMAPFILEHEADER*) src1;
	BITMAPFILEHEADER* f2 = (BITMAPFILEHEADER*) src2;

	BITMAPINFOHEADER* info1 = (BITMAPINFOHEADER*)(src1 + sizeof(BITMAPFILEHEADER));
	BITMAPINFOHEADER* info2 = (BITMAPINFOHEADER*)(src2 + sizeof(BITMAPFILEHEADER));

	BITMAPPOSITION* pos1 = (BITMAPPOSITION*)(src1 + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER));
	BITMAPPOSITION* pos = (BITMAPPOSITION*)(src2 + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER));

	if(info1->biWidth<info2->biWidth+pos->bgx_start || info1->biHeight<info2->biHeight+pos->bgy_start)
		return -2 ; // bmp size incorrect

	if(pos1->bgx_start != NULLNUM || pos->MAGIC != MAGICNUM || pos1->MAGIC != MAGICNUM)
		return -3; //not a CG bmp or not g00 origin

	uint8_t* pstart1 = src1 + f1->bfOffBits;
	uint8_t* pstart2 = src2 + f2->bfOffBits;

	_merge(pstart1+4*info1->biWidth*pos->bgy_start + 4*pos->bgy_start,pstart2,info1->biWidth,info2->biWidth,info2->biHeight);

	std::string cg_name;
	std::string mask_name;
	getFilenameFromPath(cg_path,cg_name,0);
	getFilenameFromPath(mask_path,mask_name,0);

	if(dir[dir.length()-1] != '\\')
		dir += "\\";

	std::string dst_path = dir + cg_name + "["+mask_name+"]" +".bmp";

	FILE* out = fopen(dst_path.c_str(),"wb");
	fwrite(src1,1,len1,out);
	fclose(out);

	delete src1;
	delete src2;
	return 0;
}

void _run_merge(std::string &cg_path,std::string &mask_path,std::string &dir){
	int state = mergebmp(cg_path,mask_path, dir);

	std::string info =  cg_path +" <- "+ mask_path;

	switch(state){
	case 0: info += " OK";break;
	case -1: info += " file error";break;
	case -2: info += " bmp size error";break;
	case -3: info += " missing g00 position info";break;
	default : info += " unknown error";break;
	}
	std::cout<<info<<std::endl;
}