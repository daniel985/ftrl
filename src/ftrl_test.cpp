/*************************************************************************
	> File Name: ftr_main.cpp
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: Wed 10 Jun 2015 03:57:29 PM CST
	> Brief: 
 ************************************************************************/

#include <iostream>
#include <time.h>
#include <sys/stat.h>
#include "config.h"
#include "splitString.h"
#include "ftrl.h"
#include "glog/logging.h"

using namespace ftrl;

int32_t main(int32_t argc, char* argv[])
{
	google::InitGoogleLogging(argv[0]);
	FLAGS_log_dir = "./log";
	std::string default_conf_file = "./conf/ftrl.conf";
	Config g_conf = Config();
	if (-1 == g_conf.load(default_conf_file))
	{
		LOG(ERROR) << "load configuration failed";
		return -1;
	}
	valueType_t l1 = g_conf.getFloat("l1", 1.0);
	valueType_t l2 = g_conf.getFloat("l2", 1.0);
	valueType_t alpha = g_conf.getFloat("alpha", 1.0);
	valueType_t beta = g_conf.getFloat("beta", 1.0);
	indexType_t epoch = (indexType_t)g_conf.getInt("epoch", 1);
	std::string weight_file = g_conf.getString("weight_file", "");
	std::string sample_file = g_conf.getString("sample_file", "");
	std::string model_file = g_conf.getString("model_file", "");
	LOG(INFO) << "pramater:" << "\n"
		<< "L1:" << l1 << "\n"  
		<< "L2:" << l2 << "\n"
		<< "alpha:" << alpha << "\n"
		<< "beta:" << beta;

	FTRL* ftrl = FTRL::get_obj();
	ftrl->init(l1,l2,alpha,beta);
	ftrl->load_weight(weight_file);

	for (indexType_t i = 0; i < epoch; ++i)
	{
		ftrl->train(sample_file);
	}
	ftrl->output_model(model_file);
	return 0;
}

