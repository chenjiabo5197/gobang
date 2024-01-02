#include "player.h"

Player::Player(Chessboard* chessboard)
{
    this->chessboard = chessboard;
    INFOLOG("Player construct success!");
}

Player::~Player()
{
    INFOLOG("~Player success!");
}