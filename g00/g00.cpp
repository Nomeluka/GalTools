#include "g00.h"
#include <string>
#include <iostream>
using namespace std;

void g002bmp(uint8_t* src, uint8_t* dst, uint8_t* dst_end){
	//SiglusEngine from katsuyuki sakura 
	// LZSS decompress algorithm
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


int extractg00(char* src_path,char* dst_path){
	/*
	Have problems when using ifstream to read
    ifstream fi;
    fi.open("C:\\zl.g00");
    fi.seekg(0,ios::end);
    int len = fi.tellg();
    fi.seekg(0,ios::beg);
    
    uint8* buf = new uint8[len];
    fi.read((char*)buf,len);
    fi.close();
    */
	
	string s_src_path(src_path);
	string g00 = s_src_path.substr(s_src_path.length()-4,4);
	if(g00 != ".g00"){
		cout<<"not g00 type"<<endl;
		return 0;
	}
	FILE* pFile;
	pFile= fopen(src_path,"rb");
	fseek(pFile,0,SEEK_END);
	int len = ftell(pFile);
	fseek(pFile,0,SEEK_SET);
	uint8_t* buf = new uint8_t[len];
	fread(buf,len,1,pFile);
	fclose(pFile);

    uint8_t * type = (uint8_t*)buf;
	if(*type != 0){
		cout<<"not 00 type"<<endl;
		return 0;
	}
	cout<<src_path<<endl;
    uint32_t* clen = (uint32_t*)(buf + 5);
    uint32_t* dclen = (uint32_t*)(buf + 9);
    uint16_t* x = (uint16_t*)(buf + 1);
    uint16_t* y = (uint16_t*)(buf + 3);
    //cout<<"g00 info£º ";
    //cout<<dec<<*x<<" "<<*y<<" "<<*clen<<" "<<*dclen<<endl;
    uint8_t* dbuf = new uint8_t[*dclen];
    
    uint8_t* src = buf + 13;
    uint8_t* dst = dbuf;
    uint8_t* dst_end = dbuf + (*dclen);

	//cout<<"g00->bmp... ";
	g002bmp(src,dst,dst_end);
	//cout<<"ok"<<endl;

	//cout<<"writing bmp... ";
	writebmp(dbuf,(uint32_t)(*x),(uint32_t)(*y),1,dst_path);
	//cout<<"ok"<<endl;
	delete dbuf;
	delete buf;
    return 0;
}; 