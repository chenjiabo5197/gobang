#pragma once
/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2023-12-31
* Description: This is sdl_texture.h file
* Copyright (c) 2023, All rights reserved
=============================================*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../logger/logger.h"

// 封装SDL_Texture
//Texture wrapper class
class SDLTexture
{
public:
    //Initializes variables
    SDLTexture(SDL_Window* gWindow, SDL_Renderer* gRenderer, const std::string& name);

    //Deallocates memory
    ~SDLTexture();

    //Loads image at specified path
    bool loadFromFile(const std::string& path);

    #if defined(SDL_TTF_MAJOR_VERSION)
    /*
    如果头文件不包含 SDL_ttf.h，编译器就会忽略它。它检查是否定义了 SDL_TTF_MAJOR_VERSION 宏。
    与 #include 一样，#if 也是用于与编译器对话的宏。在本例中，它表示如果 SDL_ttf 未定义，则忽略这段代码
    */
    //Creates image from font string
    // SDL_ttf 的工作方式是根据字体和颜色创建新图像。对于纹理类来说，这意味着将从 SDL_ttf 渲染的文本而不是文件中加载图像
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
    #endif

    //Loads image into pixel buffer  加载像素 
    bool loadPixelsFromFile(const std::string& path);

    //Creates image from preloaded pixels  将纹理转化为像素  
    bool loadFromPixels();

    //createBlank 函数分配了一个空白纹理，可以在流式传输时将数据复制到该纹理中
    bool createBlank(int width, int height);

    //接收了access参数，该参数定义了如何访问它
    bool createBlank(int width, int height, SDL_TextureAccess access);

    //Deallocates texture
    void free();

    //设置调制颜色，接收红、绿、蓝三色分量
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //设置纹理混合  它将控制纹理的混合方式。为了使混合正常工作，必须设置纹理的混合模式
    void setBlendMode(SDL_BlendMode blending);

    //设置纹理的 alpha 透明度
    void setAlpha(Uint8 alpha);

    //Renders texture at given point
    /*接受一个矩形参数，用来定义要渲染的纹理部分。给它一个默认参数 nullptr，以防渲染整个纹理
    接受旋转角度、纹理旋转点和 SDL 翻转枚举值，同样也给出了参数的默认值，以备在不旋转或翻转的情况下渲染纹理。
    */
    void render(int x, int y, float multiple = 1.0, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //渲染该纹理
    void setAsRenderTarget();

    //Gets image dimensions
    int getWidth();
    int getHeight();

    //Pixel accessors
    /*
    间距基本上就是纹理在内存中的宽度。在一些老式硬件和移动硬件上，纹理的大小会受到限制。
    如果创建一个宽度为 100 像素的纹理，它可能会被填充为 128 像素宽（2 的下一次幂）。
    通过间距，可以知道图像在内存中的位置
    */
    Uint32* getPixels32();  // 获取原始像素
    Uint32 getPitch32();   // 获取像素间距
    Uint32 getPixel32(Uint32 x, Uint32 y);  // 获取精确 x/y 坐标的像素
    // copyRawPixels()函数会复制要流式传输的原始像素数据
    void copyRawPixels32(void* pixels);
    //  lockTexture()，可以获取一个指针来发送像素
    bool lockTexture();
    // unlockTexture()则可以将像素数据上传到像素
    bool unlockTexture();

private:
    // 渲染的窗口
    SDL_Window * gWindow;
    SDL_Renderer* gRenderer;

    std::string texture_name;

    //实际渲染的纹理
    SDL_Texture* mTexture;

    //Surface pixels
    SDL_Surface* mSurfacePixels;

    //原始像素
    void* mRawPixels;
    int mRawPitch;

    //图像的宽和高
    int mWidth;
    int mHeight;
};

