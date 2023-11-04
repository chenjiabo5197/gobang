#include "Chess.h"
#include <mmsystem.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")

Chess::Chess(int chessBoardSize, int marginX, int marginY, float chessSize, PictureDraw* pictureDraw)
{
	this->chessBoardSize = chessBoardSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;
	this->pictureDraw = pictureDraw;

	// 初始化储存上一次黑白棋位置的参数
	lastBlackPos.row = -1;
	lastWhitePos.row = -1;

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
	INFOLOG("Chess::Chess||construct chess success||chessBoardSize={}||marginX={}||marginY={}||chessSize={}", 
		chessBoardSize, marginX, marginY, chessSize);
}

void Chess::init()
{
	mciSendString("play res/start.wav", 0, 0, 0);  //需要修改字符集为多字符集

	for (int i = 0; i < chessMap.size(); i++)
	{
		for (int j = 0; j < chessMap[i].size(); j++)
		{
			chessMap[i][j] = 0;
		}
	}

	playerFlag = true;
	INFOLOG("Chess::init||init chess success||graph.width={}||graph.height={}", 897, 895);
}

/*
* 先计算点击位置附近的4个点（棋盘的交界处即棋子出现位置）的位置，然后再计算点击位置到这四个点之间的距离，
如果离某个点的距离小于“阈值”，就认为这个点是落子位置。这个“阈值”， 小于棋子大小的一半即可。我们这里取棋格大小的0.4倍
*/
bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	// 鼠标点击在棋盘上的行，列数(用鼠标坐标减去棋盘边缘再除每个方格大小)
	int row = (x - margin_x) / chessSize;  // 列数
	int col = (y - margin_y) / chessSize;  // 行数

	// easyx的坐标原点在左上角，y轴向下，x轴向右
	// 鼠标点击坐标的方格左上角坐标
	int leftTopPosX = margin_x + chessSize * row;
	int leftTopPosY = margin_y + chessSize * col;

	// 鼠标点击坐标的方格右上角坐标
	int rightTopPosX = margin_x + chessSize * (row + 1);
	int rightTopPosY = margin_y + chessSize * col;

	// 鼠标点击坐标的方格左下角坐标
	int leftBottomPosX = margin_x + chessSize * row;
	int leftBottomPosY = margin_y + chessSize * (col + 1);

	// 鼠标点击坐标的方格右下角坐标
	int rightBottomPosX = margin_x + chessSize * (row + 1);
	int rightBottomPosY = margin_y + chessSize * (col + 1);

	int offset = chessSize * 0.4;  // 20 鼠标点击的模糊距离上限

	int len;
	bool selectPos = false;
	chess_coordinate flag_coord = COORD_DEFAULT;

	INFOLOG("Chess::clickBoard||margin_x={}||margin_y={}||row={}||col={}||leftTopPosX={}||leftTopPosY={}||rightTopPosX={}||rightTopPosY={}||leftBottomPosX={}||leftBottomPosY={}||rightBottomPosX={}||rightBottomPosY={}||offset={}",
		margin_x, margin_y, row, col, leftTopPosX, leftTopPosY, rightTopPosX, rightTopPosY, leftBottomPosX, leftBottomPosY, rightBottomPosX, rightBottomPosY, offset);

	do
	{
		// 距左上角位置的距离
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset)
		{
			flag_coord = LEFTTOP;
			pos->row = row;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("Chess::clickBoard||leftTop||len={}", len);

		// 距右上角位置的距离
		len = sqrt((x - rightTopPosX) * (x - rightTopPosX) + (y - rightTopPosY) * (y - rightTopPosY));
		if (len < offset)
		{
			flag_coord = RIGHTTOP;
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("Chess::clickBoard||rightTop||len={}", len);

		// 距左下角位置的距离
		len = sqrt((x - leftBottomPosX) * (x - leftBottomPosX) + (y - leftBottomPosY) * (y - leftBottomPosY));
		if (len < offset)
		{
			flag_coord = LEFTBOTTOM;
			pos->row = row;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("Chess::clickBoard||leftBottom||len={}", len);

		// 距右下角位置的距离
		len = sqrt((x - rightBottomPosX) * (x - rightBottomPosX) + (y - rightBottomPosY) * (y - rightBottomPosY));
		if (len < offset)
		{
			flag_coord = RIGHTBOTTOM;
			pos->row = row + 1;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("Chess::clickBoard||rightBottom||len={}", len);

	} while (0);
	INFOLOG("Chess::clickBoard||flag_coord={}||len={}||pos.row={}||pos.col={}", (int)flag_coord, len, pos->row, pos->col);

	return selectPos;
}

// 当函数声明和函数定义分离时，参数的默认值只能出现在函数声明中，在函数定义的函数头中无需添加默认值
void Chess::chessDown(ChessPos* pos, chess_kind_type kind, bool isRecord)
{
	int x = margin_x + pos->row * chessSize - 0.5 * chessSize;
	int y = margin_y + pos->col * chessSize - 0.5 * chessSize;

	if (this->chessBoard.isUse)   // 判断是否在下棋页面
	{
		if (kind == CHESS_WHITE)
		{
			if (this->lastWhitePos.row != -1)
			{
				putImagePNG(this->lastWhitePos.row, this->lastWhitePos.col, &this->chessWhite.pictureFile);   // 更新非最新的棋子图片
			}
			putImagePNG(x, y, &this->curWhite.pictureFile); 
			if (isRecord)
			{
				ChessData temp2{ pos->row, pos->col, x, y, CHESS_WHITE };
				this->chessBoardData.push_back(temp2);
			}
			ChessPos temp{ x, y };
			this->lastWhitePos = temp;
			DEBUGLOG("Chess::chessDown||CHESS_WHITE||x={}||y={}||row={}||col={}", x, y, pos->row, pos->col);
		}
		else
		{
			if (this->lastBlackPos.row != -1)
			{
				putImagePNG(this->lastBlackPos.row, this->lastBlackPos.col, &this->chessBlack.pictureFile);   // 更新非最新的棋子图片
			}
			putImagePNG(x, y, &this->curBlack.pictureFile);
			if (isRecord)
			{
				ChessData temp2{ pos->row, pos->col, x, y, CHESS_BLACK };
				this->chessBoardData.push_back(temp2);
			}
			ChessPos temp{ x, y };
			this->lastBlackPos = temp;
			DEBUGLOG("Chess::chessDown||CHESS_BLACK||x={}||y={}||row={}||col={}", x, y, pos->row, pos->col);
		}
		// 更新棋盘数据
		updateChessMap(pos);
	}
}

int Chess::getChessBoardSize()
{
	return chessBoardSize;
}

int Chess::getChessData(ChessPos* pos)
{
	return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
	return chessMap[row][col];
}

bool Chess::checkOver()
{
	if (checkWin())
	{
		Sleep(1500);
		if (!playerFlag)   // 黑棋赢，玩家赢
		{
			INFOLOG("Chess::checkOver||black win");
			mciSendString("play res/clap.mp3", 0, 0, 0);
			GlobalVar::instance()->setResultFlag(PLAYER_WIN);
			this->chessBoardData.clear();
			while (1)
			{
				MOUSEMSG msg = GetMouseMsg();
				if (msg.uMsg == WM_LBUTTONDOWN)
				{
					DEBUGLOG("Chess::checkOver||mouse click||x={}||y={}", msg.x, msg.y);
					if (this->isValidClick(msg.x, msg.y, this->exitGame))
					{

						GlobalVar::instance()->set_value("exit_game", true);
						return false;
					}
					if (this->isValidClick(msg.x, msg.y, this->againGame))
					{
						return true;
					}
				}
			}
		}
		else
		{
			INFOLOG("Chess::checkOver||white win");
			mciSendString("play res/失败.mp3", 0, 0, 0);
			drawGraph(LOSE_MENU);
			this->chessBoardData.clear();
			while (1)
			{
				MOUSEMSG msg = GetMouseMsg();
				if (msg.uMsg == WM_LBUTTONDOWN)
				{
					DEBUGLOG("Chess::checkOver||mouse click||x={}||y={}", msg.x, msg.y);
					if (this->isValidClick(msg.x, msg.y, this->exitGame))
					{

						GlobalVar::instance()->set_value("exit_game", true);
						return false;
					}
					if (this->isValidClick(msg.x, msg.y, this->againGame))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void Chess::putImagePNG(int x, int y, IMAGE* picture)
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();   // GetImageBuffer()函数用于获取绘图设备的显存指针，easyx函数
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture);  // 获取picture的显存指针
	int picture_width = picture->getwidth();  // 获取picture宽度，easyx函数
	int picture_height = picture->getheight();  // 获取picture高度，easyx函数
	int graphWidth = getwidth();   // 获取绘图区的宽度，easyx函数
	int graphHeight = getheight();   // 获取绘图区的宽度，easyx函数
	int dstX = 0;   // 在显存里像素的坐标

	// 实现透明贴图  公式 Cp=αp*FP+(1-αp)*BP   贝叶斯定理进行点颜色的概率计算
	for (int ix = 0; ix < picture_height; ix++)
	{
		for (int iy = 0; iy < picture_width; iy++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}

bool Chess::checkWin()
{
	// 横竖左斜右斜四个方向，每种情况都根据当前落子向后遍历5个棋子，有一种符合就算赢
	int row = lastPos.row;
	int col = lastPos.col;

	// 水平方向, 向左和右分别匹配4个子
	for (int i = 0; i < 5; i++)
	{
		if (col - i >= 0 && col - i + 4 < chessBoardSize && chessMap[row][col - i] == chessMap[row][col - i + 1] && chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] && chessMap[row][col - i] == chessMap[row][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate horizontal direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	// 竖直方向 
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 && row - i + 4 < chessBoardSize && chessMap[row - i][col] == chessMap[row - i + 1][col] && chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] && chessMap[row - i][col] == chessMap[row - i + 4][col])
		{
			INFOLOG("Chess::checkWin||calculate vertical direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	// 左斜方向"/"
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 && col - i >= 0 && row - i + 4 < chessBoardSize && col - i + 4 < chessBoardSize && chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] && chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] && chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate left oblique direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	// 右斜方向 "\"
	for (int i = 0; i < 5; i++)
	{
		if (row + i < chessBoardSize && row + i - 4 >= 0 && col - i >= 0 && col - i + 4 < chessBoardSize && chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] && chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] && chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate right oblique direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	return false;

}

void Chess::playerWithDraw()
{
	// 初始化黑白棋上次位置
	this->lastBlackPos.row = -1;
	this->lastWhitePos.row = -1;
	// 判断棋盘上棋子数量
	if (this->chessBoardData.size() <= 1)
	{
		WARNLOG("Chess::playerWithDraw||chess nums small 1||can not withdraw");
		return;
	}

	// this->chessMap.clear();// 会删除vector的空间    // 先清空棋盘数据
	for (int i = 0; i < chessBoardSize; i++)
	{
		for (int j = 0; j < chessBoardSize; j++)
		{
			chessMap[i][j] = 0;
		}
	}

	// 删除棋子图片, 需要重新加载图片，easyx不能删除图片
	this->chessBoardData.pop_back();
	this->chessBoardData.pop_back();  //删除两个元素，因为玩家选择悔棋时，AI的棋子也需要撤销掉
	//drawGraph(CHESSBOARD_MENU);
	for (std::vector<ChessData>::iterator it = this->chessBoardData.begin(); it != this->chessBoardData.end(); it++)
	{
		DEBUGLOG("Chess::chessDown222||CHESS_BLACK||x={}||y={}", it->pos.row, it->pos.col);
		if (it->chessType == CHESS_WHITE)
		{
			//putImagePNG(it->imagePos.row, it->imagePos.col, &this->chessWhite.pictureFile);
			ChessPos temp{ it->pos.row, it->pos.col };
			//this->chessDown(&temp, CHESS_WHITE, false);
			this->playerFlag = false;
		}
		else
		{
			//putImagePNG(it->imagePos.row, it->imagePos.col, &this->chessBlack.pictureFile);
			ChessPos temp{ it->pos.row, it->pos.col };
			//this->chessDown(&temp, CHESS_BLACK, false);
			this->playerFlag = true;
		}
		// 更新棋盘数据
		DEBUGLOG("Chess::chessDown111||CHESS_BLACK||x={}||y={}", it->pos.row, it->pos.col);
		ChessPos temp{ it->pos.row, it->pos.col };
		updateChessMap(&temp);
	}
}

void Chess::updateChessMap(ChessPos* pos)
{
	DEBUGLOG("Chess::updateChessMap||playerFlag={}||pos->row={}||pos->col={}", !playerFlag, pos->row, pos->col);
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? 1 : -1;  // 储存当前位置是黑棋还是白棋
	playerFlag = !playerFlag;   // 更换下棋方
}


