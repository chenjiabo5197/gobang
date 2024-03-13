/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2023-12-30
* Description: 储存全局变量
* Copyright (c) 2023, All rights reserved
=============================================*/
#pragma once

#include <SDL2/SDL_mixer.h>
#include "../render/sdl_window.h"

// 背景音乐  // TODO 背景音
extern Mix_Music* background_music;

// 下棋音效
extern Mix_Chunk* chess_down_sound;

// 选择按键音效
extern Mix_Chunk* select_button_sound;

// 主窗口
extern SDLWindow* main_window;