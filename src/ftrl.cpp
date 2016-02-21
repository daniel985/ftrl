/*************************************************************************
	> File Name: ftrl.cpp
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: Wed 10 Jun 2015 01:38:46 PM CST
	> Brief: 
 ************************************************************************/
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */
#include "glog/logging.h"
#include "ftrl.h"
#include "splitString.h"

namespace ftrl
{
const valueType_t MIN = 1e-6;
const valueType_t LOS_MIN = 10e-15;
const valueType_t LOG_MAX = 35.0;

valueType_t FTRL::sgn(valueType_t value)
{
	if (value < 0)
	{
		return -1.0;
	}
	else if (value > 0)
	{
		return 1.0;
	}
	else 
	{
		return 0.0;
	}
}

valueType_t FTRL::logloss(valueType_t p, labelType_t y)
{
	p = std::max(std::min(p,1 - LOS_MIN),LOS_MIN);
	if (y == 1)
	{
		return -log(p);
	}
	else
	{
		return -log(1-p);
	}
}

FTRL::FTRL()
{
}
FTRL::~FTRL()
{
}

int32_t FTRL::init(paramType_t _l1, paramType_t _l2, paramType_t _alpha, paramType_t _beta, paramType_t _thr, numType_t _thread_num)
{
	l1 = _l1;
	l2 = _l2;
	alpha = _alpha;
	beta = _beta;
	thr = _thr;
	thread_num = _thread_num;
	LOG(INFO) << "ftrl init succ!";
	return 0;
}

int32_t FTRL::destroy()
{
	WgtMap.clear();
	return 0;
}

int32_t FTRL::load_weight(const std::string batch_weight_file)
{
	std::ifstream rwf;
	rwf.open(batch_weight_file.c_str(),std::ifstream::in);
	if (!rwf.is_open())
	{
		LOG(ERROR) << "batch_weight_file is empty!";
		return -1;
	}
	std::string line;
	while(getline(rwf,line))
	{
		//std::vector<std::string> WgtVec = splitString(line, "\t");
		std::vector<std::string> WgtVec = splitString(line, " ");
		if (2 != WgtVec.size())
		{
			LOG(ERROR) << "parse weight line failed" << line;
			continue;
		}
		//feaType_t key = WgtVec[0];
		//valueType_t value = atof(WgtVec[1].c_str());
		feaType_t key = WgtVec[1];
		valueType_t value = atof(WgtVec[0].c_str());
		WgtInfo wgt;
		wgt.w = value;
		wgt.n = 0.0;
		wgt.z = 0.0;
		wgt.pc = 0;
		wgt.nc = 0;
		WgtMap[key] = wgt;
	}
	rwf.close();
	LOG(INFO) << "load batch_weight succ!";
	return 0;
}

valueType_t FTRL::predict(std::vector<feaType_t> &features)
{
	valueType_t wsum = 0.0;
	for (indexType_t i = 0; i < features.size(); ++i)
	{
		WgtInfo wgt;
		std::tr1::unordered_map<feaType_t,WgtInfo>::const_iterator pw = WgtMap.find(features[i]);
		if (WgtMap.end() == pw)
		{
			wgt.w = 0.0;
			wgt.n = 0.0;
			wgt.z = 0.0;
			wgt.pc = 0;
			wgt.nc = 0;
			WgtMap[features[i]] = wgt;
		}
		else
		{
			wgt = pw->second;
		}
		//LOG(INFO) << "w:" << wgt.w;				
		//LOG(INFO) << "n:" << wgt.n;				
		//LOG(INFO) << "z:" << wgt.z;				
		wsum += wgt.w;
	}

	return 1.0/(1.0 + exp(-std::max(std::min(wsum,LOG_MAX),-LOG_MAX)));
}

int32_t FTRL::update(valueType_t &p, std::vector<feaType_t> &features, labelType_t &label)
{
	valueType_t grad = p - label;

	for (indexType_t i = 0; i < features.size(); ++i)
	{
		std::tr1::unordered_map<feaType_t,WgtInfo>::const_iterator pw = WgtMap.find(features[i]);
		WgtInfo wgt = pw->second;
		//LOG(INFO) << "w:" << wgt.w;				
		//LOG(INFO) << "n:" << wgt.n;				
		//LOG(INFO) << "z:" << wgt.z;				
		//LOG(INFO) << "grad:" << grad;				
		//LOG(INFO) << "alpha:" << alpha;				
		//LOG(INFO) << "beta:" << beta;				
		//LOG(INFO) << "L1:" << l1;				
		//LOG(INFO) << "L2:" << l2;				
		//LOG(INFO) << "sig_tmp:" << sqrt(wgt.n + grad*grad) - sqrt(wgt.n);				
		valueType_t sigma = (sqrt(wgt.n + grad*grad) - sqrt(wgt.n))/alpha;
		//LOG(INFO) << "sigma:" << sigma;				
		wgt.z += grad - sigma * wgt.w;
		wgt.n += grad*grad;
		if (label == 1)
		{
			wgt.pc += 1;
		}
		else
		{
			wgt.nc += 1;
		}
		//LOG(INFO) << features[i] << sqrt(wgt.n);
		//后更新w
		if (fabs(wgt.z) <= l1)
		{
			wgt.w = 0.0;
		}
		else
		{
			valueType_t sign = sgn(wgt.z);
			//LOG(INFO) << "w:" << wgt.w;				
			//LOG(INFO) << "n:" << wgt.n;				
			//LOG(INFO) << "z:" << wgt.z;				
			//LOG(INFO) << "fenmu:" << (beta + sqrt(wgt.n))/alpha + l2;				
			//LOG(INFO) << "fenzi:" << sign*l1 - wgt.z;
			wgt.w = (sign*l1 - wgt.z)/((beta + sqrt(wgt.n))/alpha + l2);
			//LOG(INFO) << "w:" << wgt.w;				
		}
		WgtMap[features[i]] = wgt;
	}
	return 0;
}

int32_t FTRL::load_data(const std::string data_file, std::vector<Sample> data)
{
	std::ifstream rsf;
	rsf.open(data_file.c_str(),std::ifstream::in);
	if (!rsf.is_open())
	{
		LOG(ERROR) << "data_file is empty!";
		return -1;
	}
	std::string line;
	while(getline(rsf,line))
	{
		//特殊处理,从hadoop上拉下的文件末尾有\t,要除去
		//std::vector<std::string> Sample = splitString(line, "\t");
		//std::vector<std::string> SamVec = splitString(Sample[0], ",");
		std::vector<std::string> SamVec = splitString(line, ",");
		if (2 != SamVec.size())
		{
			LOG(ERROR) << "parse data line failed!" << line;
			continue;
		}
		Sample sample;
		sample.label = atoi(SamVec[0].c_str());
		sample.features = splitString(SamVec[1], " ");
		data.push_back(sample);
	}
	rsf.close();

	return 0;
}

int32_t FTRL::train(const std::vector<Sample> &data)
{
	valueType_t total_loss = 0.0;
	uint64_t total_count = 0;
		valueType_t p = predict(x);
		total_loss += logloss(p,y);
		total_count += 1;
		update(p,x,y);
	LOG(INFO) << "train finishes!";
	LOG(INFO) << "total loss:" << total_loss; 
	LOG(INFO) << "total_count:" << total_count;
	LOG(INFO) << "avg_loss:" << total_loss/total_count;
}

int32_t FTRL::output_model(const std::string model_file)
{
	std::ofstream wmf;
	wmf.open(model_file.c_str(), std::ofstream::out);
	if (!wmf.is_open())
	{
		LOG(ERROR) << "fail to open model_file";
		return -1;
	}
	std::tr1::unordered_map<feaType_t,WgtInfo>::iterator iter;
	for (iter = WgtMap.begin(); iter != WgtMap.end(); ++iter)
	{
		feaType_t feature = iter->first;
		WgtInfo wgt = iter->second;
		paramType_t lr = 1/(1 + sqrt(wgt.pc+wgt.nc));
		//LOG(INFO) << feature << ":P:" << wgt.pc << ",N:" << wgt.nc << "," << lr << "," << wgt.w;
		if (lr <= thr && fabs(wgt.w) >= MIN)
		{
			wmf << wgt.w 
				<< " "
				<< feature
				<< "\n";
		}
	}
	wmf.close();

	LOG(INFO) << "output model succ";
	return 0;
}

}
