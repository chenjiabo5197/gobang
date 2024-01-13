#include "select_play_manage.h"

SelectPlayManage::SelectPlayManage(const Config& config)
{
    this->single_player_button = new SDLButton(config, "single_player");
    this->two_players_button = new SDLButton(config, "two_players");
    this->play_internet_button = new SDLButton(config, "play_internet");
    this->normal_back_menu_button = new SDLButton(config, "normal_back_menu");
    this->button_interval = config.Read("select_play_buttons_interval", 0);
    DEBUGLOG("SelectPlayManage construct success||button_interval={}", this->button_interval);
}

SelectPlayManage::~SelectPlayManage()
{
    delete single_player_button;
    delete two_players_button;
    delete play_internet_button;
    delete normal_back_menu_button;
    DEBUGLOG("~SelectPlayManage success||release resource");
}

void SelectPlayManage::loadResource()
{
    this->single_player_button->loadResource(this->global_window, this->global_renderer);
    this->two_players_button->loadResource(this->global_window, this->global_renderer);
    this->play_internet_button->loadResource(this->global_window, this->global_renderer);
    this->normal_back_menu_button->loadResource(this->global_window, this->global_renderer);
    INFOLOG("loadResource||load resource success");
}

void SelectPlayManage::init(SDL_Window* global_window, SDL_Renderer* global_renderer)
{
    this->global_window = global_window;
    this->global_renderer = global_renderer;
    this->single_player_button->initButtonCurrentSprite();
    this->two_players_button->initButtonCurrentSprite();
    this->play_internet_button->initButtonCurrentSprite();
    this->normal_back_menu_button->initButtonCurrentSprite();
    INFOLOG("init||init variable success");
}

void SelectPlayManage::startRender()
{
    SDL_SetRenderDrawColor(this->global_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(this->global_renderer);
    this->single_player_button->buttonRender(this->global_renderer);
    this->two_players_button->buttonRender(this->global_renderer);
    this->play_internet_button->buttonRender(this->global_renderer);
    this->normal_back_menu_button->buttonRender(this->global_renderer);
    //Update screen
    SDL_RenderPresent(this->global_renderer);
    // DEBUGLOG("startRender");
}

bool SelectPlayManage::handleEvents(SDL_Event* event)
{
    this->single_player_button->handleButtonEvent(event);
    this->two_players_button->handleButtonEvent(event);
    this->play_internet_button->handleButtonEvent(event);
    this->normal_back_menu_button->handleButtonEvent(event);
    return true;
}
