/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: 结算页面管理系统，负责结算页面的渲染
* Copyright (c) 2024, All rights reserved
=============================================*/
#pragma once

#include <SDL2/SDL.h>
#include "../common/common.h"
#include "../common/global.h"
#include "../render/sdl_button.h"
#include "../render/sdl_ttf.h"
#include "../render/sdl_window.h"
#include "../logger/logger.h"
#include "../utils/config.h"

class SettlementManage
{
private:
    // 当前页面要渲染的按键数组
    SDLButton* settlement_buttons[2];

    // 数组长度
    int array_length;

    //当前页面所有按键的中心位置，按键以此为中心，纵向分布
    int buttons_x;
    int buttons_y;

    // 按键之间的间隔(上下间隔)
    int button_interval;

    // 要渲染字体的坐标(字体中心坐标)
    int font_x;
    int font_y;

    // 玩家赢结算页面的渲染
    SDLTTF* player_win_interface;
    // 玩家输结算页面的渲染
    SDLTTF* player_lose_interface;
    // 双人游戏黑方胜利结算页面的渲染
    SDLTTF* black_win_interface;
    // 双人游戏白方胜利结算页面的渲染
    SDLTTF* white_win_interface;

    // 渲染的种类
    interface_kind_type interface_render_type;
public:
    SettlementManage(const Config& config);
    ~SettlementManage();

    void startRender();

    // 初始化
    void init();
    
    // 处理事件
    void handleEvents(SDL_Event* event);

    // 设置渲染文字的中心坐标
    void set_font_coordinate(const int& x, const int& y);

    // 设置要渲染的界面种类
    void set_interface_render_type(const interface_kind_type& type);
};


