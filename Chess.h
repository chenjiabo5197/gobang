#pragma once
#include <graphics.h>
#include <vector>
#include "logger.h"
#include <conio.h>
#include "GlobalVar.hpp"


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
	MENU_DEFAULT = 0,
	MAIN_MENU = 1,   // ѡ��ʼ��Ϸ���˳���Ϸ
	PLAYER_NUM_MENU = 2,   // ѡ������Ϸ��˫����Ϸ�������ս
	CHESSBOARD_MENU = 3,  // ��ս�в˵������̺��˳���Ϸ������
	WIN_MENU = 4,   // Ӯ�ˣ�ѡ������һ�ֻ����˳���Ϸ
	LOSE_MENU = 5   // ���ˣ�ѡ������һ�ֻ����˳���Ϸ
} menu_kind_type;

// ÿ�ż��ص�ͼƬ�ṹ��
struct LoadPicture {
	IMAGE pictureFile;
	std::string name;   // ͼƬ���֣���ӡ��־ʹ��
	bool isUse;		// ��ͼƬ�Ƿ�����չʾ��
	int x;     // ͼƬ���õ����Ͻ�x����
	int y;     // ͼƬ���õ����Ͻ�y����
	int width;     // ͼƬ�Ŀ��
	int height;     // ͼƬ�ĸ߶�
};

struct ChessPos {   // ����λ��
	int row;
	int col;
};

struct ChessData {    // ����ÿһ������λ�ú��������͵Ľṹ��
	ChessPos pos;        // �������������x��y����
	ChessPos imagePos;   // ����ÿ�����������x��y����
	chess_kind_type chessType;
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

	//// �Ƿ��˳���Ϸ  x,yΪ�������λ��
	//bool isExitGame(int x, int y);

	//// �Ƿ����  x,yΪ�������λ��
	//bool isWithDraw(int x, int y);

	//// �Ƿ�����һ��  x,yΪ�������λ��
	//bool isAgainGame(int x, int y);

	LoadPicture chessBlack, chessWhite, chessBoard, exitGame, withDraw, againGame, onePlayer, twoPlayers, playerInternet, startGame, winGame, loseGame;

	// �Ƿ���Ч���  x,yΪ�������λ��, pictureΪҪ�����ͼƬ
	bool isValidClick(int x, int y, LoadPicture picture);

	// ����,������Ҫ��¼ÿһ����˳�������λ��
	void playerWithDraw();

private:
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

	// �������̵���������
	void updateChessMap(ChessPos* pos);

	// ͳһ�Ļ�ͼ����������menu_kind_type���;���������������ͼ
	void drawGraph(menu_kind_type kind);

	// ����ͼƬ�ĳߴ磬��ֹ��������̵��������ܰ���ʱ����Ϊ���嵼������Խ��
	int chessBoardWidth;
	int chessBoardHeight;

	// �����һ�ε�graph���ݣ���ֹ���´ε�����Ӱ��
	void clearLastGraph();

};

