#pragma once

#include <SDL2/SDL.h>
#include "../chess/chess.h"
#include "../render/sdl_ttf.h"
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

    // 处理选择先后手窗口事件,返回白棋和黑棋与鼠标的位置关系枚举
    std::pair<sdl_button_sprite, sdl_button_sprite> handleWindowEvents(SDL_Event* event, bool mouse_focus);

    // 渲染选择先后手窗口,返回值为true表示已经选择好了先后手，选择窗口关闭
    bool selectChessRender(std::pair<sdl_button_sprite, sdl_button_sprite> mouse_event);

    // 选择先后手的棋子
    Chess* white_color_chess;		
    Chess* black_color_chess;

    // 棋盘渲染的字体
    TTF_Font* art_ttf;
    SDLTTF* select_chess_color_ttf;

    // 渲染文字到指定位置(选择先后手窗口专用)
	void renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple);
public:
    SelectPlayManage(const Config& config);
    ~SelectPlayManage();

    void startRender();

    // 初始化
    void init(SDLWindow* sdl_window, TTF_Font* art_ttf);

    // 处理事件
    void handleEvents(SDL_Event* event);
};



