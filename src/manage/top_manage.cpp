/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: This is managevent.cpp file
* Copyright (c) 2023, All rights reserved
=============================================*/
#include "top_manage.h"

// 下棋音效
extern Mix_Chunk* g_chess_down_sound;
// 按键选择音效
extern Mix_Chunk* g_select_button_sound;
// 主窗口
extern SDLWindow* g_main_window; 
// 艺术字体 行楷
extern TTF_Font* g_art_font;
// 普通字体
extern TTF_Font* g_normal_font;

TopManage::TopManage(const Config& config)
{
    this->setRendererType(DEFAULT_INTERFACE);
    std::string temp;
    m_art_ttf_path = config.Read("art_ttf_font_path", temp);
    m_art_ttf_ptsize = config.Read("art_ttf_font_ptsize", 0);
    m_normal_ttf_path = config.Read("normal_ttf_font_path", temp);
    m_normal_ttf_ptsize = config.Read("normal_ttf_font_ptsize", 0);
    m_main_menu_manage = new MainMenuManage(config);// TODO 新建优化
    m_select_play_manage = new SelectPlayManage(config);
    m_playchess_manage = new PlaychessManage(config);
    m_settlement_manage = new SettlementManage(config);
    g_main_window = new SDLWindow(config, "g_main_window");
    DEBUGLOG("Manage construct success||render_type={}||art_ttf_path={}||art_ttf_ptsize={}||normal_ttf_resource_path={}||normal_ttf_ptsize={}", 
    (int)m_render_type, m_art_ttf_path, m_art_ttf_ptsize, m_normal_ttf_path, m_normal_ttf_ptsize);
}

TopManage::~TopManage()
{
    delete m_main_menu_manage;
    delete m_select_play_manage;
    delete m_playchess_manage;
    delete m_settlement_manage;
    // delete g_main_window;
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
            switch (event.type)
            {
            case SDL_QUIT:
            case EXIT_GAME_EVENT:
                quit = true;
                break;
            case PLAYER_WIN_EVENT:
                m_playchess_manage->handleEvents(&event);
                this->setRendererType(PLAYER_WIN_PRE_INTERFACE);
                break;
            case PLAYER_LOSE_EVENT:
                m_playchess_manage->handleEvents(&event);
                this->setRendererType(PLAYER_LOSE_PRE_INTERFACE);
                break;
            case TWO_PLAYER_BLACK_WIN_EVENT:
                m_playchess_manage->handleEvents(&event);
                this->setRendererType(BLACK_WIN_INTERFACE);
                break;
            case TWO_PLAYER_WHITE_WIN_EVENT:
                m_playchess_manage->handleEvents(&event);
                this->setRendererType(WHITE_WIN_INTERFACE);
                break;
            case START_GAME_EVENT:
                this->setRendererType(SELECT_PLAY_INTERFACE);
                break;
            case SINGLE_PLAYER_EVENT:
            case SINGLE_PLAYER_AGAIN_GAME_EVENT:
                m_select_play_manage->handleEvents(&event);
                break;
            case TWO_PLAYER_EVENT:
                this->setRendererType(PLAYCHESS_INTERFACE);
                m_playchess_manage->handleEvents(&event);
                break;
            case SINGLE_PLAYER_WHITE_EVENT:
            case SINGLE_PLAYER_BLACK_EVENT:
            case TWO_PLAYER_AGAIN_GAME_EVENT:
                this->setRendererType(PLAYCHESS_INTERFACE);
                m_playchess_manage->handleEvents(&event);
                break;
            case BACK_MANU_EVENT:
                this->setRendererType(MAIN_MENU_INTERFACE);
                break;
            case PLAYER_WITHDRAW_EVENT:
                m_playchess_manage->handleEvents(&event);
                break;
            case BEST_SCORES_EVENT:
                m_main_menu_manage->handleEvents(&event);
                break;
            default:  // 其他事件，鼠标移动，点击等事件
                switch (m_render_type)
                {
                case PLAYCHESS_INTERFACE:
                    m_playchess_manage->handleEvents(&event);
                    break;
                case PLAYER_WIN_INTERFACE:
                case PLAYER_LOSE_INTERFACE:
                case BLACK_WIN_INTERFACE:
                case WHITE_WIN_INTERFACE:
                    m_settlement_manage->handleEvents(&event);
                    break;
                case MAIN_MENU_INTERFACE:
                    m_main_menu_manage->handleEvents(&event);
                    break;
                case SELECT_PLAY_INTERFACE:
                    m_select_play_manage->handleEvents(&event);
                    break;
                default:
                    break;
                }
                break;
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
        SDL_SetRenderDrawColor(g_main_window->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(g_main_window->getRenderer());
        g_main_window->renderBackground();
        switch (m_render_type)
        {
        case PLAYCHESS_INTERFACE:
        case PLAYER_LOSE_PRE_INTERFACE:
        case PLAYER_WIN_PRE_INTERFACE:
            m_playchess_manage->startRender();
            break;
        case PLAYER_WIN_INTERFACE:
        case PLAYER_LOSE_INTERFACE:
        case BLACK_WIN_INTERFACE:
        case WHITE_WIN_INTERFACE:
        {
            if (last_render_type != m_render_type)
            {
                sleepSeconds(1.5);
            }
            m_settlement_manage->startRender();
            break;
        }
        case MAIN_MENU_INTERFACE:
            m_main_menu_manage->startRender();
            break;
        case SELECT_PLAY_INTERFACE:
            m_select_play_manage->startRender();
            break;
        default:
            break;
        }
        last_render_type = m_render_type;
        //Update screen
        SDL_RenderPresent(g_main_window->getRenderer());
    }
    //Free resources and close SDL
    this->closeRender();
}

bool TopManage::initRender()
{
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0)
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
    //Initialize SDL_mixer
    /*
    Mix_OpenAudio,第一个参数设置声音频率，44100 是标准频率，适用于大多数系统。
    第二个参数决定采样格式，这里使用默认格式。第三个参数是硬件声道数，这里使用 2 个声道作为立体声声道。
    最后一个参数是采样大小，它决定了播放声音时使用的音块大小。这里使用的是 2048 字节（又称 2 千字节），但要尽量减少播放声音时的延迟，可能需要尝试使用这个值。
    */
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        ERRORLOG("SDL_mixer could not initialize! SDL_mixer Error={}", Mix_GetError());
        return false;
    }
    DEBUGLOG("SDL_mixer initialize success");
    INFOLOG("initRender success!");
    if(!this->loadResource())
    {
        return false;
    }
    return true;
}

bool TopManage::loadResource()
{
    //使用 TTF_OpenFont 加载字体。这需要输入字体文件的路径和要渲染的点尺寸
    g_art_font = TTF_OpenFont(m_art_ttf_path.c_str(), m_art_ttf_ptsize);
    if(g_art_font == nullptr)
    {
        ERRORLOG("Failed to load STXingkai font! SDL_ttf Error={}", TTF_GetError());
        return false;
    }
    g_normal_font = TTF_OpenFont(m_normal_ttf_path.c_str(), m_normal_ttf_ptsize);
    if(g_normal_font == nullptr)
    {
        ERRORLOG("Failed to load STkaiti font! SDL_ttf Error={}", TTF_GetError());
        return false;
    }
    DEBUGLOG("Create font success!");
    g_chess_down_sound = Mix_LoadWAV("resources/chess_down.mp3");
    if(g_chess_down_sound == nullptr)
    {
        ERRORLOG("Failed to load g_chess_down_sound sound effect! SDL_mixer Error={}", Mix_GetError());
        return false;
    }
    DEBUGLOG("load g_chess_down_sound success!");
    g_select_button_sound = Mix_LoadWAV("resources/select_button.wav");
    if(g_select_button_sound == nullptr)
    {
        ERRORLOG("Failed to load g_select_button_sound sound effect! SDL_mixer Error={}", Mix_GetError());
        return false;
    }
    DEBUGLOG("load g_select_button_sound success!");
    // TODO 优化加载，默认不用加载后面用不到的管理页面
    g_main_window->init();
    m_main_menu_manage->init();
    m_select_play_manage->init();
    m_playchess_manage->init();
    m_settlement_manage->init();
    m_settlement_manage->setFontCoordinate(m_playchess_manage->getChessboardCenterX(), m_playchess_manage->getChessboardCenterY());
    INFOLOG("loadResource success!");
    return true;
}

void TopManage::closeRender()
{
    TTF_CloseFont(g_art_font);
    g_art_font = nullptr;
    TTF_CloseFont(g_normal_font);
    g_normal_font = nullptr;
    g_main_window->free();
    Mix_FreeChunk(g_chess_down_sound);
    g_chess_down_sound = nullptr;
    g_select_button_sound = nullptr;
    g_main_window = nullptr;

    //Quit SDL subsystems
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    INFOLOG("closeRender||close render, quit sdl");
}

void TopManage::setRendererType(const interface_kind_type& render_type)
{
    m_render_type = render_type;
    if(render_type == PLAYER_WIN_INTERFACE || render_type == PLAYER_LOSE_INTERFACE || render_type == BLACK_WIN_INTERFACE || render_type == WHITE_WIN_INTERFACE)
    {
        m_settlement_manage->setInterfaceRenderType(render_type);
    }
    INFOLOG("setRendererType||set render_type={}", (int)render_type);
}


