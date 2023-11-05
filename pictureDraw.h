#pragma once
#include <graphics.h>
#include <string>
#include "logger.h"
#include <vector>
#include <algorithm>

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

typedef enum {
	MENU_DEFAULT = 0,
	MAIN_MENU = 1,   // ѡ��ʼ��Ϸ���˳���Ϸ
	PLAYER_NUM_MENU = 2,   // ѡ������Ϸ��˫����Ϸ�������ս
	CHESSBOARD_MENU = 3,  // ��ս�в˵������̺��˳���Ϸ������
	WIN_MENU = 4,   // ������ϷӮ�ˣ�ѡ������һ�ֻ����˳���Ϸ
	LOSE_MENU = 5,   // ������Ϸ���ˣ�ѡ������һ�ֻ����˳���Ϸ
	DRAW_MENU = 6     // ƽ�֣������������ӻ�δ����ʤ��
} menu_kind_type;

class PictureDraw
{
public:
	PictureDraw(int chessBoardSize, int marginX, int marginY, float chessSize);

	LoadPicture chessBlackPicture, chessWhitePicture, chessBoardPicture, exitGamePicture, withDrawPicture, againGamePicture, onePlayerPicture, bestScoresPicture, drawGamePicture, 
		twoPlayersPicture, playerInternetPicture, startGamePicture, winGamePicture, loseGamePicture, curBlackPicture, curWhitePicture, backgroudPicture, backwardMenu;

	// �Ƿ���Ч���  x,yΪ�������λ��, pictureΪҪ�����ͼƬ
	bool isValidClick(int x, int y, LoadPicture picture);

	// ͳһ�Ļ�ͼ����������menu_kind_type���;���������������ͼ
	void drawGraph(menu_kind_type kind);
private:
	// ���������ͼƬ����������б��е�ͼƬ�������Ҫ�е������
	std::vector<std::string> clickSound;
	
	// �����һ�ε�graph���ݣ���ֹ���´ε�����Ӱ��
	void clearLastGraph();

};

