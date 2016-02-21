/*************************************************************************
	> File Name: ftrl.h
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: Wed 10 Jun 2015 12:13:41 PM CST
	> Brief: 
 ************************************************************************/
#ifndef _FTRL_H_
#define _FTRL_H_

#include <tr1/unordered_map>
#include "ftrl_common.h"

namespace ftrl
{
class FTRL
{
private:
	paramType_t l1;
	paramType_t l2;
	paramType_t alpha;
	paramType_t beta;
	paramType_t thr;
	numType_t thread_num;
	std::tr1::unordered_map<feaType_t,WgtInfo> WgtMap;
private:
	FTRL();
	~FTRL();
	/**
	 * 返回x的符号1或-1
	 **/
	valueType_t sgn(valueType_t x);
	/**
	 * 获取logloss
	 **/
	valueType_t logloss(valueType_t p, labelType_t y);
	/**
	 * 更新模型权重
	 **/
	int32_t update(valueType_t &p, std::vector<feaType_t> &features, labelType_t &label);
public:
	static FTRL* get_obj()
	{
		static FTRL ftrl_obj;
		return &ftrl_obj;
	}
	
	int32_t init(paramType_t l1, paramType_t l2, paramType_t alpha, paramType_t beta, paramType_t thr);
	int32_t destroy();
	int32_t load_weight(const std::string batch_weight_file);
	valueType_t predict(std::vector<feaType_t> &features);
	int32_t load_data(const std::string data_file, std::vector<Sample> data);
	int32_t train(const std::vector<Sample> &data);
	int32_t output_model(const std::string model_file);
};

}

#endif //_FTRL_H_
