/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-14
* Description: This is chess_data_board.h file
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include <SDL2/SDL.h>
#include "chess.h"
#include "../render/sdl_ttf.h"
#include "../common.h"
#include "../utils/myUtils.h"
#include "../logger/logger.h"
#include "../utils/config.h"

class ChessDataBoard
{
private:
    // 渲染的窗口
    SDL_Window* global_window;
    SDL_Renderer* global_renderer;
	// 棋盘渲染的普通字体
	TTF_Font* global_ttf;
    SDLTTF* data_board_ttf;

    // 数据板上棋子标志
    Chess* symbol_white_chess;		
    Chess* symbol_black_chess;

    // 数据板中心坐标
    int data_board_x;
    int data_board_y;

    // 渲染文字到指定位置
	void renderText(const std::string& texture_text, const int& x, const int& y, const float& multiple);
public:
    ChessDataBoard(const Config& config);
    ~ChessDataBoard();

    // 初始化，传入渲染所需参数
	void init(SDL_Window * global_window, SDL_Renderer* global_renderer, TTF_Font* global_font);

	// 重置数据版信息
	void initDataBoard();

    // 数据版渲染
    void render();
};



