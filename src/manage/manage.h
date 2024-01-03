/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: This is manage.h file
* Copyright (c) 2023, All rights reserved
=============================================*/

#pragma once

#include <math.h>
#include "../logger/logger.h"
#include "../utils/config.h"
#include "../render/sdl_texture.h"
#include "../chess/chessboard.h"

// 枚举渲染的种类
typedef enum {
	DEFAULT_INTERFACE,
	MAIN_MENU_INTERFACE,          // 选择开始游戏和退出游戏
	SELECT_PLAY_INTERFACE,        // 选择单人游戏，双人游戏，网络对战
	PLAYCHESS_INTERFACE,          // 对战中菜单，棋盘和退出游戏，悔棋
	PLAYER_WIN_INTERFACE,         // 单人游戏赢了，选择再来一局或者退出游戏
	PLAYER_LOSE_INTERFACE,        // 单人游戏输了，选择再来一局或者退出游戏
	PLAYER_DRAW_INTERFACE,        // 平局，棋盘落满棋子还未决出胜负
	BLACK_WIN_INTERFACE,          // 黑棋赢
	WHITE_WIN_INTERFACE,          // 白棋赢
	BEST_SCORE_INTERFACE          // 输出排行榜分数
} interface_kind_type;  

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


