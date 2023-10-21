#pragma once
#include "Chess.h"
#include "logger.h"

class Player
{
public:
	void init(Chess* chess);
	void go();
private:
	Chess* chess;    //∆Â≈Ã
};

