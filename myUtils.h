#pragma once
#include <string>
#include <fstream>
#include "logger.h"
#include <sstream>
#include <chrono>

// �������а����ݵĽṹ��
struct BestScoreUser
{
	std::string userName;
	std::string userScore;
	std::string curTime;
};

class MyUtils
{
	// �����ļ������жϸ��ļ������Ҳ�Ϊ��
	static bool isFileExistAndNotEmpty(std::string fileName);

	// ��std::vector<std::string>��ʽ��vector���浽fileName�У������ش�����
	static bool saveVectorToCsv(std::vector<BestScoreUser> v, std::string fileName);

	// ��ȡ��ǰʱ�䲢����string��ʽ"%Y��%m��%d��%Hʱ%M��%S��"
	static std::string getCurTime();
};

