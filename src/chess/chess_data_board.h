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
#include "../render/sdl_timer.h"
#include "../render/sdl_ttf.h"
#include "../common.h"
#include "../utils/myUtils.h"
#include "../logger/logger.h"
#include "../utils/config.h"
#include "../render/sdl_window.h"

class ChessDataBoard
{
private:
    // 主窗口
    SDLWindow* chess_data_window;

    // 数据板数组
    ChessDataBoardIterm* data_board_arr[2];

    // 当前比赛类型，单人/双人/网络
    game_kind_type current_game_type;

    // 单人游戏先后手
    chess_color_type current_chess_sequence;
    chess_color_type last_chess_sequence;

	// 棋盘渲染的字体
	TTF_Font* normal_ttf;
    TTF_Font* art_ttf;
    SDLTTF* data_board_ttf;

    // 数据板上棋子标志
    Chess* symbol_white_chess;		
    Chess* symbol_black_chess;

    // 数据板中心坐标
    int data_board_x;
    int data_board_y;

    // 渲染文字到指定位置
	void renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple);

    // 比分信息
    std::map<std::string, std::map<std::string, std::string>> score_info;

    // 总计时器，显示当前对局进行的时间
    SDLTimer* top_timer;
public:
    ChessDataBoard(const Config& config);
    ~ChessDataBoard();

    // 初始化，传入渲染所需参数
	void init(SDLWindow* chess_data_window, TTF_Font* normal_font, TTF_Font* art_font);

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



