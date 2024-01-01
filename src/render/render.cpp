/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2023-12-30
* Description: This is main file
* Copyright (c) 2023, All rights reserved
=============================================*/
#include "render.h"

Render::Render(const Config& config)
{
    this->width = config.Read("screen_width", 0);
    this->height = config.Read("screen_height", 0);
    this->render_type = DEFAULT_INTERFACE;
    this->chessboard = new Chessboard(config);
    DEBUGLOG("Render construct success||width={}||height={}||render_type={}", this->width, this->height, (int)this->render_type);
}

Render::~Render()
{
    delete(chessboard);
    DEBUGLOG("~Render success, release chessboard");
}

bool Render::initRender()
{
    //Initialization flag
    bool success = true;
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        ERRORLOG("SDL could not initialize||SDL_Error: ", SDL_GetError());
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
            ERRORLOG("Window could not be created||SDL_Error: ", SDL_GetError());
            success = false;
        }
        else
        {
            DEBUGLOG("Create window success||width={}||height={}", this->width, this->height);
            //为窗口创建垂直同步渲染器
            /*
            垂直同步,VSync 可以让渲染与显示器在垂直刷新时的更新同步进行。在本教程中，它将确保动画的运行速度不会太快。
            大多数显示器的运行速度约为每秒 60 帧，这也是我们的假设。如果你的显示器刷新率不同，这就能解释为什么动画运行得过快或过慢
            */
            this->gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(gRenderer == nullptr)
            {
                ERRORLOG("Renderer could not be created||SDL Error:", SDL_GetError());
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
                    ERRORLOG("SDL_image could not initialize||SDL_image Error:", IMG_GetError());
                    success = false;
                }
                // //Initialize SDL_ttf
                // if(TTF_Init() == -1)
                // {
                //     ERRORLOG("SDL_ttf could not initialize||SDL_ttf Error:", TTF_GetError());
                //     success = false;
                // }
            }
        }
    }
    INFOLOG("initRender success!");
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

void Render::renderer()
{
    //Start up SDL and create window
    if(!this->initRender())
    {
        ERRORLOG("Failed to initialize!");
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
                else if (e.type == SDL_MOUSEBUTTONDOWN)   // 鼠标点击事件
                {
                    this->handleMouseClick(&e);
                }
            }
            //Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            this->chessboard->render(gWindow, gRenderer);
            //Update screen
            SDL_RenderPresent(gRenderer);
        }
        // }
        //Free resources and close SDL
        this->closeRender();
    }
}

bool Render::handleMouseClick(SDL_Event* e)
{
    //获取鼠标位置
    int x, y;
    SDL_GetMouseState(&x, &y);
    ChessPos pos;
    // 检查是否有效落子
    bool is_valid_click = this->chessboard->clickBoard(x, y, &pos);
    if (is_valid_click)
    {
        this->chessboard->chessDown(pos, CHESS_BLACK);
    }
    return is_valid_click;
}

void Render::setRendererType(const interface_kind_type& render_type)
{
    this->render_type = render_type;
    INFOLOG("setRendererType||set render_type={}", (int)render_type);
}
