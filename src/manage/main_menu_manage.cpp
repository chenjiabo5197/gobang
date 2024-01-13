#include "main_menu_manage.h"

MainMenuManage::MainMenuManage(const Config& config)
{
    this->start_game_button = new SDLButton(config, "start_game");
    this->best_scores_button = new SDLButton(config, "best_scores");
    this->exit_game_button = new SDLButton(config, "exit_game");
    this->button_interval = config.Read("main_menu_buttons_interval", 0);
    DEBUGLOG("MainMenuManage construct success||button_interval={}", this->button_interval);
}

MainMenuManage::~MainMenuManage()
{
    delete start_game_button;
    delete best_scores_button;
    delete exit_game_button;
    DEBUGLOG("~MainMenuManage success||release resource");
}

void MainMenuManage::loadResource()
{
    this->start_game_button->loadResource(this->global_window, this->global_renderer);
    this->best_scores_button->loadResource(this->global_window, this->global_renderer);
    this->exit_game_button->loadResource(this->global_window, this->global_renderer);
    INFOLOG("loadResource||load resource success");
}

void MainMenuManage::init(SDL_Window* global_window, SDL_Renderer* global_renderer)
{
    this->global_window = global_window;
    this->global_renderer = global_renderer;
    this->start_game_button->initButtonCurrentSprite();
    this->best_scores_button->initButtonCurrentSprite();
    this->exit_game_button->initButtonCurrentSprite();
    INFOLOG("init||init variable success");
}

void MainMenuManage::startRender()
{
    this->start_game_button->buttonRender(this->global_renderer);
    this->best_scores_button->buttonRender(this->global_renderer);
    this->exit_game_button->buttonRender(this->global_renderer);
    // DEBUGLOG("startRender");
}

void MainMenuManage::handleEvents(SDL_Event* event)
{
    this->start_game_button->handleButtonEvent(event);
    this->best_scores_button->handleButtonEvent(event);
    this->exit_game_button->handleButtonEvent(event);
    if (this->exit_game_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = EXIT_GAME_EVENT;
        SDL_PushEvent(&event);
        INFOLOG("handleEvents||push event=EXIT_GAME_EVENT");
    }
    if (this->start_game_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = START_GAME_EVENT;
        SDL_PushEvent(&event);
        this->start_game_button->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=START_GAME_EVENT");
    }
}





