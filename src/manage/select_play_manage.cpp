#include "select_play_manage.h"

SelectPlayManage::SelectPlayManage(const Config& config)
{
    this->buttons_x = config.Read("main_window_screen_width", 0) / 2;
    this->buttons_y = config.Read("main_window_screen_height", 0) / 2;
    this->button_interval = config.Read("select_play_buttons_interval", 0);
    this->select_play_buttons[0] = new SDLButton(config, "single_player", this->buttons_x, this->buttons_y-2*this->button_interval);
    this->select_play_buttons[1] = new SDLButton(config, "two_players", this->buttons_x, this->buttons_y-this->button_interval);
    this->select_play_buttons[2] = new SDLButton(config, "play_internet", this->buttons_x, this->buttons_y);
    this->select_play_buttons[3] = new SDLButton(config, "normal_back_menu", this->buttons_x, this->buttons_y+this->button_interval);
    this->array_length = sizeof(this->select_play_buttons) / sizeof(this->select_play_buttons[0]);
    this->select_chess_color_window = new SDLWindow(config, "select_chess_color");
    this->white_color_chess = new Chess(config, "symbol_white_chess");
    this->black_color_chess = new Chess(config, "symbol_black_chess");
    DEBUGLOG("SelectPlayManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    this->button_interval, this->buttons_x, this->buttons_y, this->array_length);
}

SelectPlayManage::~SelectPlayManage()
{
    delete select_chess_color_window;
    delete white_color_chess;
    delete black_color_chess;
    DEBUGLOG("~SelectPlayManage success||release resource");
}

void SelectPlayManage::init(SDLWindow* sdl_window)
{
    this->selete_main_window = sdl_window;
    for (int i = 0; i < this->array_length; i++)
    {
        this->select_play_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
    for (int i = 0; i < this->array_length; i++)
    {
        this->select_play_buttons[i]->loadResource(this->selete_main_window->getWindow(), this->selete_main_window->getRenderer());
    }
    INFOLOG("loadResource||load resource success");
}

void SelectPlayManage::startRender()
{
    for (int i = 0; i < this->array_length; i++)
    {
        this->select_play_buttons[i]->buttonRender(this->selete_main_window->getRenderer());
    }
    // DEBUGLOG("startRender");
}

void SelectPlayManage::handleEvents(SDL_Event* event)
{
    if (event->type == SINGLE_PLAYER_EVENT || event->type == AGAIN_GAME_EVENT)
    {
        this->singlePlaySelectChess();
    }
    for (int i = 0; i < this->array_length; i++)
    {
        this->select_play_buttons[i]->handleButtonEvent(event);
    }
    if (this->select_play_buttons[0]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = SINGLE_PLAYER_EVENT;
        SDL_PushEvent(&event);
        this->select_play_buttons[0]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=SINGLE_PLAYER_EVENT");
    }
    if (this->select_play_buttons[1]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = TWO_PLAYER_EVENT;
        SDL_PushEvent(&event);
        this->select_play_buttons[1]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=TWO_PLAYER_EVENT");
    }
    if (this->select_play_buttons[2]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = PLAY_INTERNET_EVENT;
        SDL_PushEvent(&event);
        this->select_play_buttons[2]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=PLAY_INTERNET_EVENT");
    }
    if (this->select_play_buttons[3]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = BACK_MANU_EVENT;
        SDL_PushEvent(&event);
        this->select_play_buttons[3]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=BACK_MANU_EVENT");
    }
}

void SelectPlayManage::singlePlaySelectChess()
{
    this->select_chess_color_window->init();
    this->white_color_chess->init(this->select_chess_color_window);
    this->black_color_chess->init(this->select_chess_color_window);
    //Hack to get window to stay up
    SDL_Event event;
    bool quit = false;
    // 标志鼠标是否在选择先后手弹窗内
    bool mouse_focus = false;
    //While application is running
    while(!quit)
    {
        //Handle events on queue
        while(SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_WINDOWEVENT && event.window.windowID == this->select_chess_color_window->getWindowId())
            {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    quit = true;
                }
                switch (event.window.event)
                {
                //SDL_WINDOWEVENT_ENTER用于处理鼠标移入窗口的情况
                case SDL_WINDOWEVENT_ENTER:
                DEBUGLOG("SDL_WINDOWEVENT_ENTER");
                    mouse_focus = true;
                    break;
                
                //SDL_WINDOWEVENT_LEAVE 于处理鼠标移出窗口的情况
                case SDL_WINDOWEVENT_LEAVE:
                DEBUGLOG("SDL_WINDOWEVENT_LEAVE");
                    mouse_focus = false;
                    break;
                default:
                    break;
                }
            }         
            this->handleWindowEvents(&event, mouse_focus);
        }
        //Clear screen
        SDL_SetRenderDrawColor(this->select_chess_color_window->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(this->select_chess_color_window->getRenderer());
        this->white_color_chess->chessRender(this->select_chess_color_window->getWidth()/2-60, this->select_chess_color_window->getHeight()/2-10, 0.8);
        this->black_color_chess->chessRender(this->select_chess_color_window->getWidth()/2+60, this->select_chess_color_window->getHeight()/2-10, 0.8);
        SDL_RenderPresent(this->select_chess_color_window->getRenderer());
    }
    //Free resources and close SDL
    this->select_chess_color_window->free();
}

void SelectPlayManage::handleWindowEvents(SDL_Event* event, bool mouse_focus)
{
    if (event->type == SDL_MOUSEMOTION && mouse_focus)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        DEBUGLOG("type2={}||x={}||y={}", (int)event->type, x, y);
    }
}
