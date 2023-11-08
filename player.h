#pragma once
#include "Chess.h"
#include "logger.h"
#include "globalVar.hpp"

class Player
{
public:
	std::string playerName;

	chess_kind_type chessKind;

	void init(Chess* chess, std::string playerName, chess_kind_type chessKind = CHESS_BLACK);
	
	bool go(int x, int y, chess_kind_type kind = CHESS_BLACK);
private:
	Chess* chess;    // 棋盘数据，对哪个棋盘下棋
};

