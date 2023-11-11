#pragma once
#include <string>
#include <fstream>
#include "logger.h"
#include <sstream>
#include <chrono>

#define BESTSCORESFILE "bestScoresFile.csv"
#define MAXBESTSCORES 10

// �������а����ݵĽṹ��
struct BestScoreUser
{
	std::string userName;
	std::string userScore;
	std::string curTime;
};

class MyUtils
{
public:

	// �����ļ������жϸ��ļ������Ҳ�Ϊ��
	static bool isFileExistAndNotEmpty(std::string fileName);

	// ��std::vector<std::string>��ʽ��vector���浽BESTSCORESFILE�У������ش�����
	static bool saveVectorToCsv(std::vector<BestScoreUser> v);

	// ��ȡ��ǰʱ�䲢����string��ʽ"%Y��%m��%d��%Hʱ%M��%S��"
	static std::string getCurTime();

	// ���а��ʼ��
	static std::vector<BestScoreUser> initBestScores();

	// �����а����
	static void updateBestScore(std::vector<BestScoreUser> & bestScores, const BestScoreUser & user);

	// ͨ�����������齨�ṹ��
	static BestScoreUser getIBestScoreUser(std::string userName, int chessNum);
};

