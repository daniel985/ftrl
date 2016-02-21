/*************************************************************************
	> File Name: ftrl_common.h
	> Author: daniel.wang
	> Mail: wangzhanoop@163.com 
	> Created Time: Wed 10 Jun 2015 11:58:45 AM CST
	> Brief: 
 ************************************************************************/
#ifndef _FTRL_COMMON_H_
#define _FTRL_COMMON_H_

#include <string>
#include <vector>

namespace ftrl
{
typedef std::string feaType_t;
typedef std::vector<feaType_t> feaVec;
typedef int32_t labelType_t;
typedef double valueType_t;
typedef float paramType_t;
typedef uint32_t indexType_t;
typedef uint32_t numType_t;

typedef struct _Sample
{
	labelType_t label;
	feaVec features;
}Sample;

typedef struct _WgtInfo
{
	valueType_t w; //<
	valueType_t n;  //<
	valueType_t z; //<
	indexType_t pc; //<
	indexType_t nc; //<
}WgtInfo;

}

#endif //_FTRL_COMMON_H_
