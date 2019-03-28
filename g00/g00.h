#pragma once
#pragma pack(1)
#include <stdint.h>
#include <vector>
#include "bmp.h"

struct g00_basic {
	uint8_t type;
	uint16_t bmpW;
	uint16_t bmpH;
};
struct g00_type_0 {
	uint32_t src_len;
	uint32_t dst_len;
};

//yet unknown
struct g00_type2_entry {
	uint32_t x1; // start x
	uint32_t y1; // start y
	uint32_t x2; // end x
	uint32_t y2; // end y
	uint32_t x3; //unknown
	uint32_t y3; //unknown
};

struct g00_type2{
	uint32_t g00_num; //g00 type2 file counter
	uint32_t src_len;
	uint32_t dst_len;
	g00_type2_entry* entrylist;
};

struct bmpoffset{
	uint32_t off;
	uint32_t len;
};
struct g00_stream{
	uint32_t bmp_num;
	uint32_t stream_offset;
	bmpoffset* bmpoff;
	uint8_t** bmpvector;
};

struct bmp_info{
	uint16_t flag1;
	uint16_t part_counter;

	uint32_t x; // start x
	uint32_t y; //start y
	uint32_t w; //width
	uint32_t h; //height
	uint32_t params1[2]; //unknown
	uint32_t width;
	uint32_t height;
	uint32_t params2[20];//unknown
};

struct bmp_part{
	uint16_t col;
	uint16_t row;
	uint16_t u; //1 is contour 0 is inner block
	uint16_t loop2; 
	uint16_t loop1;  
	uint16_t param1; //unused
	uint32_t params2[20];//unknown
};

int extractg00(std::string src_path,std::string file_name,std::string dst_path);
void __run_g00(std::string &dirsrc, std::string &name, std::string &dirdst,int* c, int* c1);
void _run_g00(std::string &dirsrc, std::string &dirdst, bool isDir);