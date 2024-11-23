#include "main_menu_manage.h"

// 主窗口
extern SDLWindow* g_main_window; 

MainMenuManage::MainMenuManage(const Config& config)
{
    m_buttons_x = config.Read("g_main_window_screen_width", 0) / 2;// TODO 新建优化
    m_buttons_y = config.Read("g_main_window_screen_height", 0) / 2;
    m_button_interval = config.Read("main_menu_buttons_interval", 0);
    m_main_menu_buttons[0] = new SDLButton(config, "start_game", m_buttons_x, m_buttons_y-m_button_interval);
    m_main_menu_buttons[1] = new SDLButton(config, "best_scores", m_buttons_x, m_buttons_y);
    m_main_menu_buttons[2] = new SDLButton(config, "exit_game", m_buttons_x, m_buttons_y+m_button_interval);
    m_array_length = sizeof(m_main_menu_buttons) / sizeof(m_main_menu_buttons[0]);
    m_is_show_best_score = false;
    DEBUGLOG("MainMenuManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    m_button_interval, m_buttons_x, m_buttons_y, m_array_length);
}

MainMenuManage::~MainMenuManage()
{
    DEBUGLOG("~MainMenuManage success||release resource");
}

void MainMenuManage::init()
{
    for (int i = 0; i < m_array_length; i++)
    {
        m_main_menu_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
    for (int i = 0; i < m_array_length; i++)
    {
        m_main_menu_buttons[i]->loadResource(g_main_window->getWindow(), g_main_window->getRenderer());
    }
    INFOLOG("loadResource||load resource success");
}

void MainMenuManage::startRender()
{
    if (m_is_show_best_score)
    {
        DEBUGLOG("111");
    }
    else
    {
        for (int i = 0; i < m_array_length; i++)
        {
            m_main_menu_buttons[i]->buttonRender(g_main_window->getRenderer());
        }
    }
    // DEBUGLOG("startRender");
}

// TODO 查看排行榜
void MainMenuManage::handleEvents(SDL_Event* event)
{
    switch (event->type)
    {
    case BEST_SCORES_EVENT:
        this->showBestScore();
        break;
    default:
        for (int i = 0; i < m_array_length; i++)
        {
            m_main_menu_buttons[i]->handleButtonEvent(event);
        }
        break;
    }
    if (m_main_menu_buttons[0]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = START_GAME_EVENT;
        SDL_PushEvent(&event);
        m_main_menu_buttons[0]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=START_GAME_EVENT");
    }
    if (m_main_menu_buttons[1]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = BEST_SCORES_EVENT;
        SDL_PushEvent(&event);
        m_main_menu_buttons[1]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=BEST_SCORES_EVENT");
    }
    if (m_main_menu_buttons[2]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = EXIT_GAME_EVENT;
        SDL_PushEvent(&event);
        INFOLOG("handleEvents||push event=EXIT_GAME_EVENT");
    }
}

void MainMenuManage::showBestScore()
{
    m_is_show_best_score = true;
}



