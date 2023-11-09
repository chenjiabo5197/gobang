#pragma once
#include "Chess.h"
#include "logger.h"
#include "myUtils.h"
#include <vector>

class Player
{
public:
	std::string playerName;

	chess_kind_type chessKind;

	void init(Chess* chess, std::string playerName, chess_kind_type chessKind = CHESS_BLACK);
	
	bool go(int x, int y, chess_kind_type kind = CHESS_BLACK);

	std::vector<BestScoreUser> bestScores;
private:
	Chess* chess;    // �������ݣ����ĸ���������
};

