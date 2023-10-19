#pragma once
#include "AI.h"
#include "Chess.h"
#include "Player.h"

class Management
{
public:
	Management(Player*, AI*, Chess*);
	void play();

private:
	Player* player;
	AI* ai;
	Chess* chess;
};


