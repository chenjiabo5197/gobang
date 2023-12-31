/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2023-12-30
* Description: This is main file
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include "sdl_texture.h"

class Render
{
private:
    int width;
    int height;

    SDL_Window * gWindow;
    SDL_Renderer* gRenderer;

public:
    Render(const int& width, const int& height);
    ~Render();

    bool initRender();

    bool loadMedia();

    void closeRender();

    void renderMenu();
};


