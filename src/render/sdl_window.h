#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include "../logger/logger.h"
#include "../utils/config.h"

//显示屏都有一个整数 ID 和一个与之相关的矩形，因此知道每个显示屏在桌面上的位置和尺寸
int gTotalDisplays = 0;
SDL_Rect* gDisplayBounds = nullptr; 


class SDLWindow
{
private:
    //封装的窗口,渲染
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    int window_id;
    //窗口显示 ID 用于跟踪窗口所在的显示屏
    int window_display_id;

    //窗口的尺寸
    int mWidth;
    int mHeight;

    //窗口焦点类型的标志
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullScreen;
    bool mMinimized;
    bool mShown;

    std::string name;
    // 窗口标题
    std::string title;

public:
    //Intializes internals
    SDLWindow(const Config& config, const std::string& name);

    ~SDLWindow();

    //Creates window
    bool init();

    //从窗口创建呈现器的函数
    // SDL_Renderer* createRenderer();

    //Handles window events
    void handleEvent(SDL_Event& e);

    //Focuses on window
    void focus();

    //Shows windows contents
    void render();

    //Deallocates internals
    void free();

    //Window dimensions
    int getWidth();
    int getHeight();

    //Window focii
    bool hasMouseFocus();
    bool hasKeyboardFocus();
    bool isMinimized();
    bool isShown();
};
