#include "g00.h"
#include <string>
#include <direct.h>
#include <io.h>
#include <iostream>
using namespace std;

void g002stream(uint8_t* src, uint8_t* dst, uint32_t dst_len, uint32_t src_len, uint8_t* dst_end){
	//reversed from Kud Wafter Realive engine
	uint8_t* src_end = src + src_len + 8;

	__asm{
		  PUSHAD
		  PUSHFD
          MOV ESI,src                         ;  ESI == g00 src address
          MOV EDI,dst                         ;  EDI == dst address
          MOV EBX,dst_len                     ;  EBX == dst length
		  XOR EDX, EDX

REALLIVE_0049F008:
          CMP EDI,dst_end
		  JGE SHORT REALLIVE_0049F04F
          CMP ESI,src_end
          JGE SHORT REALLIVE_0049F04F
          MOV DL,BYTE PTR [ESI]
          INC ESI
          MOV DH,8
REALLIVE_0049F017:
          CMP EDI,dst_end
          JGE SHORT REALLIVE_0049F04F
          TEST DL,1
          JE SHORT REALLIVE_0049F025
          DEC EBX
          MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
          JMP SHORT REALLIVE_0049F047
REALLIVE_0049F025:
          XOR EAX,EAX
          LODS WORD PTR DS:[ESI]
          MOV ECX,EAX
          SHR EAX,4
          AND ECX,0x0F
          ADD ECX,2
          PUSH ESI
          MOV ESI,EDI
          SUB ESI,EAX
          CMP ESI,src_end
          JL SHORT REALLIVE_POP
          CMP EBX,ECX
          JL SHORT REALLIVE_POP
          SUB EBX,ECX
          REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
          POP ESI
REALLIVE_0049F047:
          SHR DL,1
          DEC DH
          JNZ SHORT REALLIVE_0049F017
          JMP SHORT REALLIVE_0049F008
REALLIVE_POP:
		  POP ESI
REALLIVE_0049F04F:
		  POPFD
		  POPAD
	}
}

void g002bmp(uint8_t* src, uint8_t* dst, uint8_t* dst_end){
	//reversed Siglus engine from hatsuyuki sakura 
	__asm{
		mov esi, src;
		mov edi, dst;
		xor edx, edx;
Siglus_005CC15A:
		cmp edi, dword ptr dst_end;
		je short Siglus_005CC19B;
		mov dl, byte ptr [esi];
		inc esi;
		mov dh, 8;
Siglus_005CC164:
		cmp edi, dst_end;
		je short Siglus_005CC19B;
		test dl, 1;
		je short Siglus_005CC177;
		movs word ptr [edi], word ptr [esi];
		movs byte ptr [edi], byte ptr [esi];
		inc edi;
		jmp short Siglus_005CC193;
Siglus_005CC177:
		xor eax, eax;
		lods word ptr [esi];
		mov ecx, eax;
		shr eax, 4;
		shl eax, 2;
		and ecx, 0xf;
		add ecx, 1;
		mov ebx, esi;
		mov esi, edi;
		sub esi, eax;
		rep movs dword ptr [edi], dword ptr [esi];
		mov esi, ebx;
Siglus_005CC193:
		shr dl, 1;
		dec dh;
Siglus_005CC197:
		jnz short Siglus_005CC164;
Siglus_005CC199:
		jmp short Siglus_005CC15A;
Siglus_005CC19B:
	}
}

void get_g00_basic (uint8_t* ptr, g00_basic* g){
	g -> type = *ptr;
	g -> bmpW = *(uint16_t*)(ptr + 1);
	g -> bmpH = *(uint16_t*)(ptr + 3);
}

void get_type0_info (uint8_t* ptr, g00_type_0* g0){
	g0 ->src_len = *(uint32_t*)ptr;
	g0 ->dst_len = *(uint32_t*)(ptr + 4);
}

int extract_type0(uint8_t* ptr,g00_basic* gb, string src_path, string dst_path, string file_name_raw){
	g00_type_0 gt0;

	ptr += sizeof(g00_basic);
	get_type0_info(ptr,&gt0);
	uint8_t* bmpbuf = new uint8_t[gt0.dst_len];
	ptr += sizeof(g00_type_0);
	g002bmp(ptr,bmpbuf,bmpbuf+gt0.dst_len);
	
	string dst = string(dst_path + file_name_raw + ".bmp");

	writebmp(bmpbuf,gb->bmpW,gb->bmpH,0,(char*)dst.c_str(),NULL);

	delete bmpbuf;
	return 0;
}

void get_type2_info (uint8_t* ptr, g00_type2* g){
	g->g00_num = *(uint32_t*)ptr;
	g->src_len = *(uint32_t*)(ptr+4+24*g->g00_num); //encoded length
	g->dst_len = *(uint32_t*)(ptr+8+24*g->g00_num); //decoded length
	g->entrylist = new g00_type2_entry[g->g00_num];
	memcpy(g->entrylist,ptr+4,sizeof(g00_type2_entry)*g->g00_num);
}
void get_bmp_info(uint8_t* ptr, g00_stream* gs){
	gs->bmp_num = *(uint32_t*)ptr;
	gs->stream_offset = *(uint32_t*)(ptr+4);
	gs->bmpoff = new bmpoffset[gs->bmp_num];
	memcpy(gs->bmpoff,ptr+4,sizeof(bmpoffset)*gs->bmp_num);
	gs->bmpvector = new uint8_t*[gs->bmp_num];
	gs->bmpvector[0] = NULL;
}

void construct_bmp(uint8_t* src,uint8_t** bmp, bmpoffset* b,bmp_info** ptr,uint32_t dstlen){
	src += b->off;

	bmp_info* bmpinfo = (bmp_info*)src;
	*ptr = bmpinfo; 

	if(dstlen == b->off || b->len == 0){
		*bmp = NULL;
		return;
	}

	src += sizeof(bmp_info);
	*bmp = new uint8_t[4*bmpinfo->width*bmpinfo->height];
	memset(*bmp,0,4*bmpinfo->width*bmpinfo->height);

	for (int blockindex = 0; blockindex<bmpinfo->part_counter;blockindex++){
		bmp_part* bp = (bmp_part*)src;
		src += sizeof(bmp_part);

		for(int loopindex = 0 ; loopindex<bp->loop1; loopindex ++){
			uint32_t begin = 4*(bp->col+(bp->row + loopindex)*bmpinfo->width);

			/* test flag*/
			if(bp->u != 0){
				fix_alpha(src,4*bp->loop2);
			}

			memcpy(*bmp+begin,src,4*bp->loop2);

			src += 4*bp->loop2;
		}

	}

}

int extract_type2(uint8_t* ptr, g00_basic* gb, string src_path, string dst_path, string file_name_raw, uint32_t len){
	//get g00 type 2 info
	ptr += sizeof(g00_basic);
	g00_type2 g;
	get_type2_info(ptr,&g);
	
	//compare addresses
	uint8_t* p1 = new uint8_t[2*g.dst_len];
	uint8_t* p2 = new uint8_t[g.dst_len];
	uint8_t* plow;
	uint8_t* pstream;
	if((uint32_t)p1 < (uint32_t)p2){
		plow = p1;
		pstream = p2;
	}else{
		plow = p2;
		pstream = p1;
	}

	memcpy(plow,ptr,len);
	ptr = plow;

	//decode
	uint32_t l = g.src_len;
	g002stream(ptr+12+24*g.g00_num,pstream,g.dst_len,g.src_len,pstream+g.dst_len);

	//recover bmps
	g00_stream gs;
	get_bmp_info(pstream,&gs);

	uint32_t counter = 0;

	BITMAPPOSITION pos;

	for(int i=0; i<gs.bmp_num;i++){
		bmp_info* ptr = NULL;
		construct_bmp(pstream,&gs.bmpvector[i],&gs.bmpoff[i],&ptr,g.dst_len);

		if(gs.bmpvector[i] == NULL){
			continue;
		}

		counter ++;

		string bmp_path;
		if(gs.bmp_num == 1)
			bmp_path = dst_path + file_name_raw + ".bmp";
		else {
			if(counter == 1){
				dst_path = dst_path + file_name_raw+"\\";
				const char* d = dst_path.c_str();
				int a = _mkdir(d);
				if(a == 0 || (a == -1 && _access(d,0)==0))
					bmp_path = dst_path + to_string(static_cast<long long> (counter - 1))+".bmp";
				else
					return -5;
			}else
				bmp_path = dst_path + to_string(static_cast<long long> (counter - 1))+".bmp";
		}
		
		pos.bgx_start = g.entrylist[i].x1;
		pos.bgy_start = g.entrylist[i].y1;
		pos.bgx_end = g.entrylist[i].x2;
		pos.bgy_end = g.entrylist[i].y2;
		pos.sub_x = ptr->x;
		pos.sub_y = ptr->y;
		pos.sub_w = ptr->w;
		pos.sub_h = ptr->h;
		pos.params[0] = g.entrylist[i].x3;
		pos.params[1] = g.entrylist[i].y3;

		writebmp(gs.bmpvector[i],ptr->width,ptr->height,0,(char*)bmp_path.c_str(),&pos);

	}
	
	delete ptr; //copied g00 src
	delete pstream; // decoded data
	delete gs.bmpoff;
	delete g.entrylist;

	for (int j =0 ;j < gs.bmp_num ;j++){
		if(gs.bmpvector!=NULL)
			delete gs.bmpvector[j];
	}

	delete gs.bmpvector;

	return 0;
}

int extractg00(string src_path,string file_name, string dst_path){

	int namelen = file_name.length();
	if(namelen < 4)
		return -1;

	string g00 = file_name.substr(namelen-4,4);
	string file_name_raw = file_name.substr(0,namelen-4);

	if(g00 != ".g00" && g00!= ".g01")
		return -1; //not .g00 or .g01 file

	FILE* pFile;
	pFile= fopen((char*)(src_path+file_name).c_str(),"rb");

	if(!pFile)
		return -2; //file error

	fseek(pFile,0,SEEK_END);
	int len = ftell(pFile);
	fseek(pFile,0,SEEK_SET);

	uint8_t* g00buf = new uint8_t[len];
	fread(g00buf,len,1,pFile);
	fclose(pFile);

	g00_basic gb;
	get_g00_basic(g00buf, &gb);

	int ret_val = 0;

	switch (gb.type){
	case 0: ret_val = extract_type0(g00buf,&gb,src_path,dst_path,file_name_raw);break;
	case 2: ret_val = extract_type2(g00buf,&gb,src_path,dst_path,file_name_raw,len);break;
	default:ret_val = -3; break;
	}

	delete g00buf;
	return ret_val;
}; 