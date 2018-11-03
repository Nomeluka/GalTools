#include "g00.h"
#include <string>
#include <io.h>
using namespace std;

int main(){

	string dirsrc = "D:\\g00\\";
	string dirdst = "D:\\g00ext\\";

	struct _finddata_t fileinfo;
	int hFile = 0;
	string p;
	hFile = _findfirst(p.assign(dirsrc).append("\\*").c_str(),&fileinfo);

	if(hFile>=0){
		while(_findnext(hFile,&fileinfo)==0){
			if(fileinfo.name != "."){
				string name = string(fileinfo.name);
				string src = dirsrc + name;
				string dst = dirdst + name.substr(0,name.length()-4)+".bmp";
				extractg00((char*)(src.c_str()),(char*)(dst.c_str())); 
			}
		}
	}

	//extractg00("D:\\g00\\_02ef_nyEDCG04.g00","D:\\g00ext\\_00ef_nyBG01.bmp");
}