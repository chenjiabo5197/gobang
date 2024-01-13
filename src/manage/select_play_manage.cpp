#include "select_play_manage.h"

SelectPlayManage::SelectPlayManage(const Config& config)
{
    this->buttons_x = config.Read("screen_width", 0) / 2;
    this->buttons_y = config.Read("screen_height", 0) / 2;
    this->button_interval = config.Read("select_play_buttons_interval", 0);
    this->select_play_buttons[0] = new SDLButton(config, "single_player", this->buttons_x, this->buttons_y-2*this->button_interval);
    this->select_play_buttons[1] = new SDLButton(config, "two_players", this->buttons_x, this->buttons_y-this->button_interval);
    this->select_play_buttons[2] = new SDLButton(config, "play_internet", this->buttons_x, this->buttons_y);
    this->select_play_buttons[3] = new SDLButton(config, "normal_back_menu", this->buttons_x, this->buttons_y+this->button_interval);
    this->array_length = sizeof(this->select_play_buttons) / sizeof(this->select_play_buttons[0]);
    DEBUGLOG("SelectPlayManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    this->button_interval, this->buttons_x, this->buttons_y, this->array_length);
}

SelectPlayManage::~SelectPlayManage()
{
    DEBUGLOG("~SelectPlayManage success||release resource");
}

void SelectPlayManage::loadResource()
{
    for (int i = 0; i < this->array_length; i++)
    {
        this->select_play_buttons[i]->loadResource(this->global_window, this->global_renderer);
    }
    INFOLOG("loadResource||load resource success");
}

void SelectPlayManage::init(SDL_Window* global_window, SDL_Renderer* global_renderer)
{
    this->global_window = global_window;
    this->global_renderer = global_renderer;
    for (int i = 0; i < this->array_length; i++)
    {
        this->select_play_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
}

void SelectPlayManage::startRender()
{
    for (int i = 0; i < this->array_length; i++)
    {
        this->select_play_buttons[i]->buttonRender(this->global_renderer);
    }
    // DEBUGLOG("startRender");
}

void SelectPlayManage::handleEvents(SDL_Event* event)
{
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
