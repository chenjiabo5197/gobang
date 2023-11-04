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

	// ��ʼ��������һ�κڰ���λ�õĲ���
	lastBlackPos.row = -1;
	lastWhitePos.row = -1;

	playerFlag = CHESS_BLACK;

	// ��ʼ�����̣�����ÿ��λ�ö�Ϊ0����ʾ�հ�
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
	mciSendString("play res/start.wav", 0, 0, 0);  //��Ҫ�޸��ַ���Ϊ���ַ���

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
* �ȼ�����λ�ø�����4���㣨���̵Ľ��紦�����ӳ���λ�ã���λ�ã�Ȼ���ټ�����λ�õ����ĸ���֮��ľ��룬
�����ĳ����ľ���С�ڡ���ֵ��������Ϊ�����������λ�á��������ֵ���� С�����Ӵ�С��һ�뼴�ɡ���������ȡ����С��0.4��
*/
bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	// ������������ϵ��У�����(����������ȥ���̱�Ե�ٳ�ÿ�������С)
	int row = (x - margin_x) / chessSize;  // ����
	int col = (y - margin_y) / chessSize;  // ����

	// easyx������ԭ�������Ͻǣ�y�����£�x������
	// ���������ķ������Ͻ�����
	int leftTopPosX = margin_x + chessSize * row;
	int leftTopPosY = margin_y + chessSize * col;

	// ���������ķ������Ͻ�����
	int rightTopPosX = margin_x + chessSize * (row + 1);
	int rightTopPosY = margin_y + chessSize * col;

	// ���������ķ������½�����
	int leftBottomPosX = margin_x + chessSize * row;
	int leftBottomPosY = margin_y + chessSize * (col + 1);

	// ���������ķ������½�����
	int rightBottomPosX = margin_x + chessSize * (row + 1);
	int rightBottomPosY = margin_y + chessSize * (col + 1);

	int offset = chessSize * 0.4;  // 20 �������ģ����������

	int len;
	bool selectPos = false;
	chess_coordinate flag_coord = COORD_DEFAULT;

	INFOLOG("Chess::clickBoard||margin_x={}||margin_y={}||row={}||col={}||leftTopPosX={}||leftTopPosY={}||rightTopPosX={}||rightTopPosY={}||leftBottomPosX={}||leftBottomPosY={}||rightBottomPosX={}||rightBottomPosY={}||offset={}",
		margin_x, margin_y, row, col, leftTopPosX, leftTopPosY, rightTopPosX, rightTopPosY, leftBottomPosX, leftBottomPosY, rightBottomPosX, rightBottomPosY, offset);

	do
	{
		// �����Ͻ�λ�õľ���
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

		// �����Ͻ�λ�õľ���
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

		// �����½�λ�õľ���
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

		// �����½�λ�õľ���
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

// �����������ͺ����������ʱ��������Ĭ��ֵֻ�ܳ����ں��������У��ں�������ĺ���ͷ���������Ĭ��ֵ
void Chess::chessDown(ChessPos* pos, chess_kind_type kind, bool isRecord)
{
	int x = margin_x + pos->row * chessSize - 0.5 * chessSize;
	int y = margin_y + pos->col * chessSize - 0.5 * chessSize;

	if (this->chessBoard.isUse)   // �ж��Ƿ�������ҳ��
	{
		if (kind == CHESS_WHITE)
		{
			if (this->lastWhitePos.row != -1)
			{
				putImagePNG(this->lastWhitePos.row, this->lastWhitePos.col, &this->chessWhite.pictureFile);   // ���·����µ�����ͼƬ
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
				putImagePNG(this->lastBlackPos.row, this->lastBlackPos.col, &this->chessBlack.pictureFile);   // ���·����µ�����ͼƬ
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
		// ������������
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
		if (!playerFlag)   // ����Ӯ�����Ӯ
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
			mciSendString("play res/ʧ��.mp3", 0, 0, 0);
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
	// ������ʼ��
	DWORD* dst = GetImageBuffer();   // GetImageBuffer()�������ڻ�ȡ��ͼ�豸���Դ�ָ�룬easyx����
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture);  // ��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth();  // ��ȡpicture��ȣ�easyx����
	int picture_height = picture->getheight();  // ��ȡpicture�߶ȣ�easyx����
	int graphWidth = getwidth();   // ��ȡ��ͼ���Ŀ�ȣ�easyx����
	int graphHeight = getheight();   // ��ȡ��ͼ���Ŀ�ȣ�easyx����
	int dstX = 0;   // ���Դ������ص�����

	// ʵ��͸����ͼ  ��ʽ Cp=��p*FP+(1-��p)*BP   ��Ҷ˹������е���ɫ�ĸ��ʼ���
	for (int ix = 0; ix < picture_height; ix++)
	{
		for (int iy = 0; iy < picture_width; iy++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}

bool Chess::checkWin()
{
	// ������б��б�ĸ�����ÿ����������ݵ�ǰ����������5�����ӣ���һ�ַ��Ͼ���Ӯ
	int row = lastPos.row;
	int col = lastPos.col;

	// ˮƽ����, ������ҷֱ�ƥ��4����
	for (int i = 0; i < 5; i++)
	{
		if (col - i >= 0 && col - i + 4 < chessBoardSize && chessMap[row][col - i] == chessMap[row][col - i + 1] && chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] && chessMap[row][col - i] == chessMap[row][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate horizontal direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	// ��ֱ���� 
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 && row - i + 4 < chessBoardSize && chessMap[row - i][col] == chessMap[row - i + 1][col] && chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] && chessMap[row - i][col] == chessMap[row - i + 4][col])
		{
			INFOLOG("Chess::checkWin||calculate vertical direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	// ��б����"/"
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 && col - i >= 0 && row - i + 4 < chessBoardSize && col - i + 4 < chessBoardSize && chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] && chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] && chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate left oblique direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	// ��б���� "\"
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
	// ��ʼ���ڰ����ϴ�λ��
	this->lastBlackPos.row = -1;
	this->lastWhitePos.row = -1;
	// �ж���������������
	if (this->chessBoardData.size() <= 1)
	{
		WARNLOG("Chess::playerWithDraw||chess nums small 1||can not withdraw");
		return;
	}

	// this->chessMap.clear();// ��ɾ��vector�Ŀռ�    // �������������
	for (int i = 0; i < chessBoardSize; i++)
	{
		for (int j = 0; j < chessBoardSize; j++)
		{
			chessMap[i][j] = 0;
		}
	}

	// ɾ������ͼƬ, ��Ҫ���¼���ͼƬ��easyx����ɾ��ͼƬ
	this->chessBoardData.pop_back();
	this->chessBoardData.pop_back();  //ɾ������Ԫ�أ���Ϊ���ѡ�����ʱ��AI������Ҳ��Ҫ������
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
		// ������������
		DEBUGLOG("Chess::chessDown111||CHESS_BLACK||x={}||y={}", it->pos.row, it->pos.col);
		ChessPos temp{ it->pos.row, it->pos.col };
		updateChessMap(&temp);
	}
}

void Chess::updateChessMap(ChessPos* pos)
{
	DEBUGLOG("Chess::updateChessMap||playerFlag={}||pos->row={}||pos->col={}", !playerFlag, pos->row, pos->col);
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? 1 : -1;  // ���浱ǰλ���Ǻ��廹�ǰ���
	playerFlag = !playerFlag;   // �������巽
}


