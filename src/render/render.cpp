/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2023-12-30
* Description: This is main file
* Copyright (c) 2023, All rights reserved
=============================================*/
#include "render.h"

Render::Render(const int& width, const int& height)
{
    this->width = width;
    this->height = height;
    DEBUGLOG("Render init success!");
}

Render::~Render()
{
    DEBUGLOG("~Render success!");
}

bool Render::initRender()
{
    //Initialization flag
    bool success = true;
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        ERRORLOG("SDL could not initialize! SDL_Error: ", SDL_GetError());
        success = false;
    }
    else
    {
        DEBUGLOG("SDL initialize success!");
        //Set texture filtering to linear
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            ERRORLOG("Warning: Linear texture filtering not enabled!");
        }
        //Create window
        this->gWindow = SDL_CreateWindow("五子棋", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN);
        if(gWindow == nullptr)
        {
            ERRORLOG("Window could not be created! SDL_Error: ", SDL_GetError());
            success = false;
        }
        else
        {
            DEBUGLOG("Create window success!");
            //为窗口创建垂直同步渲染器
            /*
            垂直同步,VSync 可以让渲染与显示器在垂直刷新时的更新同步进行。在本教程中，它将确保动画的运行速度不会太快。
            大多数显示器的运行速度约为每秒 60 帧，这也是我们的假设。如果你的显示器刷新率不同，这就能解释为什么动画运行得过快或过慢
            */
            this->gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(gRenderer == nullptr)
            {
                ERRORLOG("Renderer could not be created! SDL Error:", SDL_GetError());
                success = false;
            }
            else
            {
                DEBUGLOG("Create renderer success!");
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) & imgFlags))
                {
                    ERRORLOG("SDL_image could not initialize! SDL_image Error:", IMG_GetError());
                    success = false;
                }
                // //Initialize SDL_ttf
                // if(TTF_Init() == -1)
                // {
                //     ERRORLOG("SDL_ttf could not initialize! SDL_ttf Error:", TTF_GetError());
                //     success = false;
                // }
            }
        }
    }
    INFOLOG("initRender success!");
    return success;
}

bool Render::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load data stream
    // if(!gSplashTexture.loadFromFile("splash.png"))
    // {        
    //     ERRORLOG("Failed to load splash texture!");
    //     success = false;
    // }
    // else
    // {
    //     //Create texture from manually color keyed pixels
    //     if(!gFooTexture.loadFromPixels())
    //     {
    //         ERRORLOG("Unable to load Foo' texture from surface!");
    //     }
    // }

    return success;
}

void Render::closeRender()
{
    //Destroy windows
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    //Quit SDL subsystems
    // TTF_Quit();
    // Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Render::renderMenu()
{
    //Start up SDL and create window
    if(!this->initRender())
    {
        ERRORLOG("Failed to initialize!");
    }
    else
    {
        //Load media
        if(!loadMedia())
        {
            ERRORLOG("Failed to load media!");
        }
        else
        {
            //Hack to get window to stay up
            SDL_Event e;
            bool quit = false;

            //While application is running
            while(!quit)
            {
                //Handle events on queue
                while(SDL_PollEvent(&e) != 0)
                {
                    //User requests quit
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }
                 //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //Render chessboard backgroud
                SDL_Rect fillRect = {0, 0, this->width, this->height};
                SDL_SetRenderDrawColor(gRenderer, 255, 228, 181, 0xFF);        
                SDL_RenderFillRect(gRenderer, &fillRect);

                // //Render green outlined quad
                // SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
                // SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);        
                // SDL_RenderDrawRect(gRenderer, &outlineRect);
                
                //Draw blue horizontal line
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);        
                SDL_RenderDrawLine(gRenderer, 0, this->height / 2, this->width, this->height / 2);

                // //Draw vertical line of yellow dots
                // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                // for(int i = 0; i < SCREEN_HEIGHT; i += 4)
                // {
                //     SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
                // }

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
        //Free resources and close SDL
        this->closeRender();
    }
}