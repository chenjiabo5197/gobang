#pragma once
#include "Chess.h"
#include "logger.h"
#include "myUtils.h"
#include <vector>

class Player
{
public:

	// ������Ϸ�����������
	int chessNum;

	std::string playerName;

	chess_kind_type chessKind;

	void init(Chess* chess, std::string playerName, chess_kind_type chessKind = CHESS_BLACK);

	// ÿ����Ϸ����ؿ���Ϸ��Ҫ���õĲ���
	void resetPlayer();
	
	bool go(int x, int y, chess_kind_type kind = CHESS_BLACK);

private:
	Chess* chess;    // �������ݣ����ĸ���������
};

