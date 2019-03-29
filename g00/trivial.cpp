#include "trivial.h"
#include <regex>
#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <sys/stat.h>

void getDirFileList(std::string &str, std::vector<std::string> &filev){

	filev.clear();

	struct _finddata_t info;
	int hFile = 0;
	std::string p;
	hFile = _findfirst(p.assign(str).append("\\*").c_str(),&info);

	if(hFile >= 0){
		while(_findnext(hFile,&info) == 0){
			if(info.name != ".")
				filev.push_back(info.name);
		}
	}
}

void split1(std::string &str, std::vector<std::string>& tokenv){

	tokenv.clear();

	std::regex r("(\\S+)");
	for(std::sregex_iterator ri = std::sregex_iterator(str.begin(),str.end(),r);
		ri != std::sregex_iterator();
		++ ri
		){
			tokenv.push_back(ri->str());
	}

}

int isDirOrFileExist(std::string &pstr, bool isMkdir,std::string &err){
	if(pstr != ""){
		struct stat statbuf;
		if(stat(pstr.c_str(),&statbuf) == 0){
			if(statbuf.st_mode & S_IFDIR){
				return 0;
			}else if(statbuf.st_mode & S_IFREG){
				return 1;
			}else
				return -6; //unknown path type
		}else{
			if(isMkdir == true){
				if(_mkdir((char*)pstr.c_str()) == 0)
					return 0; //create dir
				else
					return -5; //mkdir fails
			}else
				return -3;// file or dir does not exists
		}
	}else
		return -1; //empty path
}

uint8_t* readFileUchar(std::string &str,int* len,uint8_t u){
	FILE* pFile = fopen(str.c_str(),"rb");
	if(!pFile)
		return NULL;
	
	fseek(pFile,0,SEEK_END);
	(*len) = ftell(pFile);
	fseek(pFile,0,SEEK_SET);

	uint8_t* buf = new uint8_t[*len];
	fread(buf,*len,1,pFile);
	fclose(pFile);
	return buf;
}

void getFilenameFromPath(std::string &path, std::string &filename,uint8_t u){

	char name[0x100];
	if(u == 0)
		_splitpath(path.c_str(),NULL,NULL,name,NULL);
	else if(u == 1)
		_splitpath(path.c_str(),NULL,name,NULL,NULL);

	filename = std::string(name);
}