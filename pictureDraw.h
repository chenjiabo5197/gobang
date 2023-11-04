#pragma once
#include <graphics.h>
#include <string>
#include "logger.h"

// 每张加载的图片结构体
struct LoadPicture {
	IMAGE pictureFile;
	std::string name;   // 图片名字，打印日志使用
	bool isUse;		// 该图片是否正在展示中
	int x;     // 图片放置的左上角x坐标
	int y;     // 图片放置的左上角y坐标
	int width;     // 图片的宽度
	int height;     // 图片的高度
};

typedef enum {
	MENU_DEFAULT = 0,
	MAIN_MENU = 1,   // 选择开始游戏和退出游戏
	PLAYER_NUM_MENU = 2,   // 选择单人游戏，双人游戏，网络对战
	CHESSBOARD_MENU = 3,  // 对战中菜单，棋盘和退出游戏，悔棋
	WIN_MENU = 4,   // 赢了，选择再来一局或者退出游戏
	LOSE_MENU = 5,   // 输了，选择再来一局或者退出游戏
	DRAW_MENU = 6     // 平局，棋盘落满棋子还未决出胜负
} menu_kind_type;

class PictureDraw
{
public:
	PictureDraw(int chessBoardSize, int marginX, int marginY, float chessSize);

	LoadPicture chessBlackPicture, chessWhitePicture, chessBoardPicture, exitGamePicture, withDrawPicture, againGamePicture, onePlayerPicture, bestScoresPicture, 
		twoPlayersPicture, playerInternetPicture, startGamePicture, winGamePicture, loseGamePicture, curBlackPicture, curWhitePicture, backgroudPicture, backwardMenu;

	// 是否有效点击  x,y为鼠标点击的位置, picture为要点击的图片
	bool isValidClick(int x, int y, LoadPicture picture);

	// 统一的绘图函数，根据menu_kind_type类型决定绘制哪种类型图
	void drawGraph(menu_kind_type kind);
private:
	// 清除上一次的graph数据，防止对下次点击造成影响
	void clearLastGraph();

};

