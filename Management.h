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
	Management(PictureDraw*);
	void play();

private:
	Player* player;
	AI* ai;
	Chess* chess;
	PictureDraw* pictureDraw;

	// 选择游戏方式
	void chooseGame();

	// 单人游戏
	void onePlayerGame();

	// 单人游戏初始化对象，AI，player，chess
	void onePlayerInit();
};


