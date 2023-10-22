#pragma once
#include "Chess.h"
#include "logger.h"

class Player
{
public:
	void init(Chess* chess);
	void go();
private:
	Chess* chess;    // 棋盘数据，对哪个棋盘下棋
};

