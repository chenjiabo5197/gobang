#pragma once

#include <SDL2/SDL.h>
#include "../render/sdl_button.h"
#include "../logger/logger.h"
#include "../utils/config.h"

class SelectPlayManage
{
private:
    // 渲染的窗口
    SDL_Window* global_window;
    SDL_Renderer* global_renderer;
    
    SDLButton* main_menu_buttons[4];

    // 按键之间的间隔(上下间隔)
    int button_interval;
public:
    SelectPlayManage(const Config& config);
    ~SelectPlayManage();

    // 按键
    SDLButton* single_player_button;
    SDLButton* two_players_button;
    SDLButton* play_internet_button;
    SDLButton* normal_back_menu_button;

    void startRender();

    // 初始化
    void init(SDL_Window * global_window, SDL_Renderer* global_renderer);

    // 加载资源
    void loadResource();

    // 关闭渲染，释放资源
    void closeRender();

    // 渲染主函数
    void renderer();

    // 处理事件
    bool handleEvents(SDL_Event* event);
};



