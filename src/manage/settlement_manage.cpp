#include "settlement_manage.h"

// 主窗口
extern SDLWindow* main_window; 

SettlementManage::SettlementManage(const Config& config)
{
    std::string temp;
    this->player_win_interface = new SDLTTF(config, "result_player_win");// TODO 新建优化
    this->player_lose_interface = new SDLTTF(config, "result_player_lose");
    this->black_win_interface = new SDLTTF(config, "result_black_win");
    this->white_win_interface = new SDLTTF(config, "result_white_win");
    this->buttons_x = config.Read("settlement_buttons_x", 0);
    this->buttons_y = config.Read("settlement_buttons_y", 0);
    this->button_interval = config.Read("settlement_buttons_interval", 0);
    this->settlement_buttons[0] = new SDLButton(config, "again_game", this->buttons_x, this->buttons_y-this->button_interval);
    this->settlement_buttons[1] = new SDLButton(config, "back_menu", this->buttons_x, this->buttons_y);
    this->array_length = sizeof(this->settlement_buttons) / sizeof(this->settlement_buttons[0]);
    this->interface_render_type = DEFAULT_INTERFACE;
    DEBUGLOG("SettlementManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    this->button_interval, this->buttons_x, this->buttons_y, this->array_length);
}

SettlementManage::~SettlementManage()
{
    delete player_win_interface;
    delete player_lose_interface;
    delete black_win_interface;
    delete white_win_interface;
    DEBUGLOG("~SettlementManage success||release resource");
}

void SettlementManage::init(TTF_Font* art_font)
{
    this->art_font = art_font;
    for (int i = 0; i < this->array_length; i++)
    {
        this->settlement_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
    this->player_win_interface->loadRenderText(main_window->getRenderer(), art_font);
    this->player_lose_interface->loadRenderText(main_window->getRenderer(), art_font);
    this->black_win_interface->loadRenderText(main_window->getRenderer(), art_font);
    this->white_win_interface->loadRenderText(main_window->getRenderer(), art_font);
    for (int i = 0; i < this->array_length; i++)
    {
        this->settlement_buttons[i]->loadResource(main_window->getWindow(), main_window->getRenderer());
    }
    INFOLOG("loadResource||load resource success");
}

void SettlementManage::startRender()
{
    // 根据不同页面展示不同的结果
    if (this->interface_render_type == PLAYER_LOSE_INTERFACE)
    {
        this->player_lose_interface->ttfRender(main_window->getRenderer(), font_x, font_y);
    }
    else if (this->interface_render_type == PLAYER_WIN_INTERFACE)
    {
        this->player_win_interface->ttfRender(main_window->getRenderer(), font_x, font_y);
    }
    else if (this->interface_render_type == BLACK_WIN_INTERFACE)
    {
        this->black_win_interface->ttfRender(main_window->getRenderer(), font_x, font_y, 0.35);
    }
    else if (this->interface_render_type == WHITE_WIN_INTERFACE)
    {
        this->white_win_interface->ttfRender(main_window->getRenderer(), font_x, font_y, 0.35);
    }
    for (int i = 0; i < this->array_length; i++)
    {
        this->settlement_buttons[i]->buttonRender(main_window->getRenderer());
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
        std::string msg = "AGAIN_GAME_EVENT";
        if (this->interface_render_type == PLAYER_WIN_INTERFACE || this->interface_render_type == PLAYER_LOSE_INTERFACE)
        {
            event.type = SINGLE_PLAYER_AGAIN_GAME_EVENT;
            msg = "SINGLE_PLAYER_AGAIN_GAME_EVENT";
        }
        else if (this->interface_render_type == BLACK_WIN_INTERFACE || this->interface_render_type == WHITE_WIN_INTERFACE)
        {
            event.type = TWO_PLAYER_AGAIN_GAME_EVENT;
            msg = "TWO_PLAYER_AGAIN_GAME_EVENT";
        }
        SDL_PushEvent(&event);
        this->settlement_buttons[0]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event={}", msg);
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

void SettlementManage::set_interface_render_type(const interface_kind_type& type)
{
    if(type == PLAYER_WIN_INTERFACE || type == PLAYER_LOSE_INTERFACE || type == BLACK_WIN_INTERFACE || type == WHITE_WIN_INTERFACE)
    {
        this->interface_render_type = type;
        INFOLOG("set_interface_render_type||interface_render_type={}", (int)this->interface_render_type);
    }
    else
    {
        ERRORLOG("set_interface_render_type||interface_render_type is error!");
    }
}
