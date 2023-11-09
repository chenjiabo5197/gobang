#pragma once
#include <graphics.h>
#include <vector>
#include "logger.h"
#include <conio.h>
#include "pictureDraw.h"

typedef enum {
	RESULT_FLAG_DEFAULT = 0,
	PLAYER_WIN = 1,      // ���Ӯ 
	PLAYER_LOSE = 2,      // �����
	RESULT_DRAW = 3,      // ƽ��
	BLACK_WIN = 4,      // �ڷ�ʤ
	WHITE_WIN = 5      // �׷�ʤ
}result_flag;

typedef enum {
	CHESS_WHITE = -1,   // �׷�
	CHESS_BLACK = 1		// �ڷ�
} chess_kind_type;

// �����ʱ�����̸��ӵ���Χ�ĸ���
typedef enum {
	COORD_DEFAULT = 0,		   // ��ʼλ��
	LEFTTOP = 1,           //����     
	RIGHTTOP = 2,          //����
	LEFTBOTTOM = 3,        //����
	RIGHTBOTTOM = 4        //����
} chess_coordinate;

typedef enum {
	GAME_KIND_DEFAULT = 0,
	ONE_PLAYER_GAME = 1,    // ������Ϸ
	TWO_PLAYERS_GAME = 2,    // ˫����Ϸ
	PLAYER_INTERNET = 3    // �����ս
} game_kind;

struct ChessPos {   // ����λ��
	int row;
	int col;
};

struct ChessData {    // ����ÿһ������λ�ú��������͵Ľṹ��
	ChessPos pos;        // �������������x��y����
	ChessPos imagePos;   // ����ÿ�����������x��y����
	chess_kind_type chessType;
};

extern bool exitGame;  // �Ƿ��˳���Ϸ��true�˳���false���˳�
extern result_flag resultFlag;  // ��Ϸ���

class Chess
{
public:
	// ��ǰ���̵Ĳ�ͬ��ս���ͣ����ˡ�˫�ˡ�����
	game_kind gameKind;

	Chess(int chessBoardSize, int marginX, int marginY, float chessSize, PictureDraw*);

	// ���̵ĳ�ʼ�����������̵�ͼƬ��Դ����ʼ�����̵��������
	void init();

	// �ж���ָ������(x,y)λ�ã��Ƿ���Ч���
	// �������Ч���������Ч�����λ��(x,y)���浽����pos��
	bool clickBoard(int x, int y, ChessPos* pos);

	// �����̵�ָ��λ��pos������kind, isRecord��ʾ�Ƿ�Ҫ�����Ӽ�¼��chessBoardData�У�Ĭ����Ҫ��¼��ֻ�л���ʱ�������ݲ��ü�¼
	void chessDown(ChessPos* pos, chess_kind_type kind, bool isRecord = true);

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

	// ����,������Ҫ��¼ÿһ����˳�������λ��
	void playerWithDraw();

private: 

	PictureDraw* pictureDraw;

	// ����ÿһ������������
	std::vector<ChessData> chessBoardData;

	// ���̳ߴ�
	int chessBoardSize;

	// ���̱�Ե�������̵ľ���
	int margin_x;  // 49  
	int margin_y;  // 49

	float chessSize; // ���̷����С

	// ���浱ǰ���̺�����������հ�0������-1������1
	std::vector<std::vector<int>> chessMap;

	// ��ʾ���巽��true���巽��false ���巽
	bool playerFlag;

	// ���������λ��
	ChessPos lastPos;

	// ��һ�εĺ���Ͱ��巽λ��,Ϊ�˽�����ĺ���Ͱ��巽ͼƬ����һ��ͼƬʹ��
	ChessPos lastBlackPos, lastWhitePos;

	// �������̵���������
	void updateChessMap(ChessPos* pos);

};

