#include "settlement_manage.h"

SettlementManage::SettlementManage(const Config& config)
{
    std::string temp;
    this->player_win_interface = new SDLTTF(config, "ttf_result_player_win");
    this->player_lose_interface = new SDLTTF(config, "ttf_result_player_lose");
    this->again_game_button = new SDLButton(config, "again_game");
    this->back_menu_button = new SDLButton(config, "back_menu");
    this->button_interval = config.Read("select_play_buttons_interval", 0);
    DEBUGLOG("SelectPlayManage construct success||button_interval={}", this->button_interval);
}

SettlementManage::~SettlementManage()
{
    delete player_win_interface;
    delete player_lose_interface;
    delete again_game_button;
    delete back_menu_button;
    DEBUGLOG("~SelectPlayManage success||release resource");
}

void SettlementManage::loadResource()
{
    this->player_win_interface->loadRenderText(global_renderer, art_font);
    this->player_lose_interface->loadRenderText(global_renderer, art_font);
    this->again_game_button->loadResource(this->global_window, this->global_renderer);
    this->back_menu_button->loadResource(this->global_window, this->global_renderer);
    INFOLOG("loadResource||load resource success");
}

void SettlementManage::init(SDL_Window* global_window, SDL_Renderer* global_renderer, TTF_Font* art_font, const int& x, const int& y)
{
    this->global_window = global_window;
    this->global_renderer = global_renderer;
    this->art_font = art_font;
    this->font_x = x;
    this->font_y = y;
    this->again_game_button->initButtonCurrentSprite();
    this->back_menu_button->initButtonCurrentSprite();
    INFOLOG("init||init variable success");
}

void SettlementManage::startRender(const interface_kind_type& type)
{
    if (type == PLAYER_LOSE_INTERFACE)
    {
        this->player_lose_interface->ttfRender(global_renderer, font_x, font_y);
    }
    else if (type == PLAYER_WIN_INTERFACE)
    {
        this->player_win_interface->ttfRender(global_renderer, font_x, font_y);
    }
    this->again_game_button->buttonRender(this->global_renderer);
    this->back_menu_button->buttonRender(this->global_renderer);
    // DEBUGLOG("startRender");
}

void SettlementManage::handleEvents(SDL_Event* event)
{
    this->again_game_button->handleButtonEvent(event);
    this->back_menu_button->handleButtonEvent(event);
    if (this->again_game_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = AGAIN_GAME_EVENT;
        SDL_PushEvent(&event);
        this->again_game_button->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=AGAIN_GAME_EVENT");
    }
    if (this->back_menu_button->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = BACK_MANU_EVENT;
        SDL_PushEvent(&event);
        this->back_menu_button->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=BACK_MANU_EVENT");
    }
}
