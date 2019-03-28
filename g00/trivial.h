#pragma once
#include <string>
#include <stdint.h>
#include <vector>

void getDirFileList(std::string &str, std::vector<std::string> &filev);
void split1(std::string &str, std::vector<std::string>& tokenv);
int isDirOrFileExist(std::string &pstr, bool isMkdir,std::string &err);
uint8_t* readFileUchar(std::string &str,int* len,uint8_t u);
void getFilenameFromPath(std::string &path, std::string &filename,uint8_t u);