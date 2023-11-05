#pragma once
#include "Chess.h"
#include "logger.h"
#include "globalVar.hpp"

class Player
{
public:
	void init(Chess* chess);
	
	bool go(int x, int y);
private:
	Chess* chess;    // �������ݣ����ĸ���������
};

