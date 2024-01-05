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
    this->width = config.Read("screen_width", 0);
    this->height = config.Read("screen_height", 0);
    this->render_type = DEFAULT_INTERFACE;
    this->chessboard = new Chessboard(config);
    this->machine = new Machine(this->chessboard);
    DEBUGLOG("Manage construct success||width={}||height={}||render_type={}", this->width, this->height, (int)this->render_type);
}

Manage::~Manage()
{
    delete chessboard;
    delete machine;
    DEBUGLOG("~Manage success, release chessboard");
}

// Machine类的友元函数
int go(void* data)
{
	Machine* machine = (Machine *)(data);
    SDL_AtomicLock(&player_machine_lock);
    ChessPos pos = machine->think();
    // 程序休眠1s，假装在思考
    MyUtils::sleep_seconds(1);
	// mciSendString("play res/chess_down.mp3", 0, 0, 0);
	machine->chessboard->chessDown(pos, CHESS_WHITE);
    machine->chessboard->checkOver();
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
    }
    else
    {
        //Hack to get window to stay up
        SDL_Event e;
        bool quit = false;
        SDL_Thread* machine_thread = nullptr;
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
                else
                {
                    if(this->handleMouseClick(&e))
                    {
                        machine_thread = SDL_CreateThread(go, "machine player", this->machine);
                    }
                }
            }
            //Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            switch (this->render_type)
            {
            case PLAYCHESS_INTERFACE:
                this->chessboard->render(gWindow, gRenderer);
                break;
            default:
                break;
            }
            //Update screen
            SDL_RenderPresent(gRenderer);
        }
        SDL_WaitThread(machine_thread, nullptr);
        //Free resources and close SDL
        this->closeRender();
    }
}

bool Manage::initRender()
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

void Manage::closeRender()
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

bool Manage::handleMouseClick(SDL_Event* e)
{
    SDL_AtomicLock(&player_machine_lock);
    if (e->type == SDL_MOUSEBUTTONDOWN)  // 鼠标点击事件
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
            this->chessboard->checkOver();
        }
        this->chessboard->set_player_flag_type(MACHINE_PLAYER);
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

