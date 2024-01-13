/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: This is manage.h file
* Copyright (c) 2023, All rights reserved
=============================================*/

#pragma once

#include <math.h>
#include <SDL2/SDL_ttf.h>
#include "main_menu_manage.h"
#include "select_play_manage.h"
#include "playchess_manage.h"
#include "settlement_manage.h"
#include "../common.h"
#include "../logger/logger.h"
#include "../utils/config.h"
#include "../render/sdl_texture.h"
#include "../render/sdl_button.h"
#include "../render/sdl_ttf.h"
#include "../chess/chessboard.h"
#include "../player/machine.h"
#include "../player/player.h"

class TopManage
{
private:
    // 各个子页面管理
    MainMenuManage* main_menu_manage;
    SelectPlayManage* select_play_manage;
    PlaychessManage* playchess_manage;
    SettlementManage* settlement_manage;

    // 渲染的宽和高
    int width;
    int height;

    // 行楷字体
    std::string art_ttf_path;
    int art_ttf_ptsize;

    SDL_Window * global_window;
    SDL_Renderer* global_renderer;
    TTF_Font* gResultFont;

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

    // 渲染主函数
    void renderer();

    // 处理鼠标点击事件
    bool handleMouseClick(SDL_Event* e);

    // 设置渲染界面类型
    void setRendererType(const interface_kind_type& render_type);
};


