#pragma once
#include "AI.h"
#include "chess.h"
#include "player.h"
#include "logger.h"
#include "pictureDraw.h"
#include "globalVar.hpp"

class Management
{
public:
	Management(Player*, Player*, AI*, Chess*, PictureDraw*);
	void play();

private:
	Player* player1;
	Player* player2;
	AI* ai;
	Chess* chess;
	PictureDraw* pictureDraw;

	// 选择游戏方式
	void chooseGame();

	// 单人游戏
	void onePlayerGame();

	// 单人游戏初始化对象，AI，player，chess
	void onePlayerInit();

	// 游戏结束，根据全局变量决定展示的成功还是失败页面，并接受再来一局还是退出到主菜单
	bool isAgainGame();

	// 双人对战
	void twoPlayersGame();

	// 双人对战初始化对象，player1, player2, chess
	void twoPlayersInit();

	// 单个选手的下棋函数，传入棋手
	bool onePlayer(Player*);
};


