#pragma once

#include <SDL2/SDL.h>
#include "../render/sdl_button.h"
#include "../logger/logger.h"
#include "../utils/config.h"
#include "../chess/chessboard.h"
#include "../player/machine.h"
#include "../player/player.h"

class PlaychessManage
{
private:

    // 电脑AI
    Machine* machine;

    // 玩家
    Player* single_player;

    // 渲染的窗口
    SDL_Window* global_window;
    SDL_Renderer* global_renderer;
    
    SDLButton* main_menu_buttons[4];

    // 按键之间的间隔(上下间隔)
    int button_interval;
public:
    PlaychessManage(const Config& config);
    ~PlaychessManage();

    // 按键
    SDLButton* back_menu_button;
    SDLButton* withdraw_button;

    // 棋盘
    Chessboard* chessboard;

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

    // 处理鼠标点击事件
    bool handleMouseClick(SDL_Event* event);
};



