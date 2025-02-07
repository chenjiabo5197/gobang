/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: This is chessboard.h file
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "chess.h"
#include "sdl_ttf.h"
#include "common.h"
#include "global.h"
#include "my_utils.h"
#include "logger.h"
#include "config.h"
#include "sdl_window.h"

class Chessboard
{
private:
	// 棋子
    Chess* m_white_chess;   			// 棋盘上落子
	Chess* m_white_current_chess;		// 棋盘上最后一步落子
    Chess* m_black_chess;
	Chess* m_black_current_chess;

	// 棋盘渲染的参数
    int m_render_r;
    int m_render_g;
    int m_render_b;
    int m_render_alpha;

	// 棋盘渲染的字体
	SDLTTF* m_chessboard_ttf;

	// 储存当前棋盘大小，当前默认为15
	int m_chessboard_size;

    // 储存当前棋盘和棋子情况，空白0，白子1，黑子2
	std::vector<std::vector<int>> m_chess_map;

    // 棋盘左上角坐标
    int m_origin_x, m_origin_y;

    // 棋盘中每个格子大小
    int m_lattice_size;

	// 棋盘边界的结构体
	ChessBoardBoundary* m_chessboard_boundary;

	// 最近的落子位置，不分黑白棋
	ChessPos* m_last_chess_pos;

	// 最近落子位置，区分黑白棋，目的是为了分别渲染最近一次落子位置
	ChessPos* m_black_current_chess_pos;
	ChessPos* m_white_current_chess_pos;

	// 当前的下棋方
	player_flag_type m_player_flag;

	// 储存当前棋盘上所有棋子数据
	std::vector<ChessData> m_chessboard_data;

	// 初始化/重置当前黑白棋位置参数
	void initCurrentChessPos();

	// 根据悔棋，更新black_current_chess_pos和white_current_chess_pos
	void updateCurrentChessPos();

	// 渲染文字到指定位置
	void renderText(const std::string& texture_text, const int& x, const int& y, const float& multiple);
public:
    // 传入棋盘左上角的坐标origin_x，origin_y，棋盘中单个格子大小，每种棋子原素材的缩放倍数，默认不缩放
    Chessboard(const Config& config);

    ~Chessboard();

	// 初始化，传入渲染所需参数
	void init();

	// 重置棋盘数据
	void initChessMap();

    // 渲染棋盘初始界面
    bool renderPlayChessInterface();

    // 渲染圆形，传入圆心坐标和圆的半径
    void renderCircle(const int& x, const int& y, const int& radius);

	// 判断在指定坐标(x,y)位置，是否有效点击
	// 如果是有效点击，返回true，把有效点击的位置(x,y)保存到参数pos中，否则返回false
	bool clickBoard(const int& x, const int& y, ChessPos* pos);

	// 在棋盘的x行、y列，落子kind
	void chessDown(const ChessPos& chessPos, const chess_kind_type& kind);

    // 棋盘渲染当前对局，渲染棋盘，渲染当前已下的棋子
    void render();

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
	void setPlayerFlagType(const player_flag_type& type);

	player_flag_type getPlayerFlagType();

	// 获取棋盘中心的坐标
	int getCenterX();
	int getCenterY();

	// 设置悔棋渲染
	void setChessboardWithdraw();

	// 判断是否可以进行悔棋
	bool isCanWithdraw();
};
