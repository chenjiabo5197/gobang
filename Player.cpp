#include "Player.h"

void Player::init(Chess* chess)
{
	this->chess = chess;
	INFOLOG("Player::init||init player success");
}

void Player::go()
{
	// 等待棋手有效落子
	MOUSEMSG msg;
	ChessPos pos;
	while (1)
	{
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			DEBUGLOG("Player::go||mouse click||x={}||y={}", msg.x, msg.y);
			if (msg.x >= 0 && msg.x <= chess->getChessBoardWidth() && msg.y >= 0 && msg.y <= chess->getChessBoardHieght())
			{
				if (chess->clickBoard(msg.x, msg.y, &pos))
				{
					INFOLOG("Player::go||valid mouse click chessboard||x={}||y={}", msg.x, msg.y);
					// 落子
					chess->chessDown(&pos, CHESS_BLACK);
					DEBUGLOG("Player::go||Player chess down success");
					break;
				}
				else
				{
					INFOLOG("Player::go||invalid mouse click chessboard||no space to put chess||x={}||y={}", msg.x, msg.y);
					// break;
				}
			}
			else
			{
				INFOLOG("Player::go||select other function||x={}||y={}", msg.x, msg.y);
				// 可以选择悔棋或者退出游戏
				if (chess->isValidClick(msg.x, msg.y, chess->exitGame))
				{
					GlobalVar::instance()->set_value("exit_game", true);
					break;
				}
				if (chess->isValidClick(msg.x, msg.y, chess->withDraw))
				{
					chess->playerWithDraw();
				}
			}
		}
	}
}
