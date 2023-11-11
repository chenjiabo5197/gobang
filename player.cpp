#include "Player.h"

void Player::init(Chess* chess, std::string playerName, chess_kind_type chessKind)
{
	this->chess = chess;
	this->playerName = playerName;
	this->chessKind = chessKind;
	this->chessNum = 0;
	INFOLOG("Player::init||init player success||player name={}||chess kind={}", playerName, (int)chessKind);
}

void Player::resetPlayer()
{
	this->chessNum = 0;
	INFOLOG("Player::resetPlayer||reset player success");
}

bool Player::go(int x, int y, chess_kind_type kind)
{
	// �ȴ�������Ч����
	ChessPos pos;
	if (this->chess->clickBoard(x, y, &pos))
	{
		INFOLOG("Player::go||valid mouse click chessboard||x={}||y={}", x, y);
		// ����
		mciSendString("play res/chess_down.mp3", 0, 0, 0);
		this->chess->chessDown(&pos, kind);
		this->chessNum++;
		DEBUGLOG("Player::go||Player chess down success");
		return true;
	}
	else
	{
		INFOLOG("Player::go||invalid mouse click chessboard||no space to put chess||x={}||y={}", x, y);
		return false;
	}
}
