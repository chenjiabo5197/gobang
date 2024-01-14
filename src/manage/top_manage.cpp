/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: This is manage.cpp file
* Copyright (c) 2023, All rights reserved
=============================================*/
#include "top_manage.h"

TopManage::TopManage(const Config& config)
{
    this->setRendererType(DEFAULT_INTERFACE);
    std::string temp;
    this->width = config.Read("screen_width", 0);
    this->height = config.Read("screen_height", 0);
    this->art_ttf_path = config.Read("ttf_result_resource_path", temp);
    this->art_ttf_ptsize = config.Read("ttf_result_ptsize", 0);
    this->normal_ttf_path = config.Read("normal_ttf_resource_path", temp);
    this->normal_ttf_ptsize = config.Read("normal_ttf_ptsize", 0);
    this->main_menu_manage = new MainMenuManage(config);
    this->select_play_manage = new SelectPlayManage(config);
    this->playchess_manage = new PlaychessManage(config);
    this->settlement_manage = new SettlementManage(config);
    DEBUGLOG("Manage construct success||width={}||height={}||render_type={}||art_ttf_path={}||art_ttf_ptsize={}||normal_ttf_resource_path={}||normal_ttf_ptsize={}", 
    this->width, this->height, (int)this->render_type, this->art_ttf_path, this->art_ttf_ptsize, this->normal_ttf_path, this->normal_ttf_ptsize);
}

TopManage::~TopManage()
{
    delete main_menu_manage;
    delete select_play_manage;
    delete playchess_manage;
    delete settlement_manage;
    DEBUGLOG("~Manage success||release resource");
}

void TopManage::start()
{
    this->setRendererType(MAIN_MENU_INTERFACE);
    //Start up SDL and create window
    if(!this->initRender())
    {
        ERRORLOG("Failed to initialize!");
        return;
    }
    //Hack to get window to stay up
    SDL_Event e;
    bool quit = false;
    interface_kind_type last_render_type = DEFAULT_INTERFACE;
    //While application is running
    while(!quit)
    {
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if(e.type == SDL_QUIT || e.type == EXIT_GAME_EVENT)
            {
                quit = true;
            }
            else if (e.type == PLAYER_WIN_EVENT)
            {
                this->playchess_manage->handleEvents(&e);
                this->setRendererType(PLAYER_WIN_PRE_INTERFACE);
            }
            else if (e.type == PLAYER_LOSE_EVENT)
            {
                this->playchess_manage->handleEvents(&e);
                this->setRendererType(PLAYER_LOSE_PRE_INTERFACE);
            }
            else if (e.type == START_GAME_EVENT)
            {
                this->setRendererType(SELECT_PLAY_INTERFACE);
            }
            else if (e.type == SINGLE_PLAYER_EVENT)
            {
                this->setRendererType(PLAYCHESS_INTERFACE);
                this->playchess_manage->handleEvents(&e);
            }
            else if (e.type == BACK_MANU_EVENT)
            {
                this->setRendererType(MAIN_MENU_INTERFACE);
            }
            else if (e.type == PLAYER_WITHDRAW_EVENT)
            {
                this->playchess_manage->handleEvents(&e);
            }
            else if (e.type == AGAIN_GAME_EVENT)
            {
                this->setRendererType(PLAYCHESS_INTERFACE);
                this->playchess_manage->handleEvents(&e);
            }    
            else
            {
                if(this->render_type == PLAYCHESS_INTERFACE)
                {
                    this->playchess_manage->handleEvents(&e);
                }
                else if (this->render_type == PLAYER_WIN_INTERFACE || this->render_type == PLAYER_LOSE_INTERFACE)
                {
                    this->settlement_manage->handleEvents(&e);
                }
                else if (this->render_type == MAIN_MENU_INTERFACE)
                {
                    this->main_menu_manage->handleEvents(&e);
                }
                else if (this->render_type == SELECT_PLAY_INTERFACE)
                {
                    this->select_play_manage->handleEvents(&e);
                }
            }
        }
        if (last_render_type == PLAYER_LOSE_PRE_INTERFACE)
        {
            this->setRendererType(PLAYER_LOSE_INTERFACE);
        } 
        if (last_render_type == PLAYER_WIN_PRE_INTERFACE)
        {
            this->setRendererType(PLAYER_WIN_INTERFACE);
        }
        //Clear screen
        SDL_SetRenderDrawColor(global_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(global_renderer);
        switch (this->render_type)
        {
        case PLAYCHESS_INTERFACE:
        case PLAYER_LOSE_PRE_INTERFACE:
        case PLAYER_WIN_PRE_INTERFACE:
            this->playchess_manage->startRender();
            break;
        case PLAYER_WIN_INTERFACE:
        case PLAYER_LOSE_INTERFACE:
        {
            if (last_render_type != this->render_type)
            {
                MyUtils::sleep_seconds(1.5);
            }
            this->settlement_manage->startRender(this->render_type);
            break;
        }
        case MAIN_MENU_INTERFACE:
            this->main_menu_manage->startRender();
            break;
        case SELECT_PLAY_INTERFACE:
            this->select_play_manage->startRender();
            break;
        default:
            break;
        }
        last_render_type = this->render_type;
        //Update screen
        SDL_RenderPresent(global_renderer);
    }
    //Free resources and close SDL
    this->closeRender();
}

bool TopManage::initRender()
{
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        ERRORLOG("SDL could not initialize||SDL_Error: ", SDL_GetError());
        return false;
    }
    DEBUGLOG("SDL initialize success!");
    //Set texture filtering to linear
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        WARNLOG("Warning: Linear texture filtering not enabled!");
    }
    //Create window
    this->global_window = SDL_CreateWindow("五子棋", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN);
    if(global_window == nullptr)
    {
        ERRORLOG("Window could not be created||SDL_Error: ", SDL_GetError());
        return false;
    }
    DEBUGLOG("Create window success||width={}||height={}", this->width, this->height);
    //为窗口创建垂直同步渲染器
    this->global_renderer = SDL_CreateRenderer(global_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(global_renderer == nullptr)
    {
        ERRORLOG("Renderer could not be created||SDL Error={}", SDL_GetError());
        return false;
    }
    DEBUGLOG("Create renderer success!");
    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        ERRORLOG("SDL_image could not initialize||SDL_image Error:", IMG_GetError());
        return false;
    }
    DEBUGLOG("SDL_image initialize success");
    //Initialize SDL_ttf
    if(TTF_Init() == -1)
    {
        ERRORLOG("SDL_ttf could not initialize||SDL_ttf Error:", TTF_GetError());
        return false;
    }
    DEBUGLOG("SDL_ttf initialize success");
    //使用 TTF_OpenFont 加载字体。这需要输入字体文件的路径和要渲染的点尺寸
    this->gResultFont = TTF_OpenFont(this->art_ttf_path.c_str(), this->art_ttf_ptsize);
    if(gResultFont == nullptr)
    {
        ERRORLOG("Failed to load STXingkai font! SDL_ttf Error={}", TTF_GetError());
        return false;
    }
    this->normal_font = TTF_OpenFont(this->normal_ttf_path.c_str(), this->normal_ttf_ptsize);
    if(normal_font == nullptr)
    {
        ERRORLOG("Failed to load STkaiti font! SDL_ttf Error={}", TTF_GetError());
        return false;
    }
    DEBUGLOG("Create font success!");
    INFOLOG("initRender success!");
    this->loadResource();
    return true;
}

bool TopManage::loadResource()
{
    this->main_menu_manage->init(this->global_window, this->global_renderer);
    this->select_play_manage->init(this->global_window, this->global_renderer);
    this->playchess_manage->init(this->global_window, this->global_renderer, this->normal_font, this->gResultFont);
    this->settlement_manage->init(this->global_window, this->global_renderer, this->gResultFont);
    this->settlement_manage->set_font_coordinate(this->playchess_manage->get_chessboard_center_x(), this->playchess_manage->get_chessboard_center_y());
    INFOLOG("loadResource success!");
    return true;
}

void TopManage::closeRender()
{
    //Destroy windows
    SDL_DestroyWindow(global_window);
    global_window = nullptr;
    SDL_DestroyRenderer(global_renderer);
    global_renderer = nullptr;
    TTF_CloseFont(gResultFont);
    gResultFont = nullptr;

    //Quit SDL subsystems
    TTF_Quit();
    // Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void TopManage::setRendererType(const interface_kind_type& render_type)
{
    this->render_type = render_type;
    INFOLOG("setRendererType||set render_type={}", (int)render_type);
}


