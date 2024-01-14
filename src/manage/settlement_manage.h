#pragma once

#include <SDL2/SDL.h>
#include "../common.h"
#include "../render/sdl_button.h"
#include "../render/sdl_ttf.h"
#include "../logger/logger.h"
#include "../utils/config.h"

class SettlementManage
{
private:
    // 渲染的窗口
    SDL_Window* global_window;
    SDL_Renderer* global_renderer;
    TTF_Font* art_font;

    // 当前页面要渲染的按键数组
    SDLButton* settlement_buttons[2];

    // 数组长度
    int array_length;

    //当前页面所有按键的中心位置，按键以此为中心，纵向分布
    int buttons_x;
    int buttons_y;

    // 按键之间的间隔(上下间隔)
    int button_interval;

    // 要渲染字体的坐标(字体中心坐标)
    int font_x;
    int font_y;
public:
    SettlementManage(const Config& config);
    ~SettlementManage();

    // 玩家赢结算页面的渲染
    SDLTTF* player_win_interface;
    // 玩家输结算页面的渲染
    SDLTTF* player_lose_interface;

    void startRender(const interface_kind_type& type);

    // 初始化
    void init(SDL_Window * global_window, SDL_Renderer* global_renderer, TTF_Font* art_font);
    
    // 处理事件
    void handleEvents(SDL_Event* event);

    // 设置渲染文字的中心坐标
    void set_font_coordinate(const int& x, const int& y);
};


