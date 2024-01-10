/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2023-12-31
* Description: This is draw.cpp file
* Copyright (c) 2023, All rights reserved
=============================================*/
#include "sdl_texture.h"

SDLTexture::SDLTexture(const std::string& name)
{
    //Initialize
    mTexture = nullptr;
    mSurfacePixels = nullptr;
    mWidth = 0;
    mHeight = 0;
    this->texture_name = name;
    INFOLOG("SDLTexture construct success, texture_name={}", this->texture_name);
}

SDLTexture::~SDLTexture()
{
    //Deallocate
    free();
    INFOLOG("~SDLTexture, release resource");
}

void SDLTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    //纹理颜色调制
    /*
    DL_SetTextureColorMod 接受 Uint8 作为颜色组件的参数。Uint8 只是一个无符号的 8 位整数。这意味着它的范围从 0 到 255。
    128 大约介于 0 和 255 之间，因此当您将绿色调制为 128 时，纹理上任何像素的绿色分量都会减半。
    红色和蓝色方格不会受到影响，因为它们没有绿色，但绿色的亮度会减半，白色则会变成浅洋红色（洋红色为红色 255、绿色 0、蓝色 255）。
    颜色调制只是将一种颜色乘以整个纹理的一种方法
    */
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool SDLTexture::loadFromRenderedText(SDL_Renderer* gRenderer, TTF_Font* gFont, std::string textureText, SDL_Color textColor)
{
    ////先取消原有纹理的渲染
    free();

    //Render text surface
    /*
    使用 TTF_RenderText_Solid 加载一个曲面。函数会根据给出的字体、文本和颜色创建一个纯色曲面。如果曲面创建成功，
    就会像之前从文件加载曲面一样创建纹理。创建纹理后，可以像渲染其他纹理一样对其进行渲染。
    */
    if(gFont == nullptr)
    {
        ERRORLOG("gFont nullptr");
        return false;
    }
    // linux下加载中文
    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(gFont, textureText.c_str(), textColor);
    // SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if(textSurface == nullptr)
    {
        ERRORLOG("Unable to render text surface! SDL_ttf Error={}", TTF_GetError());
        return false;
    }
    //Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if(mTexture == nullptr)
    {
        ERRORLOG("Unable to create texture from rendered text! SDL Error={}", SDL_GetError());
        return false;
    }
    //Get image dimensions
    mWidth = textSurface->w;
    mHeight = textSurface->h;
    //Get rid of old surface
    SDL_FreeSurface(textSurface);
    //Return success
    return mTexture != nullptr;
}
#endif

bool SDLTexture::loadPixelsFromFile(SDL_Window * gWindow, const std::string& path)
{
    //Free preexisting assets
    free();

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == nullptr)
    {
        ERRORLOG("Unable to load image={}, SDL_image Error={}", path.c_str(), IMG_GetError());
    }
    else
    {
        // DEBUGLOG("loadPixelsFromFile loadedSurface success");
        //将现有的表面复制到指定格式的新的表面 第一个参数为源表面指针，第二个参数为像素格式，第三个参数设置为0即可
        mSurfacePixels = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(gWindow), 0);
        if(mSurfacePixels == nullptr)
        {
            ERRORLOG("Unable to convert loaded surface to display format! SDL Error={}", IMG_GetError());
        }
        else
        {
            //Get image dimensions
            mWidth = mSurfacePixels->w;
            mHeight = mSurfacePixels->h;
            // DEBUGLOG("loadPixelsFromFile SDL_ConvertSurfaceFormat success||mWidth={}||mHeight={}", mWidth, mHeight);
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    // INFOLOG("loadPixelsFromFile mSurfacePixels success||mWidth={}||mHeight={}", mWidth, mHeight);
    return mSurfacePixels != nullptr;
}

// 加载纹理
bool SDLTexture::loadFromPixels(SDL_Renderer* gRenderer)
{
    //Only load if pixels exist
    if(mSurfacePixels == nullptr)
    {
        ERRORLOG("No pixels loaded!");
        return false;
    }
    //Color key image
    SDL_SetColorKey(mSurfacePixels, SDL_TRUE, SDL_MapRGB(mSurfacePixels->format, 0xff, 0xff, 0xff));

    //Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface(gRenderer, mSurfacePixels);
    if(mTexture == nullptr)
    {
        ERRORLOG("Unable to create texture from loaded pixels! SDL Error={}", SDL_GetError());
        return false;
    }
    //Get image dimensions
    mWidth = mSurfacePixels->w;
    mHeight = mSurfacePixels->h;

    //Get rid of old loaded surface
    SDL_FreeSurface(mSurfacePixels);
    mSurfacePixels = nullptr;

    //Return success
    return mTexture != nullptr;
}

bool SDLTexture::loadFromFile(SDL_Window * gWindow, SDL_Renderer* gRenderer, const std::string& path)
{
    INFOLOG("loadFromFile||path={}", path);

    //Load pixels
    if(!loadPixelsFromFile(gWindow, path))
    {
        ERRORLOG("Failed to load pixels, path={}", path.c_str());
        return false;
    }
    //Load texture from pixels
    if(!loadFromPixels(gRenderer))
    {
        ERRORLOG("Failed to texture from pixels, path={}", path.c_str());
        return false;
    }
    return mTexture != nullptr;
}

Uint32* SDLTexture::getPixels32()
{
    Uint32* pixels = nullptr;

    if(mSurfacePixels != nullptr)
    {
        // pixels =  static_cast(mSurfacePixels->pixels);
        pixels =  static_cast<Uint32*>(mSurfacePixels->pixels);
    }

    return pixels;
}

Uint32 SDLTexture::getPitch32()
{
    Uint32 pitch = 0;

    if(mSurfacePixels != nullptr)
    {
        // 间距是以字节表示的，需要的是以像素为单位的间距，而每个像素有 32 位/4 字节，因此可以通过除以每个像素的 4 字节来得到每个间距的像素数
        pitch = mSurfacePixels->pitch / 4;
    }

    return pitch;
}

void SDLTexture::free()
{
    //Free texture if it exists
    if(mTexture != nullptr)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }

    //Free surface if it exists
    if(mSurfacePixels != nullptr)
    {
        SDL_FreeSurface(mSurfacePixels);
        mSurfacePixels = nullptr;
    }
    // INFOLOG("free success");
}

void SDLTexture::render(SDL_Renderer* gRenderer, int x, int y, float multiple, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //设置渲染区域并渲染到屏幕
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    /* 在特定位置渲染纹理时，需要指定一个目标矩形，该矩形可设置 x/y 位置和宽度/高度。在不知道原始图像尺寸的情况下，无法指定宽度/高度。
    因此，当渲染纹理时，创建一个包含位置参数和成员宽度/高度的矩形，并将此矩形传递给 SDL_RenderCopy
    */

    //Set clip rendering dimensions
    if(clip != nullptr)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // 使用倍数来缩放加载的图像,最好用于缩小，放大图片容易造成像素点过大
    if (multiple != 1.0)
    {
        renderQuad.w = (int)renderQuad.w * multiple;
        renderQuad.h = (int)renderQuad.h * multiple;
    }

    // 检查缩放倍数是否为0
    if (multiple == 0)
    {
        ERRORLOG("render||multiple is zero||please check");
    }
    
    /*
    在剪辑时，如果使用的是剪辑矩形的尺寸而不是纹理的尺寸，我们将把目标矩形（此处称为 renderQuad）的宽度/高度设置为剪辑矩形的尺寸。
    我们要将剪辑矩形作为源矩形传递给 SDL_RenderCopy。源矩形定义了要渲染纹理的哪一部分。当源矩形为空时，将渲染整个纹理。
    */

    //Render to screen
    // SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
    //Render to screen 该函数的工作原理与原始的 SDL_RenderCopy 相同，但增加了用于旋转和翻转的参数
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int SDLTexture::getWidth()
{
    return mWidth;
}

int SDLTexture::getHeight()
{
    return mHeight;
}

void SDLTexture::setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    // SDL_SetTextureBlendMode 允许启用混合模式
    SDL_SetTextureBlendMode(mTexture, blending);
}
        
void SDLTexture::setAlpha(Uint8 alpha)
{
    //Modulate texture alpha
    // SDL_SetTextureAlphaMod 则允许设置整个纹理的 Alpha 值
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

// 获取特定位置像素
Uint32 SDLTexture::getPixel32(Uint32 x, Uint32 y)
{
    //Convert the pixels to 32 bit
    Uint32* pixels = static_cast<Uint32*>(mSurfacePixels->pixels);

    //一个二维纹理图像 像素是以一个维度存储的
    return pixels[ (y * getPitch32()) + x ];
}

//创建一个 32 位 RGBA 纹理并进行流访问。创建纹理时必须确保的一点是，纹理像素的格式要与流式传输的像素格式一致
bool SDLTexture::createBlank(SDL_Renderer* gRenderer, int width, int height)
{
    //Get rid of preexisting texture
    free();

    //Create uninitialized texture
    mTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if(mTexture == nullptr)
    {
        ERRORLOG("Unable to create streamable blank texture! SDL Error={}", SDL_GetError());
    }
    else
    {
        mWidth = width;
        mHeight = height;
    }

    return mTexture != nullptr;
}

bool SDLTexture::createBlank(SDL_Renderer* gRenderer, int width, int height, SDL_TextureAccess access)
{
    //Get rid of preexisting texture
    free();

    //Create uninitialized texture
    mTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
    if(mTexture == nullptr)
    {
        ERRORLOG("Unable to create streamable blank texture! SDL Error={}", SDL_GetError());
    }
    else
    {
        mWidth = width;
        mHeight = height;
    }

    return mTexture != nullptr;
}

// 对纹理进行渲染，将其设置为渲染目标
void SDLTexture::setAsRenderTarget(SDL_Renderer* gRenderer)
{
    //Make self render target
    SDL_SetRenderTarget(gRenderer, mTexture);
}

/*
SDL_LockTexture() 会抓取一个像素数据指针和纹理的间距。注意：该指针不会包含原始纹理的数据。如果创建了一个纹理，然后将其锁定，则不要指望它拥有您创建纹理时使用的原始像素数据。
用SDL_LockTexture()提供一个新的像素指针，以便向纹理发送新数据。SDL_LockTexture() 还会以字节为单位抓取纹理间距。
*/
bool SDLTexture::lockTexture()
{
    bool success = true;

    //Texture is already locked
    if(mRawPixels != nullptr)
    {
        ERRORLOG("Texture is already locked!");
        success = false;
    }
    //Lock texture
    else
    {   
        // 函数功能：锁定纹理，第一个参数 纹理指针。第二个参数 锁定区域，NULL 为整个纹理，第三个参数 像素，第四个参数 程度

        if(SDL_LockTexture(mTexture, nullptr, &mRawPixels, &mRawPitch) != 0)
        {
            ERRORLOG("Unable to lock texture, err={} ", SDL_GetError());
            success = false;
        }
    }

    return success;
}

// 用所需的数据完成了对像素指针的操作，解锁它就会将像素数据发送到 GPU。
bool SDLTexture::unlockTexture()
{
    bool success = true;

    //Texture is not locked
    if(mRawPixels == nullptr)
    {
        ERRORLOG("Texture is not locked!");
        success = false;
    }
    //Unlock texture
    else
    {
        SDL_UnlockTexture(mTexture);
        mRawPixels = nullptr;
        mRawPitch = 0;
    }

    return success;
}

// 用于将数据流中的像素复制到锁定的纹理像素指针中。该函数假定像素来自与纹理大小相同的图像
void SDLTexture::copyRawPixels32(void* pixels)
{
    //Texture is locked
    if(mRawPixels != nullptr)
    {
        //Copy to locked pixels
        memcpy(mRawPixels, pixels, mRawPitch * mHeight);
    }
}
