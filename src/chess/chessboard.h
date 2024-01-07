#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "chess.h"
#include "../common.h"
#include "../utils/myUtils.h"
#include "../logger/logger.h"
#include "../utils/config.h"

class Chessboard
{
public:
    // 棋子
    Chess* white_chess;
    Chess* black_chess;

    // 传入棋盘左上角的坐标origin_x，origin_y，棋盘中单个格子大小，每种棋子原素材的缩放倍数，默认不缩放
    Chessboard(const Config& config);

    ~Chessboard();

	// 重置棋盘数据
	void initChessMap();

    // 渲染棋盘初始界面
    bool renderPlayChessInterface(SDL_Renderer* gRenderer);

    // 渲染圆形，传入圆心坐标和圆的半径
    void renderCircle(SDL_Renderer* gRenderer, const int& x, const int& y, const int& radius);

	// 判断在指定坐标(x,y)位置，是否有效点击
	// 如果是有效点击，返回true，把有效点击的位置(x,y)保存到参数pos中，否则返回false
	bool clickBoard(const int& x, const int& y, ChessPos* pos);

	// 在棋盘的x行、y列，落子kind
	void chessDown(const ChessPos& chessPos, const chess_kind_type& kind);

    // 棋盘渲染当前对局，渲染棋盘，渲染当前已下的棋子
    void render(SDL_Window * gWindow, SDL_Renderer* gRenderer);

	// 初始化棋盘边界结构体
	void initChessBoardBoundary();

	// 判断传过来的点击位置是否在棋盘上
	bool isClickOnChessBoard(const int& x, const int& y);

	// // 获取棋盘大小（13,15,19）
	int getChessBoardSize();

	// 获取指定位置是黑棋/白棋/空白  空白0，白子1，黑子2
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	// 判断棋局是否结束
	bool checkOver();

	// 判断胜负
	bool checkWin();

	// 设置当前的下棋方
	void set_player_flag_type(const player_flag_type& type);

	player_flag_type get_player_flag_type();

	// 获取棋盘中心的坐标
	int get_chessboard_center_x();
	int get_chessboard_center_y();

	// 设置悔棋渲染
	void set_chessboard_withdraw();

	// 判断是否可以进行悔棋
	bool is_can_withdraw();

private:
	// 储存当前棋盘大小，当前默认为15
	int chessboard_size;

    // 储存当前棋盘和棋子情况，空白0，白子1，黑子2
	std::vector<std::vector<int>> chessMap;

    // 棋盘左上角坐标
    int origin_x, origin_y;

    // 棋盘中每个格子大小
    int lattice_size;

	// 棋盘边界的结构体
	ChessBoardBoundary* chessboard_boundary;

	// 最近的落子位置
	ChessPos* last_chess_pos;

	// 当前的下棋方
	player_flag_type player_flag;

	// 储存当前棋盘上所有棋子数据
	std::vector<ChessData> chessboard_data;
};
