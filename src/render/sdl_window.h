#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include "logger.h"
#include "config.h"
#include "sdl_texture.h"

class SDLWindow
{
private:
    int m_window_id;
    //窗口显示 ID 用于跟踪窗口所在的显示屏
    int m_window_display_id;

    //封装的窗口,渲染
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    // 背景图片的渲染
    SDLTexture* m_background_texture;

    //窗口的尺寸
    int m_width;
    int m_height;

    //窗口焦点类型的标志
    bool m_mouse_focus;
    bool m_keyboard_focus;
    bool m_full_screen;
    bool m_minimized;
    bool m_shown;

    // 窗口名，区分
    std::string m_name;
    // 窗口标题
    std::string m_title;
    // 窗口背景图片路径，默认为纯白色
    std::string m_background_path;

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

    int getWindowId();

    // 获取渲染窗口
    SDL_Window* getWindow();
    SDL_Renderer* getRenderer();

    //Window focii
    bool hasMouseFocus();
    bool hasKeyboardFocus();
    bool isMinimized();
    bool isShown();

    // 渲染该窗口的背景
    void renderBackground();
};
