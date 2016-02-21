/*************************************************************************
	> File Name: src/splitString_test.cpp
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: Wed 17 Jun 2015 12:05:29 PM CST
	> Brief: 
 ************************************************************************/
#include <fstream>
#include <iostream>
#include "splitString.h" 

int32_t main(int32_t argv, char* args[])
{
	std::ifstream rf;
	rf.open("./data/train_data");
	if (!rf.is_open())
	{
		std::cerr << "open train_data failed" <<std::endl;
		return -1;
	}
	std::string line;
	while(getline(rf,line))
	{
		std::vector<std::string> SamVec = splitString(line, "\t");
		if (2 != SamVec.size())
		{
			std::cerr << "parse line failed:" << line;
			continue;
		}
		std::vector<std::string> x = splitString(SamVec[1], ",");
		//std::cout << "label:" << SamVec[0] << "\nfeas:" << SamVec[1] << "\n" << std::endl; 
		for (size_t i = 0; i < x.size(); ++i)
			std::cout << "fea:" << x[i] << "\n";

	}
	return 0;
}

