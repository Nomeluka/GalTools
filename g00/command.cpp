#include "command.h"
#include "g00.h"
#include "trivial.h"
#include <iostream>
#include <string>

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

		if(cmd == "q")
			break;

		if(cmd == "g00"){
			run_g00();
		}else{
			std::cout<<">> Unknown command"<<std::endl;
		}
	}
}