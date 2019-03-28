#include "g00.h"
#include "merge.h"
#include "command.h"
#include <string>
#include <io.h>
#include <direct.h>
#include <iostream>

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