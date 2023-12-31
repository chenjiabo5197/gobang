/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2023-12-30
* Description: This is main file
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include <math.h>
#include "sdl_texture.h"

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

class Render
{
private:
    int width;
    int height;

    SDL_Window * gWindow;
    SDL_Renderer* gRenderer;

    // 渲染的种类
    interface_kind_type render_type;

public:
    Render(const int& width, const int& height);
    ~Render();

    // 初始化渲染
    bool initRender();

    bool loadMedia();

    // 关闭渲染，释放资源
    void closeRender();

    // 渲染主函数
    void renderer();

    // 渲染圆形，传入圆心坐标和圆的半径
    void renderCircle(const int& x, const int& y, const int& radius);

    // 渲染下棋界面
    bool renderPlayChessInterface();

    // 设置渲染界面类型
    void setRendererType(const interface_kind_type& render_type);
};


