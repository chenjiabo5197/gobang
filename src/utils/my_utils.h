#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "logger.h"

#if defined(__linux__)
// Linux系统
#include<unistd.h>
#elif defined(_WIN32)
// Windows系统
#include<windows.h>
#endif

#define BESTSCORESFILE "bestScoresFile.csv"
#define MAXBESTSCORES 10

// 储存排行榜数据的结构体
struct BestScoreUser
{
	std::string userName;
	std::string userScore;
	std::string curTime;
};


// 输入文件名，判断改文件存在且不为空
extern bool isFileExistAndNotEmpty(const std::string& fileName);

// 将std::vector<std::string>格式的vector储存到BESTSCORESFILE中，并返回储存结果
extern bool saveVectorToCsv(std::vector<BestScoreUser>& v);

// 获取当前时间并返回string格式"%Y年%m月%d日%H时%M分%S秒"
extern std::string getCurTime();

// 排行榜初始化
extern std::vector<BestScoreUser> initBestScores();

// 向排行榜插入
extern void updateBestScore(std::vector<BestScoreUser> & bestScores, const BestScoreUser & user);

// 通过输入姓名组建结构体
extern BestScoreUser getIBestScoreUser(const std::string& userName, const int& chessNum);

// 让程序休眠的秒数
extern void sleepSeconds(const float& seconds);

// 交换两个值
template<typename T>
extern void swapValue(T &value1, T &value2)
{
	T temp = value1;
	value1 = value2;
	value2 = temp;
}

