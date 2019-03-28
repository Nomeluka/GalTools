#include "g00.h"
#include "merge.h"
#include "command.h"
#include <string>
#include <io.h>
#include <direct.h>
#include <iostream>

void test_extract(){
	/*
	std::string dirsrc = "D:\\gal\\g01\\";
	std::string dirdst = "D:\\gal\\g00ext1\\";
	struct _finddata_t fileinfo;
	int hFile = 0;
	std::string p;
	hFile = _findfirst(p.assign(dirsrc).append("\\*").c_str(),&fileinfo);

	int state, count = 0, count1 = 0;

	if(hFile>=0){
		while(_findnext(hFile,&fileinfo)==0){
			if(fileinfo.name != "."){
				std::string name = std::string(fileinfo.name);

				state = extractg00((char*)(dirsrc.c_str()), (char*)name.c_str(), (char*)(dirdst.c_str())); 

				if(state == 0) { //successfully extract a g00 file
					count1 ++;
					count ++;
					std::cout<<count<<" "<<dirsrc+name<<": OK"<<std::endl;
				}else if(state != -1){ // is a g00 or g01 but having other exceptions 
					count ++;
					
					std::string errinfo = dirsrc + name;
					switch (state){
					case -5: errinfo += ": mkdir error";break;
					case -3: errinfo += ": unsupported type";break;
					case -2: errinfo += ": file error";break;
					default: errinfo += ": unknown error";break;
					}
					std::cout<<count<<" "<<errinfo<<std::endl;
				}else{
					std::cout<<dirsrc+name<<": not g00 file"<<std::endl;
				}
			}
		}
		std::cout<<"successfully extract: "<<count1 <<"/"<<count<<std::endl;
	}
	*/
	
	/* test g00 type 2 */
	
	std::string string1 = "D:\\gal\\g00ext\\";
	std::string string2 = "N.g00";
	std::string string3 = "D:\\gal\\g00ext\\";
	std::cout<<extractg00(string1,string2,string3);
	
	/* test g00 type 0 */
	/*
	string2 = "FGKW01a.g00";
	std::cout<<extractg00(string1, string2, string3);
	*/

	/* test merge */
	//std::cout<<merge("D:\\gal\\g00ext\\cg.bmp","D:\\gal\\g00ext\\mask.bmp","D:\\gal\\blow.bmp");
	return;
}

void test_merge(){
	std::string string1 = "D:\\gal\\g00ext\\bg.bmp";
	std::string string2 = "D:\\gal\\g00ext\\kud.bmp";
	std::string string3 = "D:\\gal";
	std::cout<<mergebmp(string1, string2, string3);
}

int main(){
	//test_extract();
	test_merge();
	//run_cmd();
	system("pause");
	return 0;
}