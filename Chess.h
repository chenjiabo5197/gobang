#pragma once

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
};

