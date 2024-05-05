/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-6
* Description: This is ttf_interface.h file
* Copyright (c) 2023, All rights reserved
=============================================*/

#pragma once

#include "sdl_texture.h"
#include "logger.h"
#include "config.h"

class SDLTTF
{
private:
    SDLTexture* m_sdl_texture;

    // 是否已经加载渲染文字
    bool m_is_load_text;

    // 字体的缩放倍数
    float m_ttf_result_multiple;

    std::string m_ttf_name;

    // 渲染的参数
    int m_render_r;
    int m_render_g;
    int m_render_b;
    int m_render_alpha;
    // 渲染文字
    std::string m_render_text;
public:
    SDLTTF(const Config& config, const std::string& name);

    // 该初始化函数初始化之后，渲染的文字和颜色需要实时传入
    SDLTTF(const std::string& name);
    ~SDLTTF();

    // 加载渲染文字
    bool loadRenderText(SDL_Renderer* global_renderer, TTF_Font* global_font);

    // 加载渲染文字,输入要渲染的文字和颜色
    bool loadRenderText(SDL_Renderer* global_renderer, TTF_Font* global_font, const std::string& texture_text, SDL_Color sdl_color);

    // 渲染
    bool ttfRender(SDL_Renderer* global_renderer, const int& x, const int& y);

    // 渲染，加渲染倍数
    bool ttfRender(SDL_Renderer* global_renderer, const int& x, const int& y, const float& multiple);
};




