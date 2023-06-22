
#ifndef UTILS_RESPONSE_H
#define UTILS_RESPONSE_H

#include <string>
#include <fstream>
#include <iostream>

std::string getFileContent(std::string filename);
int getFileContentLength(std::string filename);
std::string getFileContentType(std::string filename);

#endif
