#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>

//The window renderer
SDL_Renderer* gRenderer = nullptr;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//显示屏都有一个整数 ID 和一个与之相关的矩形，因此知道每个显示屏在桌面上的位置和尺寸
int gTotalDisplays = 0;
SDL_Rect* gDisplayBounds = nullptr; 


class LWindow
{
    public:
        //Intializes internals
        LWindow();

        //Creates window
        bool init();

        //从窗口创建呈现器的函数
        // SDL_Renderer* createRenderer();

        //Handles window events
        void handleEvent( SDL_Event& e );

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

    private:
        //封装的窗口
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;
        int mWindowID;
        //窗口显示 ID 用于跟踪窗口所在的显示屏
        int mWindowDisplayID;

        //窗口的尺寸
        int mWidth;
        int mHeight;

        //窗口焦点类型的标志
        bool mMouseFocus;
        bool mKeyboardFocus;
        bool mFullScreen;
        bool mMinimized;
        bool mShown;
};
