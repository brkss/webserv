#include <stdio.h>
#include <unistd.h>
#include "Utils.hpp"
#include <iostream>


#include <vector>
int main(int ac,  char	**av)
{
	std::string test(";a;b;;;;;;;;;;;;;;hello");

	std::vector<std::string> vec = Utils::split(test, ";");
	for (int i = 0; i < vec.size() ; i++) {
		std::cout << "[" << vec[i] << "]" << std::endl;
	}
	return (0);
}

