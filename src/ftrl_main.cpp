/*************************************************************************
	> File Name: ftr_main.cpp
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: Wed 10 Jun 2015 03:57:29 PM CST
	> Brief: 
 ************************************************************************/

#include <iostream>
#include <cstdlib>
#include <ctime>
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
	//std::string default_conf_file = "./conf/ftrl.conf";
	std::string conf_file = argv[1];
	Config g_conf = Config();
	if (-1 == g_conf.load(conf_file))
	{
		LOG(ERROR) << "load configuration failed";
		return -1;
	}
	paramType_t l1 = g_conf.getFloat("l1", 1.0);
	paramType_t l2 = g_conf.getFloat("l2", 1.0);
	paramType_t alpha = g_conf.getFloat("alpha", 1.0);
	paramType_t beta = g_conf.getFloat("beta", 1.0);
	paramType_t thr = g_conf.getFloat("thr", 1.0);
	indexType_t epoch = (indexType_t)g_conf.getInt("epoch", 1);
	int32_t slp_time = g_conf.getInt("sleep_time", 600);
	int32_t start_t = g_conf.getInt("start_t", 10);
	int32_t end_t = g_conf.getInt("end_t", 24);
	int32_t max_try_times = g_conf.getInt("max_try_times",10);
	bool lwt = g_conf.getBool("load_weight", false);
	std::string weight_file = g_conf.getString("weight_file", "");
	std::string sample_file = g_conf.getString("sample_file", "");
	std::string model_file = g_conf.getString("model_file", "");
	std::string sample_done = g_conf.getString("sample_done", "");
	std::string model_done = g_conf.getString("model_done", "");
	LOG(INFO) << "Params:" << "\n"
		<< "L1:" << l1 << "\n"  
		<< "L2:" << l2 << "\n"
		<< "alpha:" << alpha << "\n"
		<< "beta:" << beta << "\n"
		<< "thr:" << thr;
	FTRL* ftrl = FTRL::get_obj();
	ftrl->init(l1,l2,alpha,beta,thr);
	if (lwt)
	{
		ftrl->load_weight(weight_file);
	}

	int32_t try_times = 0;
	while (true)
	{
		time_t nowtime = time(NULL);
		struct tm* ctime = localtime(&nowtime);
		//运行时段：从start_t到end_t
		if (ctime->tm_hour < start_t || ctime->tm_hour > end_t)
		{
			ftrl->destroy();
			LOG(INFO) << "Run finished!";
			return 0;
		}
		
		while (try_times < max_try_times)
		{
			char test_cmd[100];
			sprintf(test_cmd,"test -e %s",sample_done.c_str());
			if (0 != system(test_cmd))
			{
				LOG(INFO) << "sample is not ready, wait...";
				try_times += 1;
				sleep(slp_time);
			}
			else break;
		}
		if (try_times >= max_try_times)
		{
			ftrl->destroy();
			LOG(ERROR) << "train data is not exist and try times is over max times, exit!";
			return -1;
		}
		try_times = 0;
	
		//重新加载一次配置文件
		if (-1 == g_conf.load(conf_file))
		{
			LOG(ERROR) << "load configuration failed";
			return -1;
		}
		l1 = g_conf.getFloat("l1", 1.0);
		l2 = g_conf.getFloat("l2", 1.0);
		alpha = g_conf.getFloat("alpha", 1.0);
		beta = g_conf.getFloat("beta", 1.0);
		thr = g_conf.getFloat("thr", 1.0);
		epoch = (indexType_t)g_conf.getInt("epoch", 1);
		//slp_time = g_conf.getInt("sleep_time", 600);
		//start_t = g_conf.getInt("start_t", 10);
		//end_t = g_conf.getInt("end_t", 24);
		//max_try_times = g_conf.getInt("max_try_times",10);
		//lwt = g_conf.getBool("load_weight", false);
		//weight_file = g_conf.getString("weight_file", "");
		//sample_file = g_conf.getString("sample_file", "");
		//model_file = g_conf.getString("model_file", "");
		//sample_done = g_conf.getString("sample_done", "");
		//model_done = g_conf.getString("model_done", "");
		LOG(INFO) << "Params:" << "\n"
			<< "L1:" << l1 << "\n"  
			<< "L2:" << l2 << "\n"
			<< "alpha:" << alpha << "\n"
			<< "beta:" << beta << "\n"
			<< "thr:" << thr;
		ftrl->init(l1,l2,alpha,beta,thr);
		
		for (indexType_t i = 0; i < epoch; ++i)
		{
			ftrl->train(sample_file);
			LOG(INFO) << "end train epoch:" << i;
		}

		if (0 == ftrl->output_model(model_file))
		{
			char touch_cmd[100];
			sprintf(touch_cmd,"touch %s",model_done.c_str());
			system(touch_cmd);
		}
		char rm_cmd[100];
		sprintf(rm_cmd,"rm -rf %s",sample_done.c_str());
		system(rm_cmd);
	}
	return 0;
}

