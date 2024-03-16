/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-22
* Description: This is chess_data_board.h file
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include <iostream>
#include "chess.h"
#include "../common/common.h"
#include "../common/global.h"
#include "../render/sdl_ttf.h"
#include "../logger/logger.h"
#include "../render/sdl_window.h"
#include "../utils/myUtils.h"

class ChessDataBoardIterm
{
private:
    // 名字
    std::string iterm_name;
    // 要渲染的文字坐标
    int iterm_name_x;
    int iterm_name_y;
    // 文字缩放倍数
    float iterm_name_multiple;

    // 分数
    int iterm_score;
    // 要渲染的文字坐标
    int iterm_score_x;
    int iterm_score_y;
    // 文字缩放倍数
    float iterm_score_multiple;

    // 棋盘渲染的字体
    SDLTTF* data_board_iterm_ttf;
public:
    // 只传入姓名坐标，比分坐标可以根据姓名坐标推算出
    ChessDataBoardIterm(const std::string& name, const int& name_x, const int& name_y, const float& name_multiple, const float& score_multiple);
    ~ChessDataBoardIterm();

    // 初始化，传入渲染所需参数
	void init();

    // 渲染
    void render(SDL_Color color);

    // 分数+1
    void addScore();

    // 渲染文字到指定位置
	void renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple);

    // 交换两个ChessDataBoardIterm的名字、分数的坐标
    void swapField(ChessDataBoardIterm& iterm);
};


