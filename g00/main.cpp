#include "g00.h"
#include "merge.h"
#include "command.h"
#include <string>
#include <iostream>

void test_merge(){
	std::string string1 = "bg.bmp";
	std::string string2 = "kud.bmp";
	std::string string3 = ".";
	std::cout<<mergebmp(string1, string2, string3);
}

int main(){
	//test_extract();
	//test_merge();
	run_cmd();
	system("pause");
	return 0;
}