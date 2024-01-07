/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2024-1-3
* Description: This is manage.cpp file
* Copyright (c) 2023, All rights reserved
=============================================*/
#include "manage.h"

//多线程锁
SDL_SpinLock player_machine_lock = 0;

Manage::Manage(const Config& config)
{
    this->render_type = DEFAULT_INTERFACE;
    this->chessboard = new Chessboard(config);
    this->machine = new Machine(this->chessboard);
    this->player_win_interface = new TTFResultInterface(config, "ttf_result_player_win");
    this->player_lose_interface = new TTFResultInterface(config, "ttf_result_player_lose");
    this->single_player = new Player(this->chessboard, "single_player", CHESS_BLACK);
    this->start_game_button = new SDLButton(config, "start_game");
    this->again_game_button = new SDLButton(config, "again_game");
    this->back_menu_button = new SDLButton(config, "back_menu");
    this->best_scores_button = new SDLButton(config, "best_scores");
    this->exit_game_button = new SDLButton(config, "exit_game");
    this->play_internet_button = new SDLButton(config, "play_internet");
    this->single_player_button = new SDLButton(config, "single_player");
    this->two_players_button = new SDLButton(config, "two_players");
    this->withdraw_button = new SDLButton(config, "withdraw");
    std::string temp;
    this->width = config.Read("screen_width", 0);
    this->height = config.Read("screen_height", 0);
    this->ttf_result_path = config.Read("ttf_result_resource_path", temp);
    this->ttf_result_ptsize = config.Read("ttf_result_ptsize", 0);
    this->chessboard_x = this->chessboard->get_chessboard_center_x();
    this->chessboard_y = this->chessboard->get_chessboard_center_y();
    DEBUGLOG("Manage construct success||width={}||height={}||render_type={}||ttf_result_path={}||chessboard_x={}||chessboard_y={}||ttf_result_ptsize={}", 
    this->width, this->height, (int)this->render_type, this->ttf_result_path, this->chessboard_x, this->chessboard_y, this->ttf_result_ptsize);
}

Manage::~Manage()
{
    delete chessboard;
    delete machine;
    delete single_player;
    delete player_win_interface;
    delete player_lose_interface;
    delete start_game_button;
    delete again_game_button;
    delete back_menu_button;
    delete best_scores_button;
    delete exit_game_button;
    delete play_internet_button;
    delete single_player_button;
    delete two_players_button;
    delete withdraw_button;
    DEBUGLOG("~Manage success||release resource");
}

// Machine类的友元函数
int machineChessDown(void* data)
{
	Machine* machine = (Machine *)(data);
    ChessPos pos = machine->think();
    // 程序休眠1s，假装在思考
    MyUtils::sleep_seconds(1);
	// mciSendString("play res/chess_down.mp3", 0, 0, 0);
	machine->chessboard->chessDown(pos, CHESS_WHITE);
    if(machine->chessboard->checkOver())
    {
        SDL_Event event;
        event.type = PLAYER_LOSE_EVENT;
        SDL_PushEvent(&event);
    }
    machine->chessboard->set_player_flag_type(SINGLE_PLAYER);
    SDL_AtomicUnlock(&player_machine_lock);
    DEBUGLOG("Machine::go||Machine chess down success");
    return 0;
}

void Manage::start()
{
    this->render_type = PLAYCHESS_INTERFACE;
    this->chessboard->set_player_flag_type(SINGLE_PLAYER);
    //Start up SDL and create window
    if(!this->initRender())
    {
        ERRORLOG("Failed to initialize!");
        return;
    }
    //Hack to get window to stay up
    SDL_Event e;
    bool quit = false;
    SDL_Thread* machine_thread = nullptr;
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
                    if(this->handleMouseClick(&e) && this->chessboard->get_player_flag_type() == MACHINE_PLAYER)
                    {
                        machine_thread = SDL_CreateThread(machineChessDown, "machine player", this->machine);
                    }
                    this->back_menu_button->handleButtonEvent(&e);
                    this->withdraw_button->handleButtonEvent(&e);
                }
                else if (this->render_type == PLAYER_WIN_INTERFACE || this->render_type == PLAYER_LOSE_INTERFACE)
                {
                    this->back_menu_button->handleButtonEvent(&e);
                    this->again_game_button->handleButtonEvent(&e);
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
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        switch (this->render_type)
        {
        case PLAYCHESS_INTERFACE:
            this->chessboard->render(gWindow, gRenderer);
            this->withdraw_button->buttonRender(gRenderer);
            this->back_menu_button->buttonRender(gRenderer);
            break;
        case PLAYER_LOSE_PRE_INTERFACE:
            this->chessboard->render(gWindow, gRenderer);
            break;
        case PLAYER_WIN_PRE_INTERFACE:
            this->chessboard->render(gWindow, gRenderer);
            break;
        case PLAYER_WIN_INTERFACE:
        {
            if (last_render_type != this->render_type)
            {
                MyUtils::sleep_seconds(1.5);
            }
            this->player_win_interface->ttfRender(gRenderer, chessboard_x, chessboard_y);
            this->back_menu_button->buttonRender(gRenderer);
            this->again_game_button->buttonRender(gRenderer);
            break;
        }
        case PLAYER_LOSE_INTERFACE:
        {
            if (last_render_type != this->render_type)
            {
                MyUtils::sleep_seconds(1.5);
            }
            this->player_lose_interface->ttfRender(gRenderer, chessboard_x, chessboard_y);
            this->back_menu_button->buttonRender(gRenderer);
            this->again_game_button->buttonRender(gRenderer);
            break;
        }
        default:
            break;
        }
        last_render_type = this->render_type;
        //Update screen
        SDL_RenderPresent(gRenderer);
    }
    SDL_WaitThread(machine_thread, nullptr);
    //Free resources and close SDL
    this->closeRender();
}

bool Manage::initRender()
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
    this->gWindow = SDL_CreateWindow("五子棋", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN);
    if(gWindow == nullptr)
    {
        ERRORLOG("Window could not be created||SDL_Error: ", SDL_GetError());
        return false;
    }
    DEBUGLOG("Create window success||width={}||height={}", this->width, this->height);
    //为窗口创建垂直同步渲染器
    this->gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(gRenderer == nullptr)
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
    this->loadResource();
    return true;
}

bool Manage::loadResource()
{
    //使用 TTF_OpenFont 加载字体。这需要输入字体文件的路径和要渲染的点尺寸
    this->gResultFont = TTF_OpenFont(this->ttf_result_path.c_str(), this->ttf_result_ptsize);
    if(gResultFont == nullptr)
    {
        ERRORLOG("Failed to load STXingkai font! SDL_ttf Error={}", TTF_GetError());
        return false;
    }
    DEBUGLOG("Create font success!");
    this->player_win_interface->loadRenderText(gRenderer, gResultFont);
    this->player_lose_interface->loadRenderText(gRenderer, gResultFont);
    this->again_game_button->loadResource(gWindow, gRenderer);
    this->withdraw_button->loadResource(gWindow, gRenderer);
    this->back_menu_button->loadResource(gWindow, gRenderer);
    INFOLOG("loadResource success!");
    return true;
}

void Manage::closeRender()
{
    //Destroy windows
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;
    TTF_CloseFont(gResultFont);
    gResultFont = nullptr;

    //Quit SDL subsystems
    TTF_Quit();
    // Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Manage::handleMouseClick(SDL_Event* e)
{
    SDL_AtomicLock(&player_machine_lock);
    if (e->type == SDL_MOUSEBUTTONDOWN && this->chessboard->get_player_flag_type() == SINGLE_PLAYER)  // 鼠标点击事件
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
            this->single_player->addChessNum();
            if(this->chessboard->checkOver())
            {
                SDL_Event event;
                event.type = PLAYER_WIN_EVENT;
                SDL_PushEvent(&event);
            }
            this->chessboard->set_player_flag_type(MACHINE_PLAYER);
        }
        SDL_AtomicUnlock(&player_machine_lock);
        return is_valid_click;
    }
    SDL_AtomicUnlock(&player_machine_lock);
    return false;
}

void Manage::setRendererType(const interface_kind_type& render_type)
{
    this->render_type = render_type;
    INFOLOG("setRendererType||set render_type={}", (int)render_type);
}

