#pragma once
#include <graphics.h>
#include <vector>


typedef enum {
	CHESS_WHITE = -1,   // �׷�
	CHESS_BLACK = 1		// �ڷ�
} chess_kind_type;

struct ChessPos {   // ����λ��
	int row;
	int col;
};

class Chess
{
public:
	Chess(int chessBoardSize, int marginX, int marginY, float chessSize);

	// ���̵ĳ�ʼ�����������̵�ͼƬ��Դ����ʼ�����̵��������
	void init();

	// �ж���ָ������(x,y)λ�ã��Ƿ���Ч���
	// �������Ч���������Ч�����λ��(x,y)���浽����pos��
	bool clickBoard(int x, int y, ChessPos* pos);

	// �����̵�ָ��λ��pos������kind
	void chessDown(ChessPos* pos, chess_kind_type kind);

	// ��ȡ���̴�С��13,15,19��
	int getChessBoardSize();

	// ��ȡָ��λ���Ǻ���/����/�հ�
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	// �ж�����Ƿ����
	bool checkOver();

private:
	// ���̳ߴ�
	int chessBoardSize;
	int margin_x;  // 49
	int margin_y;  // 49
	float chessSize; // ���̷����С

	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;

	// ���浱ǰ���̺�����������հ�0������-1������1
	std::vector<std::vector<int>> chessMap;

	// ��ʾ���巽��true���巽��false ���巽
	bool playerFlag;

};

