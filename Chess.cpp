#include "Chess.h"
#include <mmsystem.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")

Chess::Chess(int chessBoardSize, int marginX, int marginY, float chessSize)
{
	this->chessBoardSize = chessBoardSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;
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
	initgraph(897, 895);
	loadimage(0, "res/����2.jpg");

	mciSendString("play res/start.wav", 0, 0, 0);  //��Ҫ�޸��ַ���Ϊ���ַ���

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
	int flag_coord = 0;

	INFOLOG("Chess::clickBoard||margin_x={}||margin_y={}||row={}||col={}||leftTopPosX={}||leftTopPosY={}||rightTopPosX={}||rightTopPosY={}||leftBottomPosX={}||leftBottomPosY={}||rightBottomPosX={}||rightBottomPosY={}||offset={}",
		margin_x, margin_y, row, col, leftTopPosX, leftTopPosY, rightTopPosX, rightTopPosY, leftBottomPosX, leftBottomPosY, rightBottomPosX, rightBottomPosY, offset);

	do
	{
		// �����Ͻ�λ�õľ���
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset)
		{
			flag_coord = 1;
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
			flag_coord = 2;
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
			flag_coord = 3;
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
			flag_coord = 4;
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
	INFOLOG("Chess::clickBoard||flag_coord={}||len={}||pos.row={}||pos.col={}", flag_coord, len, pos->row, pos->col);

	return selectPos;
}

void Chess::chessDown(ChessPos* pos, chess_kind_type kind)
{
	mciSendString("play res/down7.WAV", 0, 0, 0);
	int x = margin_x + pos->row * chessSize - 0.5 * chessSize;
	int y = margin_y + pos->col * chessSize - 0.5 * chessSize;

	if (kind == CHESS_WHITE)
	{
		putImagePNG(x, y, &chessWhiteImg);
		DEBUGLOG("Chess::chessDown||CHESS_WHITE||x={}||y={}", x, y);
	}
	else
	{
		putImagePNG(x, y, &chessBlackImg);
		DEBUGLOG("Chess::chessDown||CHESS_BLACK||x={}||y={}", x, y);
	}
	// ������������
	updateChessMap(pos);
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

void Chess::updateChessMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? 1 : -1;  // ���浱ǰλ���Ǻ��廹�ǰ���
	playerFlag = !playerFlag;   // �������巽
	DEBUGLOG("Chess::updateChessMap||playerFlag={}||pos->row={}||pos->col={}", !playerFlag, pos->row, pos->col);
}
