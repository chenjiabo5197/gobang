/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-6
* Description: This is ttf_interface.h file
* Copyright (c) 2023, All rights reserved
=============================================*/

#pragma once

#include "sdl_texture.h"
#include "../logger/logger.h"
#include "../utils/config.h"

class SDLTTF
{
private:
    SDLTexture* sdl_texture;

    // 是否已经加载渲染文字
    bool is_load_text;

    // 字体的缩放倍数
    float ttf_result_multiple;

    std::string ttf_name;

    // 渲染的参数
    int render_r;
    int render_g;
    int render_b;
    int render_alpha;
    // 渲染文字
    std::string render_text;
public:
    SDLTTF(const Config& config, const std::string& name);

    // 该初始化函数初始化之后，渲染的文字和颜色需要实时传入
    SDLTTF(const std::string& name);
    ~SDLTTF();

    // 加载渲染文字
    bool loadRenderText(SDL_Renderer* gRenderer, TTF_Font* gFont);

    // 加载渲染文字,输入要渲染的文字和颜色
    bool loadRenderText(SDL_Renderer* gRenderer, TTF_Font* gFont, const std::string& texture_text, SDL_Color sdl_color);

    // 渲染
    bool ttfRender(SDL_Renderer* gRenderer, const int& x, const int& y);

    // 渲染，加渲染倍数
    bool ttfRender(SDL_Renderer* gRenderer, const int& x, const int& y, const float& multiple);
};




