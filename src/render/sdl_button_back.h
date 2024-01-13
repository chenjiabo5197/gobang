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
    SDLButton(const Config& config, const std::string button_name, const int& x, const int& y);

    ~SDLButton();

    //用于事件循环的事件处理函数
    void handleButtonEvent(SDL_Event* e);

    //渲染函数,默认缩放倍数1.0，一般使用config中的倍数，当一个图片在不同地方使用时，需要外传此倍数
    bool buttonRender(SDL_Renderer* global_renderer);

    // 加载资源
    bool loadResource(SDL_Window * global_window, SDL_Renderer* global_renderer);

    // 获取按键当前枚举值
    sdl_button_sprite getButtonCurrentSprite();

    // 重置按键枚举值
    void initButtonCurrentSprite();

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
    int button_x;
    int button_y;

    // 按键精灵  按键外，按键内，鼠标点击，鼠标释放
    SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

    // 按键名
    std::string button_name;

    SDLTexture* sdl_texture;

    //精灵枚举
    sdl_button_sprite mCurrentSprite;
};

