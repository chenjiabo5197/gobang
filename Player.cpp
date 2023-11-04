#include "Player.h"

void Player::init(Chess* chess)
{
	this->chess = chess;
	INFOLOG("Player::init||init player success");
}

void Player::go(int x, int y)
{
	// 等待棋手有效落子
	MOUSEMSG msg;
	ChessPos pos;
	while (1)
	{
		if (chess->clickBoard(msg.x, msg.y, &pos))
		{
			INFOLOG("Player::go||valid mouse click chessboard||x={}||y={}", msg.x, msg.y);
			// 落子
			mciSendString("play res/chess_down.mp3", 0, 0, 0);
			chess->chessDown(&pos, CHESS_BLACK);
			DEBUGLOG("Player::go||Player chess down success");
			break;
		}
		else
		{
			INFOLOG("Player::go||invalid mouse click chessboard||no space to put chess||x={}||y={}", msg.x, msg.y);
		}
	}
}
