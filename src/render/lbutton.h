#pragma once
#include <SDL2/SDL.h>

// 单个区域的宽和高
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT,
    BUTTON_SPRITE_MOUSE_OVER_MOTION,
    BUTTON_SPRITE_MOUSE_DOWN,
    BUTTON_SPRITE_MOUSE_UP,
    BUTTON_SPRITE_TOTAL
};

// 场景精灵
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];

LTexture gButtonSpriteSheetTexture;

//鼠标按键
class LButton
{
    public:
        //Initializes internal variables
        LButton();

        //位置设置函数
        void setPosition( int x, int y );

        //用于事件循环的事件处理函数
        void handleEvent( SDL_Event* e );
    
        //渲染函数
        void render();

    private:
        //位置枚举
        SDL_Point mPosition;

        //精灵枚举
        LButtonSprite mCurrentSprite;
};

