#include "settlement_manage.h"

SettlementManage::SettlementManage(const Config& config)
{
    std::string temp;
    this->player_win_interface = new SDLTTF(config, "ttf_result_player_win");
    this->player_lose_interface = new SDLTTF(config, "ttf_result_player_lose");
    this->buttons_x = config.Read("settlement_buttons_x", 0);
    this->buttons_y = config.Read("settlement_buttons_y", 0);
    this->button_interval = config.Read("settlement_buttons_interval", 0);
    this->settlement_buttons[0] = new SDLButton(config, "again_game", this->buttons_x, this->buttons_y-this->button_interval);
    this->settlement_buttons[1] = new SDLButton(config, "back_menu", this->buttons_x, this->buttons_y);
    this->array_length = sizeof(this->settlement_buttons) / sizeof(this->settlement_buttons[0]);
    DEBUGLOG("SettlementManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    this->button_interval, this->buttons_x, this->buttons_y, this->array_length);
}

SettlementManage::~SettlementManage()
{
    delete player_win_interface;
    delete player_lose_interface;
    DEBUGLOG("~SettlementManage success||release resource");
}

void SettlementManage::init(SDLWindow* sdl_window, TTF_Font* art_font)
{
    // this->global_window = global_window;
    // this->this->settlement_main_window->getRenderer() = this->settlement_main_window->getRenderer();
    this->settlement_main_window = sdl_window;
    this->art_font = art_font;
    for (int i = 0; i < this->array_length; i++)
    {
        this->settlement_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
    this->player_win_interface->loadRenderText(this->settlement_main_window->getRenderer(), art_font);
    this->player_lose_interface->loadRenderText(this->settlement_main_window->getRenderer(), art_font);
    for (int i = 0; i < this->array_length; i++)
    {
        this->settlement_buttons[i]->loadResource(this->settlement_main_window->getWindow(), this->settlement_main_window->getRenderer());
    }
    INFOLOG("loadResource||load resource success");
}

void SettlementManage::startRender(const interface_kind_type& type)
{
    if (type == PLAYER_LOSE_INTERFACE)
    {
        this->player_lose_interface->ttfRender(this->settlement_main_window->getRenderer(), font_x, font_y);
    }
    else if (type == PLAYER_WIN_INTERFACE)
    {
        this->player_win_interface->ttfRender(this->settlement_main_window->getRenderer(), font_x, font_y);
    }
    for (int i = 0; i < this->array_length; i++)
    {
        this->settlement_buttons[i]->buttonRender(this->settlement_main_window->getRenderer());
    }
    // DEBUGLOG("startRender");
}

void SettlementManage::handleEvents(SDL_Event* event)
{
    for (int i = 0; i < this->array_length; i++)
    {
        this->settlement_buttons[i]->handleButtonEvent(event);
    }
    if (this->settlement_buttons[0]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = AGAIN_GAME_EVENT;
        SDL_PushEvent(&event);
        this->settlement_buttons[0]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=AGAIN_GAME_EVENT");
    }
    if (this->settlement_buttons[1]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = BACK_MANU_EVENT;
        SDL_PushEvent(&event);
        this->settlement_buttons[1]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=BACK_MANU_EVENT");
    }
}

void SettlementManage::set_font_coordinate(const int& x, const int& y)
{
    this->font_x = x;
    this->font_y = y;
    INFOLOG("set_font_coordinate||font_x={}||font_y={}", this->font_x, this->font_y);
}
