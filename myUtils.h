#pragma once
#include <string>
#include <fstream>
#include "logger.h"
#include <sstream>
#include <chrono>

// 储存排行榜数据的结构体
struct BestScoreUser
{
	std::string userName;
	std::string userScore;
	std::string curTime;
};

class MyUtils
{
	// 输入文件名，判断改文件存在且不为空
	static bool isFileExistAndNotEmpty(std::string fileName);

	// 将std::vector<std::string>格式的vector储存到fileName中，并返回储存结果
	static bool saveVectorToCsv(std::vector<BestScoreUser> v, std::string fileName);

	// 获取当前时间并返回string格式"%Y年%m月%d日%H时%M分%S秒"
	static std::string getCurTime();
};

