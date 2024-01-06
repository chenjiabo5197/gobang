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

class TTFResultInterface
{
private:
    SDLTexture* sdl_texture;

    bool is_load_text;

    float ttf_result_multiple;
public:
    TTFResultInterface(const Config& config);
    ~TTFResultInterface();

    bool loadRenderText(SDL_Renderer* gRenderer, TTF_Font* gFont, const std::string& textureText, SDL_Color textColor);

    bool ttfRender(SDL_Renderer* gRenderer, const int& x, const int& y);
};




