/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-14
* Description: This is chess_data_board.h file
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <iomanip>
#include "chess.h"
#include "chess_data_board_iterm.h"
#include "sdl_timer.h"
#include "sdl_ttf.h"
#include "common.h"
#include "global.h"
#include "my_utils.h"
#include "logger.h"
#include "config.h"
#include "sdl_window.h"

class ChessDataBoard
{
private:
    // 数据板数组
    ChessDataBoardIterm* m_data_board_arr[4];

    // 单人游戏先后手
    chess_color_type m_current_chess_sequence;
    chess_color_type m_last_chess_sequence;

	// 棋盘渲染的字体
    SDLTTF* m_data_board_ttf;

    // 数据板上棋子标志
    Chess* m_symbol_white_chess;		
    Chess* m_symbol_black_chess;

    // 数据板中心坐标
    int m_data_board_x;
    int m_data_board_y;

    // 总计时器，显示当前对局进行的时间
    SDLTimer* m_top_timer;

    // 渲染文字到指定位置
	void renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple);

public:
    ChessDataBoard(const Config& config);
    ~ChessDataBoard();

    // 初始化，传入渲染所需参数
	void init();

	// 初始化数据板信息
	void initDataBoard(const chess_color_type& type);

    // 重置数据板信息
    void resetDataBoard();

    // 数据版渲染
    void render(const player_flag_type& type);

    // 更新比分信息
    void updateScoreInfo(const result_info_type& type);

    // 开始单人游戏，设置一些数据
    void startSingleGame(const chess_color_type& type);

    // 获取当前单人游戏先后手顺序
    chess_color_type getCurrentChessSequence();
};



