#include <io.h>
#include <vector>
#include <direct.h>
#include <iostream>
#include <string>
#include <regex>
#include <sys/stat.h>
#include "command.h"
#include "g00.h"

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

void __run_g00(std::string &dirsrc, std::string &name, std::string &dirdst,int* c, int* c1){

	int state = extractg00((char*)(dirsrc.c_str()), (char*)name.c_str(), (char*)(dirdst.c_str())); 

	if(state == 0) { //successfully extract a g00 file
		(*c1)++;
		(*c) ++;
		std::cout<<*c<<" "<<dirsrc+name<<": OK"<<std::endl;
	}else if(state != -1){ // is a g00 or g01 but having other exceptions 
		(*c)++;
		std::string errinfo = dirsrc + name;
		switch (state){
		case -5: errinfo += ": mkdir error";break;
		case -3: errinfo += ": unsupported type";break;
		case -2: errinfo += ": file error";break;
		default: errinfo += ": unknown error";break;
	}
	std::cout<<(*c)<<" "<<errinfo<<std::endl;
	}else{
		//std::cout<<dirsrc+name<<": not g00 file"<<std::endl;
	}
}

void _run_g00(std::string &dirsrc, std::string &dirdst, bool isDir){

	if(isDir == true && dirsrc[dirsrc.length() - 1] != '\\')
		dirsrc += "\\";

	if(dirdst[dirdst.length() - 1] != '\\')
		dirdst += "\\";

	std::cout<<dirdst<<std::endl;
	//std::cout<<dirsrc<<" "<<dirdst<<std::endl;
	int c = 0; 
	int c1 = 0;

	if(isDir){
		std::vector<std::string> fv;
		getDirFileList(dirsrc,fv);
		for(auto i = fv.begin(); i != fv.end(); i++){
			//std::cout<<*i<<std::endl;
			__run_g00(dirsrc,*i,dirdst,&c,&c1);
		}
		std::cout<<"successfully extract: "<<c1<<"/"<<c<<std::endl;
	}else{
		int index = 0;
		for(int i = dirsrc.length()-1; i>=0 ;i--){
			if(dirsrc[i] == '\\'){
				index = i;
				break;
			}
		}
		
		__run_g00(dirsrc.substr(0,index),dirsrc.substr(index),dirdst,&c,&c1);
		std::cout<<"successfully extract: "<<c1<<"/"<<c<<std::endl;
	}
}

void run_g00(){
	std::string cmdline;
	std::getline(std::cin,cmdline);
	std::vector<std::string> cmdargs;
	split1(cmdline,cmdargs);

	if(cmdargs.size() != 2){
		std::cout<<"Unknown command. "<<std::endl;
		return;
	}

	std::string info = "";

	int ret1 = isDirOrFileExist(cmdargs[0],0,info);
	bool mkdir = (ret1 >= 0);
	int ret2 = isDirOrFileExist(cmdargs[1],mkdir,info);

	if(ret1 == 0 && ret2 == 0){
		std::cout<<"Extract directory from "<<cmdargs[0]<<" -> "<<cmdargs[1]<<std::endl;
		_run_g00(cmdargs[0],cmdargs[1],1);
	}else if(ret1 == 1 && ret2 == 0){
		std::cout<<"Extract file from "<<cmdargs[0]<<" -> "<<cmdargs[1]<<std::endl;
		_run_g00(cmdargs[0],cmdargs[1],0);
	}else{
			std::cout<<ret1<<ret2;
			std::cout<<"Incorrect command. "<<std::endl;
	}
}

void run_cmd(){
	std::string cmd;

	while(1){
		std::cout<<"Prompt: ";
		std::cin>>cmd;

		if(cmd == "q"){
			break;
		}

		if(cmd == "g00"){
			run_g00();
		}else{
			std::cout<<">> Unknown command"<<std::endl;
		}
	}
}