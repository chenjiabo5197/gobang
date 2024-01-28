/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: 主菜单管理，负责主菜单的渲染
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include <SDL2/SDL.h>
#include "../render/sdl_button.h"
#include "../render/sdl_window.h"
#include "../logger/logger.h"
#include "../utils/config.h"

class MainMenuManage
{
private:
    // 主窗口
    SDLWindow* menu_main_window;
    
    // 当前页面要渲染的按键数组
    SDLButton* main_menu_buttons[3];

    // 数组长度
    int array_length;

    // 当前页面所有按键的中心位置，按键以此为中心，纵向分布
    int buttons_x;
    int buttons_y;

    // 按键之间的间隔(上下间隔)
    int button_interval;
public:
    MainMenuManage(const Config& config);
    ~MainMenuManage();

    void startRender();

    // 初始化
    void init(SDLWindow* sdl_window);

    // 处理事件
    void handleEvents(SDL_Event* event);
};
