#pragma once
#include "Chess.h"
#include "logger.h"

class Player
{
public:
	void init(Chess* chess);
	// ����һ��boolֵ��true�˳���Ϸ��false������Ϸ
	bool go();
private:
	Chess* chess;    // �������ݣ����ĸ���������
};

