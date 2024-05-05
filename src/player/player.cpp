#include "player.h"

Player::Player(Chessboard* chessboard, const std::string& player_name, const chess_kind_type& chess_kind)
{
    m_chessboard = chessboard;
    m_player_name = player_name;
    m_chess_kind = chess_kind;
    m_chess_num =0;
    INFOLOG("Player construct success||player name={}||chess kind={}", m_player_name = player_name, (int)m_chess_kind);
}

Player::~Player()
{
    INFOLOG("~Player success!");
}

void Player::init()
{
	INFOLOG("Player::init||init player success");
}

void Player::resetPlayer()
{
	m_chess_num = 0;
	INFOLOG("Player::resetPlayer||reset player success||name={}", m_player_name);
}

void Player::addChessNum()
{
    INFOLOG("Player::addChessNum||player_name={}||chess_num={}", m_player_name, ++m_chess_num);
}

int Player::getChessNum()
{
    INFOLOG("Player::getChessNum||getChessNum||chess_num={}", m_chess_num);
	return m_chess_num;
}

// bool Player::go(int x, int y, chess_kind_type kind)
// {
// 	// 等待棋手有效落子
// 	ChessPos pos;
// 	if (this->chess->clickBoard(x, y, &pos))
// 	{
// 		INFOLOG("Player::go||valid mouse click chessboard||x={}||y={}", x, y);
// 		// 落子
// 		// mciSendString("play res/g_chess_down_sound.mp3", 0, 0, 0);
// 		this->chess->chessDown(&pos, kind);
// 		this->chessNum++;
// 		DEBUGLOG("Player::go||Player chess down success");
// 		return true;
// 	}
// 	else
// 	{
// 		INFOLOG("Player::go||invalid mouse click chessboard||no space to put chess||x={}||y={}", x, y);
// 		return false;
// 	}
// }