#pragma once
#include "Chess.h"
#include "logger.h"
#include "myUtils.h"
#include <vector>

class Player
{
public:

	// 单人游戏玩家棋子数量
	int chessNum;

	std::string playerName;

	chess_kind_type chessKind;

	void init(Chess* chess, std::string playerName, chess_kind_type chessKind = CHESS_BLACK);

	// 每次游戏完后，重开游戏需要设置的参数
	void resetPlayer();
	
	bool go(int x, int y, chess_kind_type kind = CHESS_BLACK);

private:
	Chess* chess;    // 棋盘数据，对哪个棋盘下棋
};

