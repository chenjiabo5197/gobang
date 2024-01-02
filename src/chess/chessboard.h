#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "chess.h"
#include "../logger/logger.h"
#include "../utils/config.h"

typedef enum {
    CHESS_DEFAULT,  // 空白
	CHESS_WHITE,    // 白方
	CHESS_BLACK		// 黑方
} chess_kind_type;

// 鼠标点击时在棋盘格子的周围四个角
typedef enum {
	COORD_DEFAULT,		   // 初始位置
	LEFTTOP,           //左上     
	RIGHTTOP,          //右上
	LEFTBOTTOM,        //左下
	RIGHTBOTTOM       //右下
} chess_coordinate;

typedef enum {
	GAME_KIND_DEFAULT,
	ONE_PLAYER_GAME,    // 单人游戏
	TWO_PLAYERS_GAME,    // 双人游戏
	PLAYER_INTERNET    // 网络对战
} game_kind;

// 棋子位置
struct ChessPos {   
	int chess_row;
	int chess_col;
};

// 储存每一步棋子位置和棋子类型的结构体
struct ChessData {
	ChessPos* chessPos;
	chess_kind_type chess_type;
};

// 棋盘边界，储存棋盘的四个角坐标
struct ChessBoardBoundary {
	int left_top_x;
	int left_top_y;
	int right_top_x;
	int right_top_y;
	int left_bottom_x;
	int left_bottom_y;
	int right_bottom_x;
	int right_bottom_y;
};

class Chessboard
{
public:
    // 棋子
    Chess* white_chess;
    Chess* black_chess;

    // 传入棋盘左上角的坐标origin_x，origin_y，棋盘中单个格子大小，每种棋子原素材的缩放倍数，默认不缩放
    Chessboard(const Config& config);

    ~Chessboard();

	// 棋盘的初始化，加载棋盘的图片资源，初始化棋盘的相关数据
	void init();

    // 渲染棋盘初始界面
    bool renderPlayChessInterface(SDL_Renderer* gRenderer);

    // 渲染圆形，传入圆心坐标和圆的半径
    void renderCircle(SDL_Renderer* gRenderer, const int& x, const int& y, const int& radius);

	// 判断在指定坐标(x,y)位置，是否有效点击
	// 如果是有效点击，返回true，把有效点击的位置(x,y)保存到参数pos中，否则返回false
	bool clickBoard(const int& x, const int& y, ChessPos* pos);

	// 在棋盘的x行、y列，落子kind, isRecord表示是否要将棋子记录到chessBoardData中，默认需要记录，只有悔棋时下棋数据不用记录
	void chessDown(const ChessPos& chessPos, const chess_kind_type& kind, bool isRecord = true);

    // 棋盘渲染当前对局，渲染棋盘，渲染当前已下的棋子
    void render(SDL_Window * gWindow, SDL_Renderer* gRenderer);

	// 初始化棋盘边界结构体
	void initChessBoardBoundary();

	// 判断传过来的点击位置是否在棋盘上
	bool isClickOnChessBoard(const int& x, const int& y);

	// // 获取棋盘大小（13,15,19）
	// int getChessBoardSize();

	// // 获取指定位置是黑棋/白棋/空白  空白0，白子-1，黑子1
	// int getChessData(ChessPos* pos);
	// int getChessData(int row, int col);

	// // 判断棋局是否结束
	// bool checkOver();

	// // 自定义图形渲染，easyx图形库不支持背景透明的png格式图片，把透明背景渲染为黑色
	// void putImagePNG(int x, int y, IMAGE* picture);

	// // 判断胜负
	// bool checkWin();

	// // 悔棋,悔棋需要记录每一步的顺序和下棋位置
	// void playerWithDraw();

private:
    // 储存当前棋盘和棋子情况，空白0，白子1，黑子2
	std::vector<std::vector<int>> chessMap;

    // 棋盘左上角坐标
    int origin_x, origin_y;

    // 棋盘中每个格子大小
    int lattice_size;

	// 棋盘边界的结构体
	ChessBoardBoundary* chessboard_boundary;
};
