#pragma once
#include <graphics.h>
#include <vector>
#include "logger.h"
#include <conio.h>


typedef enum {
	CHESS_WHITE = -1,   // 白方
	CHESS_BLACK = 1		// 黑方
} chess_kind_type;

// 鼠标点击时在棋盘格子的周围四个角，日志打印有问题，暂时先放弃
typedef enum {   
	LEFTTOP = 1,           //左上     
	RIGHTTOP = 2,          //右上
	LEFTBOTTOM = 3,        //左下
	RIGHTBOTTOM = 4        //右下
} chess_coordinate;

struct ChessPos {   // 棋子位置
	int row;
	int col;
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

private:
	// 棋盘尺寸
	int chessBoardSize;

	// 棋盘边缘距离棋盘的距离
	int margin_x;  // 49  
	int margin_y;  // 49

	float chessSize; // 棋盘方格大小

	IMAGE chessBlackImg;
	IMAGE chessWhiteImg;

	// 储存当前棋盘和棋子情况，空白0，白子-1，黑子1
	std::vector<std::vector<int>> chessMap;

	// 表示下棋方，true黑棋方，false 白棋方
	bool playerFlag;

	// 最近的落子位置
	ChessPos lastPos;

	// 更新棋盘的棋子数据
	void updateChessMap(ChessPos* pos);

	// 棋盘图片的尺寸，防止点击到棋盘的其他功能按键时误以为下棋导致数组越界
	int chessBoardWidth;
	int chessBoardHeight;

};

