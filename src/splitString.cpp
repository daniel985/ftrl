/*************************************************************************
	> File Name: splitString.cpp
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: Thu 11 Jun 2015 10:23:19 AM CST
	> Brief: 
 ************************************************************************/

#include "splitString.h"

std::vector<std::string> splitString(const std::string& str, const char* separator)
{
	std::vector<std::string> result;
	std::string::size_type prev_pos = 0;
	std::string::size_type pos = 0;
	while ((pos = str.find_first_of(separator, prev_pos)) != std::string::npos)
	{
		if (prev_pos < pos)
		{
			result.push_back(str.substr(prev_pos, pos-prev_pos));
		}
		prev_pos = pos + 1;
	}
	if (prev_pos < str.size())
	{
		result.push_back(str.substr(prev_pos));
	}
	return result;
}

