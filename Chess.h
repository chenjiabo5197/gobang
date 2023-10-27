#pragma once
#include <graphics.h>
#include <vector>
#include "logger.h"
#include <conio.h>


typedef enum {
	CHESS_WHITE = -1,   // �׷�
	CHESS_BLACK = 1		// �ڷ�
} chess_kind_type;

// �����ʱ�����̸��ӵ���Χ�ĸ��ǣ���־��ӡ�����⣬��ʱ�ȷ���
typedef enum {   
	LEFTTOP = 1,           //����     
	RIGHTTOP = 2,          //����
	LEFTBOTTOM = 3,        //����
	RIGHTBOTTOM = 4        //����
} chess_coordinate;

struct ChessPos {   // ����λ��
	int row;
	int col;
};

class Chess
{
public:
	Chess(int chessBoardWidth, int chessBoardHeight, int chessBoardSize, int marginX, int marginY, float chessSize);

	// ���̵ĳ�ʼ�����������̵�ͼƬ��Դ����ʼ�����̵��������
	void init();

	// �ж���ָ������(x,y)λ�ã��Ƿ���Ч���
	// �������Ч���������Ч�����λ��(x,y)���浽����pos��
	bool clickBoard(int x, int y, ChessPos* pos);

	// �����̵�ָ��λ��pos������kind
	void chessDown(ChessPos* pos, chess_kind_type kind);

	// ��ȡ���̴�С��13,15,19��
	int getChessBoardSize();

	// ��ȡָ��λ���Ǻ���/����/�հ�  �հ�0������-1������1
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	// �ж�����Ƿ����
	bool checkOver();

	// �Զ���ͼ����Ⱦ��easyxͼ�οⲻ֧�ֱ���͸����png��ʽͼƬ����͸��������ȾΪ��ɫ
	void putImagePNG(int x, int y, IMAGE* picture);

	// �ж�ʤ��
	bool checkWin();

	// ��ȡ���̵Ŀ�͸߳ߴ�
	int getChessBoardWidth();
	int getChessBoardHieght();

private:
	// ���̳ߴ�
	int chessBoardSize;

	// ���̱�Ե�������̵ľ���
	int margin_x;  // 49  
	int margin_y;  // 49

	float chessSize; // ���̷����С

	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;

	// ���浱ǰ���̺�����������հ�0������-1������1
	std::vector<std::vector<int>> chessMap;

	// ��ʾ���巽��true���巽��false ���巽
	bool playerFlag;

	// ���������λ��
	ChessPos lastPos;

	// �������̵���������
	void updateChessMap(ChessPos* pos);

	// ����ͼƬ�ĳߴ磬��ֹ��������̵��������ܰ���ʱ����Ϊ���嵼������Խ��
	int chessBoardWidth;
	int chessBoardHeight;

};

