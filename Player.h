#pragma once
#include "Chess.h"
#include "logger.h"
#include "GlobalVar.hpp"

class Player
{
public:
	void init(Chess* chess);
	
	void go();
private:
	Chess* chess;    // �������ݣ����ĸ���������
};

