#pragma once
#include "../logger/logger.h"
#include "../chess/chessboard.h"

class Player
{
private:
    // 棋盘数据，对哪个棋盘下棋
    Chessboard* chessboard;    

    // 单人游戏玩家棋子数量
	int chessNum;

	std::string playerName;

	chess_kind_type chessKind;
public:
    Player(Chessboard* chessboard);
    ~Player();

	void init(Chess* chess, std::string playerName, chess_kind_type chessKind = CHESS_BLACK);

	// 每次游戏完后，重开游戏需要设置的参数
	void resetPlayer();
	
	bool go(int x, int y, chess_kind_type kind = CHESS_BLACK);
};


