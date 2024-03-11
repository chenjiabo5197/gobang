/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: 顶层管理，负责调用各个子管理系统
* Copyright (c) 2023, All rights reserved
=============================================*/

#pragma once

#include <math.h>
#include <SDL2/SDL_ttf.h>
#include "main_menu_manage.h"
#include "select_play_manage.h"
#include "playchess_manage.h"
#include "settlement_manage.h"
#include "../common/common.h"
#include "../logger/logger.h"
#include "../utils/config.h"
#include "../render/sdl_texture.h"
#include "../render/sdl_button.h"
#include "../render/sdl_ttf.h"
#include "../render/sdl_window.h"
#include "../chess/chessboard.h"
#include "../player/machine.h"
#include "../player/player.h"

class TopManage
{
private:
    // 主窗口
    SDLWindow* main_window;

    // 各个子页面管理
    MainMenuManage* main_menu_manage;
    SelectPlayManage* select_play_manage;
    PlaychessManage* playchess_manage;
    SettlementManage* settlement_manage;

    // 行楷字体
    std::string art_ttf_path;
    int art_ttf_ptsize;
    TTF_Font* art_ttf;

    // 普通字体
    TTF_Font* normal_font;
    std::string normal_ttf_path;
    int normal_ttf_ptsize;

    // 渲染的种类
    interface_kind_type render_type;
public:
    TopManage(const Config& config);
    ~TopManage();

    void start();

    // 初始化渲染
    bool initRender();

    // 加载资源
    bool loadResource();

    // 关闭渲染，释放资源
    void closeRender();

    // 设置渲染界面类型
    void setRendererType(const interface_kind_type& render_type);
};


