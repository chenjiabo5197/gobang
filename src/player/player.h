#pragma once
#include "logger.h"
#include "chessboard.h"

class Player
{
private:
    // 棋盘数据，对哪个棋盘下棋
    Chessboard* m_chessboard;    

    // 当前棋子数量，用于制作单人玩家游戏排行榜
	int m_chess_num;

	// 当前的玩家名，主要用于双人游戏
	std::string m_player_name;

	// 当前玩家的棋子类型，用于双人游戏
	chess_kind_type m_chess_kind;
public:
    Player(Chessboard* chessboard, const std::string& player_name, const chess_kind_type& chess_kind);
    ~Player();

	void init();

	// 每次游戏完后，重开游戏需要设置的参数
	void resetPlayer();

	// 下棋成功，棋子数+1
	void addChessNum();

	// 获取当前对局下棋数量
	int getChessNum();
	
	friend int playerChessDown(void* data);
};


