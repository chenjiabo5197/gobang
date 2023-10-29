#pragma once
#include <graphics.h>
#include <vector>
#include "logger.h"
#include <conio.h>
#include "GlobalVar.hpp"


typedef enum {
	CHESS_WHITE = -1,   // 白方
	CHESS_BLACK = 1		// 黑方
} chess_kind_type;

// 鼠标点击时在棋盘格子的周围四个角
typedef enum {
	COORD_DEFAULT = 0,		   // 初始位置
	LEFTTOP = 1,           //左上     
	RIGHTTOP = 2,          //右上
	LEFTBOTTOM = 3,        //左下
	RIGHTBOTTOM = 4        //右下
} chess_coordinate;

typedef enum {
	MENU_DEFAULT = 0,
	MAIN_MENU = 1,   // 选择开始游戏和退出游戏
	PLAYER_NUM_MENU = 2,   // 选择单人游戏，双人游戏，网络对战
	CHESSBOARD_MENU = 3,  // 对战中菜单，棋盘和退出游戏，悔棋
	WIN_MENU = 4,   // 赢了，选择再来一局或者退出游戏
	LOSE_MENU = 5   // 输了，选择再来一局或者退出游戏
} menu_kind_type;

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

struct ChessPos {   // 棋子位置
	int row;
	int col;
};

struct ChessData {    // 储存每一步棋子位置和棋子类型的结构体
	ChessPos pos;        // 储存棋盘坐标的x，y数据
	ChessPos imagePos;   // 储存每个棋子坐标的x，y数据
	chess_kind_type chessType;
};

class Chess
{
public:
	Chess(int chessBoardWidth, int chessBoardHeight, int chessBoardSize, int marginX, int marginY, float chessSize);

	// 棋盘的初始化，加载棋盘的图片资源，初始化棋盘的相关数据
	void init();

	// 判断在指定坐标(x,y)位置，是否有效点击
	// 如果是有效点击，把有效点击的位置(x,y)保存到参数pos中
	bool clickBoard(int x, int y, ChessPos* pos);

	// 在棋盘的指定位置pos，落子kind
	void chessDown(ChessPos* pos, chess_kind_type kind);

	// 获取棋盘大小（13,15,19）
	int getChessBoardSize();

	// 获取指定位置是黑棋/白棋/空白  空白0，白子-1，黑子1
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	// 判断棋局是否结束
	bool checkOver();

	// 自定义图形渲染，easyx图形库不支持背景透明的png格式图片，把透明背景渲染为黑色
	void putImagePNG(int x, int y, IMAGE* picture);

	// 判断胜负
	bool checkWin();

	// 获取棋盘的宽和高尺寸
	int getChessBoardWidth();
	int getChessBoardHieght();

	//// 是否退出游戏  x,y为鼠标点击的位置
	//bool isExitGame(int x, int y);

	//// 是否悔棋  x,y为鼠标点击的位置
	//bool isWithDraw(int x, int y);

	//// 是否再来一局  x,y为鼠标点击的位置
	//bool isAgainGame(int x, int y);

	LoadPicture chessBlack, chessWhite, chessBoard, exitGame, withDraw, againGame, onePlayer, twoPlayers, playerInternet, startGame, winGame, loseGame;

	// 是否有效点击  x,y为鼠标点击的位置, picture为要点击的图片
	bool isValidClick(int x, int y, LoadPicture picture);

	// 悔棋,悔棋需要记录每一步的顺序和下棋位置
	void playerWithDraw();

private:
	std::vector<ChessData> chessBoardData;

	// 棋盘尺寸
	int chessBoardSize;

	// 棋盘边缘距离棋盘的距离
	int margin_x;  // 49  
	int margin_y;  // 49

	float chessSize; // 棋盘方格大小

	// 储存当前棋盘和棋子情况，空白0，白子-1，黑子1
	std::vector<std::vector<int>> chessMap;

	// 表示下棋方，true黑棋方，false 白棋方
	bool playerFlag;

	// 最近的落子位置
	ChessPos lastPos;

	// 更新棋盘的棋子数据
	void updateChessMap(ChessPos* pos);

	// 统一的绘图函数，根据menu_kind_type类型决定绘制哪种类型图
	void drawGraph(menu_kind_type kind);

	// 棋盘图片的尺寸，防止点击到棋盘的其他功能按键时误以为下棋导致数组越界
	int chessBoardWidth;
	int chessBoardHeight;

	// 清楚上一次的graph数据，防止对下次点击造成影响
	void clearLastGraph();

};

