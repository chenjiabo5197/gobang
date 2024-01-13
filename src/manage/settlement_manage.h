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
    
    SDLButton* main_menu_buttons[4];

    // 按键之间的间隔(上下间隔)
    int button_interval;

    // 要渲染字体的坐标(字体中心坐标)
    int font_x;
    int font_y;

    // 按键
    SDLButton* back_menu_button;
    SDLButton* again_game_button;
public:
    SettlementManage(const Config& config);
    ~SettlementManage();

    // 玩家赢结算页面的渲染
    SDLTTF* player_win_interface;
    // 玩家输结算页面的渲染
    SDLTTF* player_lose_interface;

    void startRender(const interface_kind_type& type);

    // 初始化
    void init(SDL_Window * global_window, SDL_Renderer* global_renderer, TTF_Font* art_font, const int& x, const int& y);

    // 加载资源
    void loadResource();

    // 处理事件
    void handleEvents(SDL_Event* event);
};


