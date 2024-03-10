/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: This is managevent.cpp file
* Copyright (c) 2023, All rights reserved
=============================================*/
#include "top_manage.h"

TopManage::TopManage(const Config& config)
{
    this->setRendererType(DEFAULT_INTERFACE);
    std::string temp;
    this->art_ttf_path = config.Read("result_resource_path", temp);
    this->art_ttf_ptsize = config.Read("result_ptsize", 0);
    this->normal_ttf_path = config.Read("normal_ttf_resource_path", temp);
    this->normal_ttf_ptsize = config.Read("normal_ttf_ptsize", 0);
    this->main_menu_manage = new MainMenuManage(config);// TODO 新建优化
    this->select_play_manage = new SelectPlayManage(config);
    this->playchess_manage = new PlaychessManage(config);
    this->settlement_manage = new SettlementManage(config);
    this->main_window = new SDLWindow(config, "main_window");
    DEBUGLOG("Manage construct success||render_type={}||art_ttf_path={}||art_ttf_ptsize={}||normal_ttf_resource_path={}||normal_ttf_ptsize={}", 
    (int)this->render_type, this->art_ttf_path, this->art_ttf_ptsize, this->normal_ttf_path, this->normal_ttf_ptsize);
}

TopManage::~TopManage()
{
    delete main_menu_manage;
    delete select_play_manage;
    delete playchess_manage;
    delete settlement_manage;
    delete main_window;
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
    SDL_Event event;
    bool quit = false;
    interface_kind_type last_render_type = DEFAULT_INTERFACE;
    //While application is running
    while(!quit)
    {
        //Handle events on queue
        while(SDL_PollEvent(&event) != 0)
        {
            //User requests quit
            if(event.type == SDL_QUIT || event.type == EXIT_GAME_EVENT)
            {
                quit = true;
            }
            else if (event.type == PLAYER_WIN_EVENT)
            {
                this->playchess_manage->handleEvents(&event);
                this->setRendererType(PLAYER_WIN_PRE_INTERFACE);
            }
            else if (event.type == PLAYER_LOSE_EVENT)
            {
                this->playchess_manage->handleEvents(&event);
                this->setRendererType(PLAYER_LOSE_PRE_INTERFACE);
            }
            else if (event.type == TWO_PLAYER_BLACK_WIN_EVENT)
            {
                this->playchess_manage->handleEvents(&event);
                this->setRendererType(BLACK_WIN_INTERFACE);
            }
            else if (event.type == TWO_PLAYER_WHITE_WIN_EVENT)
            {
                this->playchess_manage->handleEvents(&event);
                this->setRendererType(WHITE_WIN_INTERFACE);
            }
            else if (event.type == START_GAME_EVENT)
            {
                this->setRendererType(SELECT_PLAY_INTERFACE);
            }
            else if (event.type == SINGLE_PLAYER_EVENT || event.type == AGAIN_GAME_EVENT)
            {
                this->select_play_manage->handleEvents(&event);
            }
            else if (event.type == TWO_PLAYER_EVENT)
            {
                this->setRendererType(PLAYCHESS_INTERFACE);
                this->playchess_manage->handleEvents(&event);
            }
            else if (event.type == SINGLE_PLAYER_WHITE_EVENT || event.type == SINGLE_PLAYER_BLACK_EVENT)
            {
                this->setRendererType(PLAYCHESS_INTERFACE);
                this->playchess_manage->handleEvents(&event);
            }
            
            else if (event.type == BACK_MANU_EVENT)
            {
                this->setRendererType(MAIN_MENU_INTERFACE);
            }
            else if (event.type == PLAYER_WITHDRAW_EVENT)
            {
                this->playchess_manage->handleEvents(&event);
            }
            // else if (event.type == AGAIN_GAME_EVENT)
            // {
            //     this->setRendererType(PLAYCHESS_INTERFACE);
            //     this->playchess_manage->handleEvents(&event);
            // }    
            else
            {
                if(this->render_type == PLAYCHESS_INTERFACE)
                {
                    this->playchess_manage->handleEvents(&event);
                }
                else if (this->render_type == PLAYER_WIN_INTERFACE || this->render_type == PLAYER_LOSE_INTERFACE || this->render_type == BLACK_WIN_INTERFACE || this->render_type == WHITE_WIN_INTERFACE)
                {
                    this->settlement_manage->handleEvents(&event);
                }
                else if (this->render_type == MAIN_MENU_INTERFACE)
                {
                    this->main_menu_manage->handleEvents(&event);
                }
                else if (this->render_type == SELECT_PLAY_INTERFACE)
                {
                    this->select_play_manage->handleEvents(&event);
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
        SDL_SetRenderDrawColor(this->main_window->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(this->main_window->getRenderer());
        this->main_window->render_background();
        switch (this->render_type)
        {
        case PLAYCHESS_INTERFACE:
        case PLAYER_LOSE_PRE_INTERFACE:
        case PLAYER_WIN_PRE_INTERFACE:
            this->playchess_manage->startRender();
            break;
        case PLAYER_WIN_INTERFACE:
        case PLAYER_LOSE_INTERFACE:
        case BLACK_WIN_INTERFACE:
        case WHITE_WIN_INTERFACE:
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
        SDL_RenderPresent(this->main_window->getRenderer());
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
    this->art_ttf = TTF_OpenFont(this->art_ttf_path.c_str(), this->art_ttf_ptsize);
    if(art_ttf == nullptr)
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
    this->main_window->init();
    INFOLOG("initRender success!");
    this->loadResource();
    return true;
}

bool TopManage::loadResource()
{
    // TODO 优化加载，默认不用加载后面用不到的管理页面
    this->main_menu_manage->init(this->main_window);
    this->select_play_manage->init(this->main_window, this->art_ttf);
    this->playchess_manage->init(this->main_window, this->normal_font, this->art_ttf);
    this->settlement_manage->init(this->main_window, this->art_ttf);
    this->settlement_manage->set_font_coordinate(this->playchess_manage->get_chessboard_center_x(), this->playchess_manage->get_chessboard_center_y());
    INFOLOG("loadResource success!");
    return true;
}

void TopManage::closeRender()
{
    TTF_CloseFont(art_ttf);
    art_ttf = nullptr;
    TTF_CloseFont(normal_font);
    normal_font = nullptr;
    this->main_window->free();

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


