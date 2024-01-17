#pragma once

#include <SDL2/SDL.h>
#include "../render/sdl_button.h"
#include "../render/sdl_window.h"
#include "../logger/logger.h"
#include "../utils/config.h"

class SelectPlayManage
{
private:
    // 主窗口
    SDLWindow* selete_main_window;
    // 选择先后手的窗口(也是选择棋子颜色，黑色先手)
    SDLWindow* select_chess_color_window;
    
    // 当前页面要渲染的按键数组
    SDLButton* select_play_buttons[4];

    // 数组长度
    int array_length;

    //当前页面所有按键的中心位置，按键以此为中心，纵向分布
    int buttons_x;
    int buttons_y;

    // 按键之间的间隔(上下间隔)
    int button_interval;

    // 单人游戏选择先后手
    void singlePlaySelectChess();
public:
    SelectPlayManage(const Config& config);
    ~SelectPlayManage();

    void startRender();

    // 初始化
    void init(SDLWindow* sdl_window);

    // 处理事件
    void handleEvents(SDL_Event* event);
};



