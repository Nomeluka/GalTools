#include "command.h"
#include "g00.h"
#include "merge.h"
#include "trivial.h"
#include <iostream>
#include <string>

void getCmdArgs(std::vector<std::string> &v){
	std::string cmdline;
	std::getline(std::cin,cmdline);
	split1(cmdline,v);
}

void run_g00(){

	std::vector<std::string> cmdargs;
	getCmdArgs(cmdargs);

	if(cmdargs.size() != 2){
		std::cout<<"Unknown command. "<<std::endl;
		return;
	}

	std::string info = "";

	int ret1 = isDirOrFileExist(cmdargs[0],0,info);
	int ret2 = isDirOrFileExist(cmdargs[1],(ret1>=0),info); //src path is a directory or file

	if(ret1 == 0 && ret2 == 0){
		std::cout<<"Extract directory from "<<cmdargs[0]<<" -> "<<cmdargs[1]<<std::endl;
		_run_g00(cmdargs[0],cmdargs[1],1);
	}else if(ret1 == 1 && ret2 == 0){
		std::cout<<"Extract file from "<<cmdargs[0]<<" -> "<<cmdargs[1]<<std::endl;
		_run_g00(cmdargs[0],cmdargs[1],0);
	}else{
			//std::cout<<ret1<<ret2;
			std::cout<<"Incorrect command. "<<std::endl;
	}
}

void run_merge(){
	std::vector<std::string> cmdargs;
	getCmdArgs(cmdargs);

	if(cmdargs.size()!= 3){
		std::cout<<"Unknown command. "<<std::endl;
		return;
	}

	std::string info = "";

	int ret1 = isDirOrFileExist(cmdargs[0],0,info);
	int ret2 = isDirOrFileExist(cmdargs[1],0,info);
	int ret3 = isDirOrFileExist(cmdargs[2],(ret1 == 1 && ret2 == 1),info); // two src paths are both files
	
	if(ret1 == 1 && ret2 == 1 && ret3 == 0){
		std::cout<<"merge "<<cmdargs[0]<<" with "<<cmdargs[1]<<" -> "<<cmdargs[2]<<std::endl;
		_run_merge(cmdargs[0],cmdargs[1],cmdargs[2]);
	}else{
		std::cout<<"Incorrect command. "<<std::endl;
	}

}

void run_help(){
	std::vector<std::string> cmdargs;
	getCmdArgs(cmdargs);
	
	if(cmdargs.size() > 0){
		std::cout<<"Unknown command"<<std::endl;
		return ;
	}

	std::cout<<"g00 [src file/dir] [dst dir]"<<std::endl
	<<"merge [src file1] [src file2] [dst dir]"<<std::endl;
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
		}else if(cmd == "merge"){
			run_merge();
		}else if(cmd == "h"){
			run_help();
		}else{
			std::cout<<">> Unknown command"<<std::endl;
		}
	}
}