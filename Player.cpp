#include "Player.h"

void Player::init(Chess* chess)
{
	this->chess = chess;
	INFOLOG("Player::init||init player success");
}

bool Player::go(int x, int y)
{
	// 等待棋手有效落子
	ChessPos pos;
	if (this->chess->clickBoard(x, y, &pos))
	{
		INFOLOG("Player::go||valid mouse click chessboard||x={}||y={}", x, y);
		// 落子
		mciSendString("play res/chess_down.mp3", 0, 0, 0);
		this->chess->chessDown(&pos, CHESS_BLACK);
		DEBUGLOG("Player::go||Player chess down success");
		return true;
	}
	else
	{
		INFOLOG("Player::go||invalid mouse click chessboard||no space to put chess||x={}||y={}", x, y);
		return false;
	}
}
