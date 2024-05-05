/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: 结算页面管理系统，负责结算页面的渲染
* Copyright (c) 2024, All rights reserved
=============================================*/
#pragma once

#include <SDL2/SDL.h>
#include "common.h"
#include "global.h"
#include "sdl_button.h"
#include "sdl_ttf.h"
#include "sdl_window.h"
#include "logger.h"
#include "config.h"

class SettlementManage
{
private:
    // 当前页面要渲染的按键数组
    SDLButton* m_settlement_buttons[2];

    // 数组长度
    int m_array_length;

    //当前页面所有按键的中心位置，按键以此为中心，纵向分布
    int m_buttons_x;
    int m_buttons_y;

    // 按键之间的间隔(上下间隔)
    int m_button_interval;

    // 要渲染字体的坐标(字体中心坐标)
    int m_font_x;
    int m_font_y;

    // 玩家赢结算页面的渲染
    SDLTTF* m_player_win_interface;
    // 玩家输结算页面的渲染
    SDLTTF* m_player_lose_interface;
    // 双人游戏黑方胜利结算页面的渲染
    SDLTTF* m_black_win_interface;
    // 双人游戏白方胜利结算页面的渲染
    SDLTTF* m_white_win_interface;

    // 渲染的种类
    interface_kind_type m_interface_render_type;
public:
    SettlementManage(const Config& config);
    ~SettlementManage();

    void startRender();

    // 初始化
    void init();
    
    // 处理事件
    void handleEvents(SDL_Event* event);

    // 设置渲染文字的中心坐标
    void setFontCoordinate(const int& x, const int& y);

    // 设置要渲染的界面种类
    void setInterfaceRenderType(const interface_kind_type& type);
};


