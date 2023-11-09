#include "myUtils.h"

bool MyUtils::isFileExistAndNotEmpty(std::string fileName)
{
	std::ifstream ifs;
	ifs.open(fileName, std::ios::in);
	if (!ifs.is_open())
	{
		// �ļ�������
		ifs.close();
		WARNLOG("MyUtils::isFileExistAndNotEmpty||no such file||fileName={}", fileName);
		return false;
	}
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		// �ļ�Ϊ��
		ifs.close();
		WARNLOG("MyUtils::isFileExistAndNotEmpty||file is empty||fileName={}", fileName);
		return false;
	}
	ifs.close();
	INFOLOG("MyUtils::isFileExistAndNotEmpty||file is exist and not empty||fileName={}", fileName);
	return true;
}

bool MyUtils::saveVectorToCsv(std::vector<BestScoreUser> v, std::string fileName)
{
	std::ofstream ofs;
	// ����д�룬Ĭ�ϴ�ʱ������ά����Ϊ���µģ���Ϊ��Ҫ���Ǹ������а����а��������Ϊ5��
	ofs.open(fileName, std::ios::out);
	if (!ofs.is_open())
	{
		ERRORLOG("MyUtils::saveVectorToCsv||open file fail");
		return false;
	}
	for (std::vector<BestScoreUser>::iterator it = v.begin(); it != v.end(); it++)
	{
		ofs << it->userName << ",";
		ofs << it->userScore << ",";
		ofs << it->curTime;
		ofs << std::endl;
		DEBUGLOG("MyUtils::saveVectorToCsv||write to {}||name={}||score={}||time={}", fileName, it->userName, it->userScore, it->curTime);
	}
	ofs.close();
	INFOLOG("MyUtils::saveVectorToCsv||best scores save to file success||fileName={}", fileName);
	return true;
}

std::string MyUtils::getCurTime()
{
	auto NowTime = std::chrono::system_clock::now();
	time_t ticks = std::chrono::system_clock::to_time_t(NowTime);
	struct tm* p_Time = new tm();
	localtime_s(p_Time, &ticks);

	char c_TimeStamp[64];
	memset(c_TimeStamp, 0, sizeof(c_TimeStamp));
	strftime(c_TimeStamp, sizeof(c_TimeStamp), "%Y-%m-%d %H:%M:%S", p_Time);
	DEBUGLOG("MyUtils::getCurTime||get curTime success||time={}", c_TimeStamp);
	return c_TimeStamp;
}

void MyUtils::init()
{
	std::ifstream ifs;
	// �ļ���Ϊ��
	if (MyUtils::isFileExistAndNotEmpty(BESTSCORESFILE))
	{
		std::string buf;
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
				tempString.push_back(buf.substr(start, pos - start));
				start = pos + 1;
			}
			tempUser.userName = tempString[0];
			tempUser.userScore = tempString[1];
			tempUser.curTime = tempString[2];
			MyUtils::bestScores.push_back(tempUser);
		}
	}
	// �ļ�Ϊ��
	else
	{
		MyUtils::bestScores.clear();
	}
}

void MyUtils::updateBestScore(const BestScoreUser& user)
{
	std::vector<BestScoreUser>::iterator it = MyUtils::bestScores.begin();
	for (; it != MyUtils::bestScores.end(); it++)
	{
		// �ҵ�����λ����
		if (user.userScore < it->userScore) {
			break;
		}
	}
	MyUtils::bestScores.insert(it, user);
	if (MyUtils::bestScores.size() > MAXBESTSCORES) {
		MyUtils::bestScores.pop_back();
	}
}
