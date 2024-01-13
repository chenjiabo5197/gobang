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
    
    // 当前页面要渲染的按键数组
    SDLButton* select_play_buttons[4];

    // 数组长度
    int array_length;

    //当前页面所有按键的中心位置，按键以此为中心，纵向分布
    int buttons_x;
    int buttons_y;

    // 按键之间的间隔(上下间隔)
    int button_interval;
public:
    SelectPlayManage(const Config& config);
    ~SelectPlayManage();

    void startRender();

    // 初始化
    void init(SDL_Window * global_window, SDL_Renderer* global_renderer);

    // 加载资源
    void loadResource();

    // 处理事件
    void handleEvents(SDL_Event* event);
};



