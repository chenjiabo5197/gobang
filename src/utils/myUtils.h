#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../logger/logger.h"

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

class MyUtils
{
public:

	// 输入文件名，判断改文件存在且不为空
	static bool isFileExistAndNotEmpty(const std::string& fileName);

	// 将std::vector<std::string>格式的vector储存到BESTSCORESFILE中，并返回储存结果
	static bool saveVectorToCsv(std::vector<BestScoreUser>& v);

	// 获取当前时间并返回string格式"%Y年%m月%d日%H时%M分%S秒"
	static std::string getCurTime();

	// 排行榜初始化
	static std::vector<BestScoreUser> initBestScores();

	// 向排行榜插入
	static void updateBestScore(std::vector<BestScoreUser> & bestScores, const BestScoreUser & user);

	// 通过输入姓名组建结构体
	static BestScoreUser getIBestScoreUser(const std::string& userName, const int& chessNum);

	// 让程序休眠的秒数
	static void sleep_seconds(const float& seconds);
};

