/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: This is manage.h file
* Copyright (c) 2023, All rights reserved
=============================================*/

#pragma once

#include <math.h>
#include "../common.h"
#include "../logger/logger.h"
#include "../utils/config.h"
#include "../render/sdl_texture.h"
#include "../chess/chessboard.h"
#include "../player/machine.h"
#include "../player/player.h"

class Manage
{
private:
    // 渲染的宽和高
    int width;
    int height;

    SDL_Window * gWindow;
    SDL_Renderer* gRenderer;

    // 棋盘
    Chessboard* chessboard;

    // 渲染的种类
    interface_kind_type render_type;

    // 电脑AI
    Machine* machine;

    // 玩家
    Player* single_player;
    
public:
    Manage(const Config& config);
    ~Manage();

    void start();

    // 初始化渲染
    bool initRender();

    // 关闭渲染，释放资源
    void closeRender();

    // 渲染主函数
    void renderer();

    // 处理鼠标点击事件
    bool handleMouseClick(SDL_Event* e);

    // 设置渲染界面类型
    void setRendererType(const interface_kind_type& render_type);
};


