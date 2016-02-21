/*************************************************************************
	> File Name: config.h
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: Thu 11 Jun 2015 10:01:23 AM CST
	> Brief: 
 ************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <map>

class Config
{
private:
	std::map<std::string,std::string> conf;

public:
	Config();
	~Config();
	int32_t load(const std::string confile);
	std::string getString(const std::string &key, const std::string defaultString);
	int32_t getInt(const std::string &key, int32_t defaultInt);
	double getFloat(const std::string &key, double defaultFloat);
	bool getBool(const std::string &key, bool defaultBool);
};



#endif //_CONFIG_H_
