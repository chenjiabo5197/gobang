#include "settlement_manage.h"

// 主窗口
extern SDLWindow* g_main_window; 
// 艺术字体 行楷
extern TTF_Font* g_art_font;
// 普通字体
extern TTF_Font* g_normal_font;

SettlementManage::SettlementManage(const Config& config)
{
    std::string temp;
    m_player_win_interface = new SDLTTF(config, "result_player_win");// TODO 新建优化
    m_player_lose_interface = new SDLTTF(config, "result_player_lose");
    m_black_win_interface = new SDLTTF(config, "result_black_win");
    m_white_win_interface = new SDLTTF(config, "result_white_win");
    m_buttons_x = config.Read("settlement_buttons_x", 0);
    m_buttons_y = config.Read("settlement_buttons_y", 0);
    m_button_interval = config.Read("settlement_buttons_interval", 0);
    m_settlement_buttons[0] = new SDLButton(config, "again_game", m_buttons_x, m_buttons_y-m_button_interval);
    m_settlement_buttons[1] = new SDLButton(config, "back_menu", m_buttons_x, m_buttons_y);
    m_array_length = sizeof(m_settlement_buttons) / sizeof(m_settlement_buttons[0]);
    m_interface_render_type = DEFAULT_INTERFACE;
    DEBUGLOG("SettlementManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    m_button_interval, m_buttons_x, m_buttons_y, m_array_length);
}

SettlementManage::~SettlementManage()
{
    delete m_player_win_interface;
    delete m_player_lose_interface;
    delete m_black_win_interface;
    delete m_white_win_interface;
    DEBUGLOG("~SettlementManage success||release resource");
}

void SettlementManage::init()
{
    for (int i = 0; i < m_array_length; i++)
    {
        m_settlement_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
    m_player_win_interface->loadRenderText(g_main_window->getRenderer(), g_art_font);
    m_player_lose_interface->loadRenderText(g_main_window->getRenderer(), g_art_font);
    m_black_win_interface->loadRenderText(g_main_window->getRenderer(), g_art_font);
    m_white_win_interface->loadRenderText(g_main_window->getRenderer(), g_art_font);
    for (int i = 0; i < m_array_length; i++)
    {
        m_settlement_buttons[i]->loadResource(g_main_window->getWindow(), g_main_window->getRenderer());
    }
    INFOLOG("loadResource||load resource success");
}

void SettlementManage::startRender()
{
    // 根据不同页面展示不同的结果
    switch (m_interface_render_type)
    {
    case PLAYER_LOSE_INTERFACE:
        m_player_lose_interface->ttfRender(g_main_window->getRenderer(), m_font_x, m_font_y);
        break;
    case PLAYER_WIN_INTERFACE:
        m_player_win_interface->ttfRender(g_main_window->getRenderer(), m_font_x, m_font_y);
        break;
    case BLACK_WIN_INTERFACE:
        m_black_win_interface->ttfRender(g_main_window->getRenderer(), m_font_x, m_font_y, 0.35);
        break;
    case WHITE_WIN_INTERFACE:
        m_white_win_interface->ttfRender(g_main_window->getRenderer(), m_font_x, m_font_y, 0.35);
        break;
    default:
        ERRORLOG("startRender||unknown m_interface_render_type");
        break;
    }
    for (int i = 0; i < m_array_length; i++)
    {
        m_settlement_buttons[i]->buttonRender(g_main_window->getRenderer());
    }
    // DEBUGLOG("startRender");
}

void SettlementManage::handleEvents(SDL_Event* event)
{
    for (int i = 0; i < m_array_length; i++)
    {
        m_settlement_buttons[i]->handleButtonEvent(event);
    }
    if (m_settlement_buttons[0]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        std::string msg = "AGAIN_GAME_EVENT";
        if (m_interface_render_type == PLAYER_WIN_INTERFACE || m_interface_render_type == PLAYER_LOSE_INTERFACE)
        {
            event.type = SINGLE_PLAYER_AGAIN_GAME_EVENT;
            msg = "SINGLE_PLAYER_AGAIN_GAME_EVENT";
        }
        else if (m_interface_render_type == BLACK_WIN_INTERFACE || m_interface_render_type == WHITE_WIN_INTERFACE)
        {
            event.type = TWO_PLAYER_AGAIN_GAME_EVENT;
            msg = "TWO_PLAYER_AGAIN_GAME_EVENT";
        }
        SDL_PushEvent(&event);
        m_settlement_buttons[0]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event={}", msg);
    }
    if (m_settlement_buttons[1]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = BACK_MANU_EVENT;
        SDL_PushEvent(&event);
        m_settlement_buttons[1]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=BACK_MANU_EVENT");
    }
}

void SettlementManage::setFontCoordinate(const int& x, const int& y)
{
    m_font_x = x;
    m_font_y = y;
    INFOLOG("setFontCoordinate||font_x={}||font_y={}", m_font_x, m_font_y);
}

void SettlementManage::setInterfaceRenderType(const interface_kind_type& type)
{
    if(type == PLAYER_WIN_INTERFACE || type == PLAYER_LOSE_INTERFACE || type == BLACK_WIN_INTERFACE || type == WHITE_WIN_INTERFACE)
    {
        m_interface_render_type = type;
        INFOLOG("setInterfaceRenderType||interface_render_type={}", (int)m_interface_render_type);
    }
    else
    {
        ERRORLOG("setInterfaceRenderType||interface_render_type is error!");
    }
}
