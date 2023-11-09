#pragma once
#include <graphics.h>
#include <vector>
#include "logger.h"
#include <conio.h>
#include "pictureDraw.h"

typedef enum {
	RESULT_FLAG_DEFAULT = 0,
	PLAYER_WIN = 1,      // 玩家赢 
	PLAYER_LOSE = 2,      // 玩家输
	RESULT_DRAW = 3,      // 平局
	BLACK_WIN = 4,      // 黑方胜
	WHITE_WIN = 5      // 白方胜
}result_flag;

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
	GAME_KIND_DEFAULT = 0,
	ONE_PLAYER_GAME = 1,    // 单人游戏
	TWO_PLAYERS_GAME = 2,    // 双人游戏
	PLAYER_INTERNET = 3    // 网络对战
} game_kind;

struct ChessPos {   // 棋子位置
	int row;
	int col;
};

struct ChessData {    // 储存每一步棋子位置和棋子类型的结构体
	ChessPos pos;        // 储存棋盘坐标的x，y数据
	ChessPos imagePos;   // 储存每个棋子坐标的x，y数据
	chess_kind_type chessType;
};

extern bool exitGame;  // 是否退出游戏，true退出，false不退出
extern result_flag resultFlag;  // 游戏结果

class Chess
{
public:
	// 当前棋盘的不同对战类型，单人、双人、网络
	game_kind gameKind;

	Chess(int chessBoardSize, int marginX, int marginY, float chessSize, PictureDraw*);

	// 棋盘的初始化，加载棋盘的图片资源，初始化棋盘的相关数据
	void init();

	// 判断在指定坐标(x,y)位置，是否有效点击
	// 如果是有效点击，把有效点击的位置(x,y)保存到参数pos中
	bool clickBoard(int x, int y, ChessPos* pos);

	// 在棋盘的指定位置pos，落子kind, isRecord表示是否要将棋子记录到chessBoardData中，默认需要记录，只有悔棋时下棋数据不用记录
	void chessDown(ChessPos* pos, chess_kind_type kind, bool isRecord = true);

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

	// 悔棋,悔棋需要记录每一步的顺序和下棋位置
	void playerWithDraw();

private: 

	PictureDraw* pictureDraw;

	// 储存每一步的下棋数据
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

	// 上一次的黑棋和白棋方位置,为了将最近的黑棋和白棋方图片换成一般图片使用
	ChessPos lastBlackPos, lastWhitePos;

	// 更新棋盘的棋子数据
	void updateChessMap(ChessPos* pos);

};

