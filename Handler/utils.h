#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../config/ConfigParse/inc/location.hpp"
#include <unistd.h>
#include <fcntl.h>


int write_file(std::string path, std::string content);
int delete_file(std::string path);

bool isPHPScript(std::string path);
bool checkAllowedMethods(Location location, std::string method);
std::string getFilenameFromRequestPath(std::string requestPath);
int findMatchingLocation(const std::string& requestPath, const std::vector<Location>& locations);
bool fileExists(std::string filepath);
bool directoryExits(std::string path);
std::string ListFile(std::string path);
bool isDirectory(std::string path);

int getFileFd(std::string path);

#endif