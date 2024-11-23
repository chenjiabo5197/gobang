#include "my_utils.h"

bool isFileExistAndNotEmpty(const std::string& fileName)
{
	std::ifstream ifs;
	ifs.open(fileName, std::ios::in);
	if (!ifs.is_open())
	{
		// 文件不存在
		ifs.close();
		WARNLOG("isFileExistAndNotEmpty||no such file||fileName={}", fileName);
		return false;
	}
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		// 文件为空
		ifs.close();
		WARNLOG("isFileExistAndNotEmpty||file is empty||fileName={}", fileName);
		return false;
	}
	ifs.close();
	INFOLOG("isFileExistAndNotEmpty||file is exist and not empty||fileName={}", fileName);
	return true;
}

bool saveVectorToCsv(std::vector<BestScoreUser>& v)
{
	std::ofstream ofs;
	// 覆盖写入，默认此时程序中维护的为最新的，因为需要考虑更新排行榜，排行榜数量最大为5个
	ofs.open(BESTSCORESFILE, std::ios::out);
	if (!ofs.is_open())
	{
		ERRORLOG("saveVectorToCsv||open file fail");
		return false;
	}
	for (std::vector<BestScoreUser>::iterator it = v.begin(); it != v.end(); it++)
	{
		ofs << it->userName << ",";
		ofs << it->userScore << ",";
		ofs << it->curTime << ",";
		ofs << std::endl;
		DEBUGLOG("saveVectorToCsv||write to {}||name={}||score={}||time={}", BESTSCORESFILE, it->userName, it->userScore, it->curTime);
	}
	ofs.close();
	INFOLOG("saveVectorToCsv||best scores save to file success||fileName={}", BESTSCORESFILE);
	return true;
}

std::string getCurTime()
{
	auto NowTime = std::chrono::system_clock::now();
	time_t ticks = std::chrono::system_clock::to_time_t(NowTime);
	struct tm* p_Time = new tm();

	#if defined(__linux__)
		// Linux系统
		localtime_r(&ticks, p_Time);
	#elif defined(_WIN32)
		// Windows系统
		localtime_s(p_Time, &ticks);
	#endif

	char c_TimeStamp[64];
	memset(c_TimeStamp, 0, sizeof(c_TimeStamp));
	strftime(c_TimeStamp, sizeof(c_TimeStamp), "%Y-%m-%d %H:%M:%S", p_Time);
	DEBUGLOG("getCurTime||get curTime success||time={}", c_TimeStamp);
	return c_TimeStamp;
}

std::vector<BestScoreUser> initBestScores()
{
	std::vector<BestScoreUser> tempScore;
	std::ifstream ifs;
	std::string str;
	// 文件不为空
	if (isFileExistAndNotEmpty(BESTSCORESFILE))
	{
		std::string buf;
		ifs.open(BESTSCORESFILE, std::ios::in);
		while (std::getline(ifs, buf))
		{
			BestScoreUser tempUser;
			std::vector<std::string> tempString;
			int start = 0;
			int pos = -1;
			while (true)
			{
				pos = buf.find(",", start);
				if (pos == -1)
				{
					break;
				}
				// DEBUGLOG("initBestScores||temp={}||buf={}||start={}||pos={}||len={}", buf.substr(start, pos - start), buf, start, pos, buf.size());
				tempString.push_back(buf.substr(start, pos - start));
				start = pos + 1;
			}
			tempUser.userName = tempString[0];
			tempUser.userScore = tempString[1];
			tempUser.curTime = tempString[2];
			DEBUGLOG("initBestScores||name={}||score={}||time={}", tempUser.userName, tempUser.userScore, tempUser.curTime);
			str += "{" + tempUser.userName + ", " + tempUser.userScore + ", " + tempUser.curTime + "}, ";
			tempScore.push_back(tempUser);
		}
	}
	// 文件为空
	else
	{
		tempScore.clear();
		WARNLOG("initBestScores||{} is empty or not exist", BESTSCORESFILE);
	}
	ifs.close();
	INFOLOG("initBestScores||init best scores success||BestScoreUser={}", str);
	return tempScore;
}

void updateBestScore(std::vector<BestScoreUser> & bestScores, const BestScoreUser& user)
{
	std::vector<BestScoreUser>::iterator it = bestScores.begin();
	int index = 0;
	for (; it != bestScores.end(); it++, index++)
	{
		// 找到插入位置了
		if (user.userScore < it->userScore) {
			DEBUGLOG("updateBestScore||find insert position||index={}", index);
			break;
		}
	}
	bestScores.insert(it, user);
	if (bestScores.size() > MAXBESTSCORES) {
		std::string lastItem = bestScores.back().userName + ", " + bestScores.back().userScore + ", " + bestScores.back().curTime;
		DEBUGLOG("updateBestScore||bestScore.size() over{}||pop={}", MAXBESTSCORES, lastItem);
		bestScores.pop_back();
	}
	INFOLOG("updateBestScore||updateBestScore success");
}

BestScoreUser getIBestScoreUser(const std::string& userName, const int& chessNum)
{
	BestScoreUser temp;
	temp.userName = userName;
	temp.userScore = std::to_string(chessNum);
	temp.curTime = getCurTime();
	return temp;
}

void sleepSeconds(const float& sceonds)
{
	#if defined(__linux__)
		// Linux系统
		usleep(1000 * 1000 * sceonds);
	#elif defined(_WIN32)
		// Windows系统
		Sleep((sceonds*1000));
	#endif
}

