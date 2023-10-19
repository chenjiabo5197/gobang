#include "Chess.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

Chess::Chess(int chessBoardSize, int marginX, int marginY, float chessSize)
{
	this->chessBoardSize = chessBoardSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;
	playerFlag = CHESS_BLACK;

	// 初始化棋盘，棋盘每个位置都为0，表示空白
	for (int i = 0; i < chessBoardSize; i++)
	{
		std::vector<int> row;
		for (int j = 0; j < chessBoardSize; j++)
		{
			row.push_back(0);
		}
		chessMap.push_back(row);
	}
}

void Chess::init()
{
	initgraph(897, 895);
	loadimage(0, "res/棋盘2.jpg");

	mciSendString("play res/start.wav", 0, 0, 0);  //需要修改字符集为多字符集

	loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
	loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);

	for (int i = 0; i < chessMap.size(); i++)
	{
		for (int j = 0; j < chessMap[i].size(); j++)
		{
			chessMap[i][j] = 0;
		}
	}

	playerFlag = true;
}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	return false;
}

void Chess::chessDown(ChessPos* pos, chess_kind_type kind)
{
}

int Chess::getChessBoardSize()
{
	return 0;
}

int Chess::getChessData(ChessPos* pos)
{
	return 0;
}

int Chess::getChessData(int row, int col)
{
	return 0;
}

bool Chess::checkOver()
{
	return false;
}
