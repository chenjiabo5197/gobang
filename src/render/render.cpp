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
    this->render_type = DEFAULT_INTERFACE;
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
            DEBUGLOG("Create window success, width={}, height={}", this->width, this->height);
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

void Render::renderer()
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

                switch (this->render_type)
                {
                case PLAYCHESS_INTERFACE:
                    this->renderPlayChessInterface();
                    break;
                default:
                    break;
                }

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
        //Free resources and close SDL
        this->closeRender();
    }
}

bool Render::renderPlayChessInterface()
{
    // 棋盘的左上角坐标
    int boundary_x = 50;
    int boundary_y = 50;
    // 棋盘的每个格子大小
    int lattice_size = 60;
    // 渲染棋盘背景色
    SDL_Rect chessboard_backgroud = {boundary_x, boundary_y, lattice_size*14, lattice_size*14};
    SDL_SetRenderDrawColor(gRenderer, 255, 246, 143, 0xFF);        
    SDL_RenderFillRect(gRenderer, &chessboard_backgroud);

    // 渲染棋盘边界正方形,五子棋盘一共有15*15行，一共是14*14个间距,此处+1是为了渲染棋盘边框两次，使棋盘边框线看起来粗一点
    SDL_Rect chessboard_boundary = {boundary_x+1, boundary_y+1, lattice_size*14+1, lattice_size*14+1};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);        
    SDL_RenderDrawRect(gRenderer, &chessboard_boundary);
    
    //渲染棋盘网格线
    for (int i = 0; i < 15; i++)
    {
        // 横向的棋盘线
        SDL_RenderDrawLine(gRenderer, boundary_x, lattice_size*i+boundary_y, lattice_size*14+boundary_x, lattice_size*i+boundary_y);
        // 纵向的棋盘线
        SDL_RenderDrawLine(gRenderer, lattice_size*i+boundary_x, boundary_y, lattice_size*i+boundary_x, lattice_size*14+boundary_y);
    }
    // 渲染棋盘上五个点
    this->renderCircle(lattice_size*7+boundary_x, lattice_size*7+boundary_y, 5);
    this->renderCircle(lattice_size*3+boundary_x, lattice_size*3+boundary_y, 5);
    this->renderCircle(lattice_size*3+boundary_x, lattice_size*11+boundary_y, 5);
    this->renderCircle(lattice_size*11+boundary_x, lattice_size*3+boundary_y, 5);
    this->renderCircle(lattice_size*11+boundary_x, lattice_size*11+boundary_y, 5);
    return true;
}

void Render::setRendererType(const interface_kind_type& render_type)
{
    this->render_type = render_type;
    INFOLOG("setRendererType||set render_type={}", (int)render_type);
}

void Render::renderCircle(const int& x, const int& y, const int& radius)
{
    INFOLOG("renderCircle||x={}||y={}||radius={}", x, y, radius);
    for (int k = 0; k < 2; k++)  // 向上和向下各渲染一次
    {
        for (int i = 1; i < radius; i++)
        {
            int row_point_nums = 2*radius+1-2*i;
            for (int j = 1; j <= row_point_nums; j++)
            {
                int temp_x = j-(radius+1-i) + x;
                int temp_y = 0;
                if (k == 0)
                {
                    temp_y = y-i;
                }
                else
                {
                    temp_y = y+i;
                }
                SDL_RenderDrawPoint(gRenderer, temp_x, temp_y);
                DEBUGLOG("renderCircle||temp_x={}||temp_y={}||row_point_nums={}", temp_x, temp_y, row_point_nums);
            }
        }
    }     
}