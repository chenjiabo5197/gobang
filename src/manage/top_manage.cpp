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
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == PLAYER_WIN_EVENT)
            {
                this->setRendererType(PLAYER_WIN_PRE_INTERFACE);
                break;
            }
            else if (e.type == PLAYER_LOSE_EVENT)
            {
                this->setRendererType(PLAYER_LOSE_PRE_INTERFACE);
                break;
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
        // 单人游戏结算界面
        if(this->render_type == PLAYER_WIN_INTERFACE || this->render_type == PLAYER_LOSE_INTERFACE)
        {
            if (this->settlement_manage->again_game_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
            {
                this->setRendererType(PLAYCHESS_INTERFACE);
                this->settlement_manage->again_game_button->initButtonCurrentSprite();
                this->playchess_manage->chessboard->initChessMap();
            }
            if (this->settlement_manage->back_menu_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
            {
                this->setRendererType(MAIN_MENU_INTERFACE);
                this->settlement_manage->back_menu_button->initButtonCurrentSprite();
                this->playchess_manage->chessboard->initChessMap();
            }
        }
        // 下棋界面
        if(this->render_type == PLAYCHESS_INTERFACE)
        {
            if (this->playchess_manage->chessboard->get_player_flag_type() == SINGLE_PLAYER && this->playchess_manage->withdraw_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
            {
                this->playchess_manage->withdraw_button->initButtonCurrentSprite();
                if(this->playchess_manage->chessboard->is_can_withdraw())
                {
                    this->playchess_manage->chessboard->set_chessboard_withdraw();
                }
            }
            if (this->playchess_manage->back_menu_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
            {
                this->setRendererType(MAIN_MENU_INTERFACE);
                this->playchess_manage->back_menu_button->initButtonCurrentSprite();
                this->playchess_manage->chessboard->initChessMap();
            }
        }
        // 主菜单界面
        if(this->render_type == MAIN_MENU_INTERFACE)
        {
            if (this->main_menu_manage->exit_game_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
            {
                quit = true;
            }
            if (this->main_menu_manage->start_game_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
            {
                this->setRendererType(SELECT_PLAY_INTERFACE);
                this->main_menu_manage->start_game_button->initButtonCurrentSprite();
            }
        }
        // 选择游戏方式界面
        if (this->render_type == SELECT_PLAY_INTERFACE)
        {
            if (this->select_play_manage->single_player_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
            {
                this->setRendererType(PLAYCHESS_INTERFACE);
                this->select_play_manage->single_player_button->initButtonCurrentSprite();
                this->playchess_manage->chessboard->set_player_flag_type(SINGLE_PLAYER);
            }
            if (this->select_play_manage->normal_back_menu_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
            {
                this->setRendererType(MAIN_MENU_INTERFACE);
                this->select_play_manage->normal_back_menu_button->initButtonCurrentSprite();
            }
        }
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
    INFOLOG("initRender success!");
    this->main_menu_manage->init(global_window, global_renderer);
    this->select_play_manage->init(global_window, global_renderer);
    this->playchess_manage->init(global_window, global_renderer);
    this->loadResource();
    return true;
}

bool TopManage::loadResource()
{
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
    this->playchess_manage->chessboard->setTTF(this->normal_font);
    this->main_menu_manage->loadResource();
    this->select_play_manage->loadResource();
    this->playchess_manage->loadResource();
    this->settlement_manage->init(global_window, global_renderer, this->gResultFont, this->playchess_manage->chessboard->get_chessboard_center_x(), this->playchess_manage->chessboard->get_chessboard_center_y());
    this->settlement_manage->loadResource();
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

int TopManage::getScreenCenterX()
{
    int screen_center_x = this->width / 2;
    INFOLOG("getScreenCenterX||screen_center_x={}", screen_center_x);
    return screen_center_x;
}

int TopManage::getScreenCenterY()
{
    int screen_center_y = this->height / 2;
    INFOLOG("getScreenCenterY||screen_center_y={}", screen_center_y);
    return screen_center_y;
}

