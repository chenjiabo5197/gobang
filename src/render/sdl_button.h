#pragma once
#include <SDL2/SDL.h>
#include "sdl_texture.h"
#include "logger.h"
#include "config.h"
#include "common.h"
#include "global.h"

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
    bool m_is_load_resource;

    // 按键的资源地址
    std::string m_button_resource_path;

    // 按键的宽和高
    int m_button_width;
    int m_button_height;

    // 按键缩放的尺寸
    float m_button_multiple;

    //位置坐标,按键的中心处坐标
    int m_button_x;
    int m_button_y;

    // 按键精灵  按键外，按键内，鼠标点击，鼠标释放
    SDL_Rect m_sprite_clips[BUTTON_SPRITE_TOTAL];

    // 按键名
    std::string m_button_name;

    SDLTexture* m_sdl_texture;

    //精灵枚举
    sdl_button_sprite m_current_sprite;
};

