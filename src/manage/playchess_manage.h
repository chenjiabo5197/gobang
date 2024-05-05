/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: 下棋页面管理，负责下棋页面的渲染
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include <SDL2/SDL.h>
#include "common.h"
#include "global.h"
#include "sdl_button.h"
#include "sdl_window.h"
#include "logger.h"
#include "config.h"
#include "chessboard.h"
#include "chess_data_board.h"
#include "machine.h"
#include "player.h"

class PlaychessManage
{
private:

    // 电脑AI
    Machine* m_machine;
    // 玩家
    Player* m_single_player;
    // 玩家2
    Player* m_single_player2;
    
    // 当前页面要渲染的按键数组
    SDLButton* m_playchess_buttons[2];

    // 是否需要重置单人游戏数据板
    bool m_is_reset_single_player_chess_data_board;
    // 是否需要重置双人游戏数据板
    bool m_is_reset_two_players_chess_data_board;

    // 按键的数组长度，渲染按键时使用
    int m_array_length;

    //当前页面所有按键的中心位置，按键以此为中心，纵向分布
    int m_buttons_x;
    int m_buttons_y;

    // 按键之间的间隔(上下间隔)
    int m_button_interval;

    // 棋盘
    Chessboard* m_chessboard;

    // 数据板
    ChessDataBoard* m_chess_data_board;
public:
    PlaychessManage(const Config& config);
    ~PlaychessManage();

    void startRender();

    // 初始化
    void init();

    // 处理事件
    void handleEvents(SDL_Event* event);

    // 处理鼠标点击事件
    bool handleMouseClick(SDL_Event* event);

    // 获取棋盘中心的坐标
	int getChessboardCenterX();
	int getChessboardCenterY();
};



