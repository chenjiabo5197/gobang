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
	}
	ofs.close();
	INFOLOG("MyUtils::saveVectorToCsv||best scores save to file success||fileName={}", fileName);
	return true;
}

std::string MyUtils::getCurTime()
{
	auto curTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	// ת��Ϊ�ַ���
	char timeStr[100];

	std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&curTime));

	DEBUGLOG("MyUtils::getCurTime||get curTime success||time={}", timeStr);

	return timeStr;
}
