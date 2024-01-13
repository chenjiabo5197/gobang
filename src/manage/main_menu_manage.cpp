#include "main_menu_manage.h"

MainMenuManage::MainMenuManage(const Config& config)
{
    this->buttons_x = config.Read("screen_width", 0) / 2;
    this->buttons_y = config.Read("screen_height", 0) / 2;
    this->button_interval = config.Read("main_menu_buttons_interval", 0);
    this->main_menu_buttons[0] = new SDLButton(config, "start_game", this->buttons_x, this->buttons_y-this->button_interval);
    this->main_menu_buttons[1] = new SDLButton(config, "best_scores", this->buttons_x, this->buttons_y);
    this->main_menu_buttons[2] = new SDLButton(config, "exit_game", this->buttons_x, this->buttons_y+this->button_interval);
    this->array_length = sizeof(this->main_menu_buttons) / sizeof(this->main_menu_buttons[0]);
    DEBUGLOG("MainMenuManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    this->button_interval, this->buttons_x, this->buttons_y, this->array_length);
}

MainMenuManage::~MainMenuManage()
{
    DEBUGLOG("~MainMenuManage success||release resource");
}

void MainMenuManage::loadResource()
{
    for (int i = 0; i < this->array_length; i++)
    {
        this->main_menu_buttons[i]->loadResource(this->global_window, this->global_renderer);
    }
    INFOLOG("loadResource||load resource success");
}

void MainMenuManage::init(SDL_Window* global_window, SDL_Renderer* global_renderer)
{
    this->global_window = global_window;
    this->global_renderer = global_renderer;
    for (int i = 0; i < this->array_length; i++)
    {
        this->main_menu_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
}

void MainMenuManage::startRender()
{
    for (int i = 0; i < this->array_length; i++)
    {
        this->main_menu_buttons[i]->buttonRender(this->global_renderer);
    }
    // DEBUGLOG("startRender");
}

void MainMenuManage::handleEvents(SDL_Event* event)
{
    for (int i = 0; i < this->array_length; i++)
    {
        this->main_menu_buttons[i]->handleButtonEvent(event);
    }
    if (this->main_menu_buttons[0]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = START_GAME_EVENT;
        SDL_PushEvent(&event);
        this->main_menu_buttons[0]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=START_GAME_EVENT");
    }
    if (this->main_menu_buttons[1]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = BEST_SCORES_EVENT;
        SDL_PushEvent(&event);
        this->main_menu_buttons[1]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=BEST_SCORES_EVENT");
    }
    if (this->main_menu_buttons[2]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = EXIT_GAME_EVENT;
        SDL_PushEvent(&event);
        INFOLOG("handleEvents||push event=EXIT_GAME_EVENT");
    }
}





