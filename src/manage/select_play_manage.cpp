#include "select_play_manage.h"

// 主窗口
extern SDLWindow* g_main_window; 
// 艺术字体 行楷
extern TTF_Font* g_art_font;

SelectPlayManage::SelectPlayManage(const Config& config)
{
    m_buttons_x = config.Read("g_main_window_screen_width", 0) / 2;// TODO 新建优化
    m_buttons_y = config.Read("g_main_window_screen_height", 0) / 2;
    m_button_interval = config.Read("select_play_buttons_interval", 0);
    m_select_play_buttons[0] = new SDLButton(config, "single_player", m_buttons_x, m_buttons_y-1.5*m_button_interval);
    m_select_play_buttons[1] = new SDLButton(config, "two_players", m_buttons_x, m_buttons_y-m_button_interval/2);
    m_select_play_buttons[2] = new SDLButton(config, "play_internet", m_buttons_x, m_buttons_y+m_button_interval/2);
    m_select_play_buttons[3] = new SDLButton(config, "normal_back_menu", m_buttons_x, m_buttons_y+1.5*m_button_interval);
    m_array_length = sizeof(m_select_play_buttons) / sizeof(m_select_play_buttons[0]);
    m_select_chess_color_window = new SDLWindow(config, "select_chess_color_window");
    m_white_color_chess = new Chess(config, "symbol_white_chess");
    m_black_color_chess = new Chess(config, "symbol_black_chess");
    m_select_chess_color_ttf = new SDLTTF("select_chess_color_ttf");
    DEBUGLOG("SelectPlayManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    m_button_interval, m_buttons_x, m_buttons_y, m_array_length);
}

SelectPlayManage::~SelectPlayManage()
{
    delete m_select_chess_color_window;
    delete m_white_color_chess;
    delete m_black_color_chess;
    delete m_select_chess_color_ttf;
    DEBUGLOG("~SelectPlayManage success||release resource");
}

void SelectPlayManage::init()
{
    for (int i = 0; i < m_array_length; i++)
    {
        m_select_play_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
    for (int i = 0; i < m_array_length; i++)
    {
        m_select_play_buttons[i]->loadResource(g_main_window->getWindow(), g_main_window->getRenderer());
    }
    INFOLOG("loadResource||load resource success");
}

void SelectPlayManage::startRender()
{
    for (int i = 0; i < m_array_length; i++)
    {
        m_select_play_buttons[i]->buttonRender(g_main_window->getRenderer());
    }
    // DEBUGLOG("startRender");
}

void SelectPlayManage::handleEvents(SDL_Event* event)
{
    // 单人游戏或单人游戏的再来一局
    if (event->type == SINGLE_PLAYER_EVENT || event->type == SINGLE_PLAYER_AGAIN_GAME_EVENT)
    {
        this->singlePlaySelectChess();
    }
    for (int i = 0; i < m_array_length; i++)
    {
        m_select_play_buttons[i]->handleButtonEvent(event);
    }
    if (m_select_play_buttons[0]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP) // TODO 优化BUTTON_SPRITE_MOUSE_UP
    {
        // 单人游戏
        m_current_game_type = SINGLE_PLAYER_GAME;
        SDL_Event event;
        event.type = SINGLE_PLAYER_EVENT;
        SDL_PushEvent(&event);
        m_select_play_buttons[0]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=SINGLE_PLAYER_EVENT||current_game_type = SINGLE_PLAYER_GAME");
    }
    if (m_select_play_buttons[1]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        // 双人游戏
        m_current_game_type = TWO_PLAYERS_GAME;
        SDL_Event event;
        event.type = TWO_PLAYER_EVENT;
        SDL_PushEvent(&event);
        m_select_play_buttons[1]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=TWO_PLAYER_EVENT||current_game_type = TWO_PLAYERS_GAME");
    }
    if (m_select_play_buttons[2]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        // 网络对战
        m_current_game_type = PLAYER_INTERNET_GAME;
        SDL_Event event;
        event.type = PLAY_INTERNET_EVENT;
        SDL_PushEvent(&event);
        m_select_play_buttons[2]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=PLAY_INTERNET_EVENT||current_game_type = PLAYER_INTERNET_GAME");
    }
    if (m_select_play_buttons[3]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        // 返回主菜单
        m_current_game_type = GAME_KIND_DEFAULT;
        SDL_Event event;
        event.type = BACK_MANU_EVENT;
        SDL_PushEvent(&event);
        m_select_play_buttons[3]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=BACK_MANU_EVENT||current_game_type = GAME_KIND_DEFAULT");
    }
}

void SelectPlayManage::singlePlaySelectChess()
{
    m_select_chess_color_window->init();
    m_white_color_chess->init(m_select_chess_color_window);
    m_black_color_chess->init(m_select_chess_color_window);
    m_white_color_chess->setChessCoordinate(m_select_chess_color_window->getWidth()/2-60, m_select_chess_color_window->getHeight()/2-20);
    m_white_color_chess->setChessMultiple(0.8);
    m_black_color_chess->setChessCoordinate(m_select_chess_color_window->getWidth()/2+60, m_select_chess_color_window->getHeight()/2-20);
    m_black_color_chess->setChessMultiple(0.8);
    std::pair<sdl_button_sprite, sdl_button_sprite> chess_mouse_type_pair;
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
            if (event.type == SDL_WINDOWEVENT && event.window.windowID == m_select_chess_color_window->getWindowId())
            {
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    quit = true;
                }
                switch (event.window.event)
                {
                //SDL_WINDOWEVENT_ENTER用于处理鼠标移入窗口的情况
                case SDL_WINDOWEVENT_ENTER:
                    mouse_focus = true;
                    break;
                //SDL_WINDOWEVENT_LEAVE 于处理鼠标移出窗口的情况
                case SDL_WINDOWEVENT_LEAVE:
                    mouse_focus = false;
                    break;
                default:
                    break;
                }
            }
            /*
                排除键盘焦点改变事件，因为此时为多窗口，鼠标点击与释放会触发此事件，所以每次鼠标点击与释放进入到handleWindowEvents中
                BUTTON_SPRITE_MOUSE_DOWN与BUTTON_SPRITE_MOUSE_UP后都会跟着一个SDL_KEYMAPCHANGED事件，在handleWindowEvents中，
                switch按照鼠标事件设置完返回值后，SDL_KEYMAPCHANGED事件会将设置的值重新置为初始状态
            */
            if (event.type != SDL_KEYMAPCHANGED && event.type != SDL_WINDOWEVENT)
            {
                chess_mouse_type_pair = this->handleWindowEvents(&event, mouse_focus);
            }
        }
        //Clear screen
        SDL_SetRenderDrawColor(m_select_chess_color_window->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_select_chess_color_window->getRenderer());
        quit = this->selectChessRender(chess_mouse_type_pair) || quit;
        SDL_RenderPresent(m_select_chess_color_window->getRenderer());
    }
    //Free resources and close SDL
    m_select_chess_color_window->free();
}

std::pair<sdl_button_sprite, sdl_button_sprite> SelectPlayManage::handleWindowEvents(SDL_Event* event, bool mouse_focus)
{
    sdl_button_sprite chess_mouse_type[2] = {BUTTON_SPRITE_MOUSE_OUT, BUTTON_SPRITE_MOUSE_OUT};
    Chess* chess_arr[2] = {m_white_color_chess, m_black_color_chess};
    if (mouse_focus)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        for (int i = 0; i < 2; i++)
        {
            std::pair<int, int> chess_coordinate = chess_arr[i] ->getChessCoordinate();
            int chess_size = chess_arr[i]->getChessSize();
            if (x >= chess_coordinate.first - (chess_size/2) && x <= chess_coordinate.first + (chess_size/2) && y >= chess_coordinate.second - (chess_size/2) && y<= chess_coordinate.second + (chess_size/2))
            {
                switch (event->type)
                {
                case SDL_MOUSEMOTION:
                    chess_mouse_type[i] = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    chess_mouse_type[i] = BUTTON_SPRITE_MOUSE_DOWN;
                    break;
                case SDL_MOUSEBUTTONUP:
                    chess_mouse_type[i] = BUTTON_SPRITE_MOUSE_UP;
                    break;
                default:
                    break;
                }                   
            }
            // DEBUGLOG("handleWindowEvents||i={}||chess_mouse_type={}", i, (int)chess_mouse_type[i]);
        }
    }
    std::pair<sdl_button_sprite, sdl_button_sprite> chess_mouse_type_pair(chess_mouse_type[0], chess_mouse_type[1]);
    // DEBUGLOG("handleWindowEvents||chess_mouse_type_pair.first={}||chess_mouse_type_pair.second={}||type={}", 
    // (int)chess_mouse_type_pair.first, (int)chess_mouse_type_pair.second, (int)event->type);
    return chess_mouse_type_pair;
}

void SelectPlayManage::renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple)
{
	m_select_chess_color_ttf->loadRenderText(m_select_chess_color_window->getRenderer(), texture_ttf, texture_text, color);
	m_select_chess_color_ttf->ttfRender(m_select_chess_color_window->getRenderer(), x, y, multiple);
}

bool SelectPlayManage::selectChessRender(std::pair<sdl_button_sprite, sdl_button_sprite> mouse_event)
{
    Chess* chess_arr[2] = {m_white_color_chess, m_black_color_chess};
    sdl_button_sprite chess_mouse_type[2] = {mouse_event.first, mouse_event.second};
    std::string render_text[2] = {"后手", "先手"};
    bool is_quit = false;
    for (int i = 0; i < 2; i++)
    {
        SDL_Color color = {0, 0, 0, 0};
        chess_arr[i]->chessRender();
        switch (chess_mouse_type[i])
        {
        case BUTTON_SPRITE_MOUSE_OVER_MOTION:
            color = {255, 0, 0, 0};
            break;
        case BUTTON_SPRITE_MOUSE_DOWN:
            color = {0, 255, 0, 0};
            break;
        case BUTTON_SPRITE_MOUSE_UP:
            SDL_Event event;
            is_quit = true;
            if (i == 0)
            {
                event.type = SINGLE_PLAYER_WHITE_EVENT;
                INFOLOG("selectChessRender||push event=SINGLE_PLAYER_WHITE_EVENT");
            }
            else
            {
                event.type = SINGLE_PLAYER_BLACK_EVENT;
                INFOLOG("selectChessRender||push event=SINGLE_PLAYER_BLACK_EVENT");
            }
            SDL_PushEvent(&event);
            break;
        default:
            break;
        }
        this->renderText(render_text[i], g_art_font, color, chess_arr[i]->getChessCoordinate().first, chess_arr[i]->getChessCoordinate().second+60, 0.05);
    }
    return is_quit;
}
