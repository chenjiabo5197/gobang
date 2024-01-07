#pragma once
#include <SDL2/SDL.h>
#include "sdl_texture.h"
#include "../logger/logger.h"
#include "../utils/config.h"
#include "../common.h"

//鼠标按键
class SDLButton
{
public:
    //Initializes internal variables
    SDLButton(const Config& config, const std::string button_name);

    ~SDLButton();

    //用于事件循环的事件处理函数
    void handleEvent(SDL_Event* e);

    //渲染函数
    bool buttonRender(SDL_Renderer* gRenderer);

    // 加载资源
    bool loadResource(SDL_Window * gWindow, SDL_Renderer* gRenderer);

private:
    bool is_load_resource;

    // 按键的资源地址
    std::string button_resource_path;
    // 按键的宽和高
    int button_width;
    int button_height;
    // 按键缩放的尺寸
    float button_multiple;
    //位置坐标
    SDL_Point button_position;

    // 按键精灵  按键外，按键内，鼠标点击，鼠标释放
    SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

    // 按键名
    std::string button_name;

    SDLTexture* sdl_texture;

    //精灵枚举
    sdl_button_sprite mCurrentSprite;
};

