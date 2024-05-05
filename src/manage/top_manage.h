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
#include "common.h"
#include "logger.h"
#include "config.h"
#include "sdl_texture.h"
#include "sdl_button.h"
#include "sdl_ttf.h"
#include "sdl_window.h"
#include "chessboard.h"
#include "../player/machine.h"
#include "../player/player.h"

class TopManage
{
private:

    // 各个子页面管理
    MainMenuManage* m_main_menu_manage;
    SelectPlayManage* m_select_play_manage;
    PlaychessManage* m_playchess_manage;
    SettlementManage* m_settlement_manage;

    // 行楷字体
    std::string m_art_ttf_path;
    int m_art_ttf_ptsize;

    // 普通字体
    std::string m_normal_ttf_path;
    int m_normal_ttf_ptsize;

    // 渲染的种类
    interface_kind_type m_render_type;
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


