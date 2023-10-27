#pragma once
#include "Chess.h"
#include "logger.h"

class Player
{
public:
	void init(Chess* chess);
	// 返回一个bool值，true退出游戏，false继续游戏
	bool go();
private:
	Chess* chess;    // 棋盘数据，对哪个棋盘下棋
};

