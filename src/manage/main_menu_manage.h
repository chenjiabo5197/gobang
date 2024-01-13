#pragma once

#include <SDL2/SDL.h>
#include "../render/sdl_button.h"
#include "../logger/logger.h"
#include "../utils/config.h"

class MainMenuManage
{
private:

    // 渲染的窗口
    SDL_Window* global_window;
    SDL_Renderer* global_renderer;
    
    SDLButton* main_menu_buttons[3];

    // 按键之间的间隔(上下间隔)
    int button_interval;

    // 按键
    SDLButton* start_game_button;
    SDLButton* best_scores_button;
    SDLButton* exit_game_button;
public:
    MainMenuManage(const Config& config);
    ~MainMenuManage();

    void startRender();

    // 初始化
    void init(SDL_Window * global_window, SDL_Renderer* global_renderer);

    // 加载资源
    void loadResource();

    // 处理事件
    void handleEvents(SDL_Event* event);
};
