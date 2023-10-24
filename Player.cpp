#include "Player.h"

void Player::init(Chess* chess)
{
	this->chess = chess;
	INFOLOG("Player::init||init player success");
}

void Player::go()
{
	// �ȴ�������Ч����
	MOUSEMSG msg;
	ChessPos pos;
	try
	{
		while (1)
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				DEBUGLOG("Player::go||mouse click||x={}||y={}", msg.x, msg.y);
			}

			if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos))
			{
				INFOLOG("Player::go||valid mouse click||x={}||y={}", msg.x, msg.y);
				break;
			}
		}

		// ����
		chess->chessDown(&pos, CHESS_BLACK);
		DEBUGLOG("Player::go||Player chess down success");
	}
	catch (const std::exception& e)
	{
		CRITICALLOG("Player::go||exception={}", e.what());
	}
}
