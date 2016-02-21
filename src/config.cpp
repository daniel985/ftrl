/*************************************************************************
	> File Name: config.cpp
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: Tue 16 Jun 2015 03:14:23 PM CST
	> Brief: 
 ************************************************************************/

#include <fstream>
#include <vector>
#include "config.h"
#include "splitString.h"
#include "glog/logging.h"

Config::Config(){}
Config::~Config(){}
int32_t Config::load(const std::string confile)
{
	std::ifstream rf;
	rf.open(confile.c_str(),std::ifstream::in);
	if (!rf.is_open())
	{
		LOG(ERROR) << "Open conf file failed!";
		return -1;
	}

	std::string line;
	while(getline(rf,line))
	{
		if (0 == line.length())continue;
		if ('#' == line[0])continue;
		std::vector<std::string> splitLine = splitString(line,"=");
		if (2 != splitLine.size())continue;
		else
		{
			conf[splitLine[0]] = splitLine[1];
		}
	}
	rf.close();
	return 0;
}

std::string Config::getString(const std::string &key, std::string defaultString)
{
	std::map<std::string,std::string>::const_iterator p = conf.find(key);
	if (conf.end() == p)
	{
		LOG(ERROR) << key << " is not found";
		return defaultString;
	}
	else
	{
		std::string value = p->second;
		return value;
	}
}

int32_t Config::getInt(const std::string &key, int32_t defaultInt)
{
	std::map<std::string,std::string>::const_iterator p = conf.find(key);
	if (conf.end() == p)
	{
		LOG(ERROR) << key << " is not found";
		return defaultInt;
	}
	else
	{
		std::string value = p->second;
		return atoi(value.c_str());
	}
}

double Config::getFloat(const std::string &key, double defaultFloat)
{
	std::map<std::string,std::string>::const_iterator p = conf.find(key);
	if (conf.end() == p)
	{
		LOG(ERROR) << key << " is not found";
		return defaultFloat;
	}
	else
	{
		std::string value = p->second;
		return atof(value.c_str());
	}
}

bool Config::getBool(const std::string &key, bool defaultBool)
{
	std::map<std::string,std::string>::const_iterator p = conf.find(key);
	if (conf.end() == p)
	{
		LOG(ERROR) << key << " is not found";
		return defaultBool;
	}
	else
	{
		std::string value = p->second;
		if ("true" == value)
		{
			return true;
		}
		else if ("false" == value)
		{
			return false;
		}
		else
		{
			return false;
		}
	}
}

