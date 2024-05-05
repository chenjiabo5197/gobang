/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: 选择游戏方式管理系统，负责选择游戏方式页面渲染和选择先后手窗口
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include <SDL2/SDL.h>
#include "global.h"
#include "chess.h"
#include "sdl_ttf.h"
#include "sdl_button.h"
#include "sdl_window.h"
#include "logger.h"
#include "config.h"

class SelectPlayManage
{
private:
    // 选择先后手的窗口(也是选择棋子颜色，黑色先手)
    SDLWindow* m_select_chess_color_window;
    
    // 当前页面要渲染的按键数组
    SDLButton* m_select_play_buttons[4];

    // 数组长度
    int m_array_length;

    //当前页面所有按键的中心位置，按键以此为中心，纵向分布
    int m_buttons_x;
    int m_buttons_y;

    // 按键之间的间隔(上下间隔)
    int m_button_interval;

    // 当前游戏类型，单人、双人、网络
    game_kind_type m_current_game_type;

    // 选择先后手的棋子
    Chess* m_white_color_chess;		
    Chess* m_black_color_chess;

    // 棋盘渲染的字体
    SDLTTF* m_select_chess_color_ttf;

    // 单人游戏选择先后手
    void singlePlaySelectChess();

    // 处理选择先后手窗口事件,返回白棋和黑棋与鼠标的位置关系枚举
    std::pair<sdl_button_sprite, sdl_button_sprite> handleWindowEvents(SDL_Event* event, bool mouse_focus);

    // 渲染选择先后手窗口,返回值为true表示已经选择好了先后手，选择窗口关闭
    bool selectChessRender(std::pair<sdl_button_sprite, sdl_button_sprite> mouse_event);

    // 渲染文字到指定位置(选择先后手窗口专用)
	void renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple);
public:
    SelectPlayManage(const Config& config);
    ~SelectPlayManage();

    void startRender();

    // 初始化
    void init();

    // 处理事件
    void handleEvents(SDL_Event* event);
};



