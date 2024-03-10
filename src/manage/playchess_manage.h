/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: 下棋页面管理，负责下棋页面的渲染
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include <SDL2/SDL.h>
#include "../render/sdl_button.h"
#include "../render/sdl_window.h"
#include "../logger/logger.h"
#include "../utils/config.h"
#include "../chess/chessboard.h"
#include "../chess/chess_data_board.h"
#include "../player/machine.h"
#include "../player/player.h"

class PlaychessManage
{
private:
    // 主窗口
    SDLWindow* playchess_main_window;

    // 电脑AI
    Machine* machine;
    // 玩家
    Player* single_player;
    // 玩家2
    Player* single_player2;
    
    // 棋盘渲染时普通字体
    TTF_Font* normal_ttf;
    // 行楷字体
    TTF_Font* art_ttf;
    
    // 当前页面要渲染的按键数组
    SDLButton* playchess_buttons[2];

    // 是否需要重置单人游戏数据板
    bool is_reset_single_player_chess_data_board;
    // 是否需要重置双人游戏数据板
    bool is_reset_two_players_chess_data_board;

    // 按键的数组长度，渲染按键时使用
    int array_length;

    //当前页面所有按键的中心位置，按键以此为中心，纵向分布
    int buttons_x;
    int buttons_y;

    // 按键之间的间隔(上下间隔)
    int button_interval;

    // 棋盘
    Chessboard* chessboard;

    // 数据板
    ChessDataBoard* chess_data_board;
public:
    PlaychessManage(const Config& config);
    ~PlaychessManage();

    void startRender();

    // 初始化
    void init(SDLWindow* sdl_window, TTF_Font* normal_ttf, TTF_Font* art_ttf);

    // 处理事件
    void handleEvents(SDL_Event* event);

    // 处理鼠标点击事件
    bool handleMouseClick(SDL_Event* event);

    // 获取棋盘中心的坐标
	int get_chessboard_center_x();
	int get_chessboard_center_y();
};



