#include "playchess_manage.h"

//多线程锁
SDL_SpinLock player_machine_lock = 0;

SDL_Thread* machine_thread = nullptr;
// 下棋音效
extern Mix_Chunk* g_chess_down_sound;
// 主窗口
extern SDLWindow* g_main_window; 
// 艺术字体 行楷
extern TTF_Font* g_art_font;
// 普通字体
extern TTF_Font* g_normal_font;

// 是否机器人先手
bool is_machine_first = false;

PlaychessManage::PlaychessManage(const Config& config)
{
    m_chessboard = new Chessboard(config); // TODO 新建优化
    m_machine = new Machine(m_chessboard);
    m_single_player = new Player(m_chessboard, "single_player", CHESS_BLACK);
    m_single_player2 = new Player(m_chessboard, "single_player2", CHESS_BLACK);
    m_chess_data_board = new ChessDataBoard(config);
    m_button_interval = config.Read("playchess_buttons_interval", 0);
    m_buttons_x = config.Read("playchess_buttons_x", 0);
    m_buttons_y = config.Read("playchess_buttons_y", 0);
    m_playchess_buttons[0] = new SDLButton(config, "withdraw", m_buttons_x, m_buttons_y-m_button_interval);
    m_playchess_buttons[1] = new SDLButton(config, "back_menu", m_buttons_x, m_buttons_y);
    m_array_length = sizeof(m_playchess_buttons) / sizeof(m_playchess_buttons[0]);
    m_is_reset_single_player_chess_data_board = true;
    m_is_reset_two_players_chess_data_board = true;
    DEBUGLOG("PlaychessManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    m_button_interval, m_buttons_x, m_buttons_y, m_array_length);
}

PlaychessManage::~PlaychessManage()
{
    delete m_chessboard;
    delete m_machine;
    delete m_single_player;
    delete m_single_player2;
    delete m_chess_data_board;
    SDL_WaitThread(machine_thread, nullptr);
    DEBUGLOG("~PlaychessManage success||release resource");
}

void PlaychessManage::init()
{
    m_chessboard->init();
    m_chess_data_board->init();
    for (int i = 0; i < m_array_length; i++)
    {
        m_playchess_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
    for (int i = 0; i < m_array_length; i++)
    {
        m_playchess_buttons[i]->loadResource(g_main_window->getWindow(), g_main_window->getRenderer());
    }
    INFOLOG("loadResource||load resource success");
}

void PlaychessManage::startRender()
{
    m_chessboard->render();
    m_chess_data_board->render(m_chessboard->getPlayerFlagType());
    for (int i = 0; i < m_array_length; i++)
    {
        m_playchess_buttons[i]->buttonRender(g_main_window->getRenderer());
    }
    // DEBUGLOG("startRender");
}

// Machine类的友元函数
int machineChessDown(void* data)
{
	Machine* machine = (Machine *)(data);
    ChessPos pos;
    // 程序休眠1s，假装在思考
    sleepSeconds(1.5);
    chess_kind_type chess_type = CHESS_DEFAULT;
    if (is_machine_first)
    {
        chess_type = CHESS_BLACK;
    }
    else
    {
        chess_type = CHESS_WHITE;
    }
    // 初始下棋在中间位置
    if (is_machine_first && machine->getChessNum() == 0)
    {
        pos = ChessPos{7, 7};
    }
    else  // 非初始下棋，需要每一步进行打分
    {
        pos = machine->think();
    }
    Mix_PlayChannel(-1, g_chess_down_sound, 0);   // TODO 优化音效延迟
	machine->m_chessboard->chessDown(pos, chess_type);
    machine->addChessNum();
    if(machine->m_chessboard->checkOver())
    {
        SDL_Event event;
        event.type = PLAYER_LOSE_EVENT;
        SDL_PushEvent(&event);
    }
    machine->m_chessboard->setPlayerFlagType(SINGLE_PLAYER);
    DEBUGLOG("Machine::go||Machine chess down success");
    return 0;
}

bool PlaychessManage::handleMouseClick(SDL_Event* event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)  // 鼠标点击事件
    {
        if (m_chessboard->getPlayerFlagType() == SINGLE_PLAYER)   // 单人游戏
        {
            //获取鼠标位置
            int x, y;
            SDL_GetMouseState(&x, &y);
            ChessPos pos;
            // 检查是否有效落子
            bool is_valid_click = m_chessboard->clickBoard(x, y, &pos);
            if (is_valid_click)
            {
                chess_kind_type chess_type = CHESS_DEFAULT;
                if (is_machine_first)
                {
                    chess_type = CHESS_WHITE;
                }
                else
                {
                    chess_type = CHESS_BLACK;
                }
                Mix_PlayChannel(-1, g_chess_down_sound, 0);
                m_chessboard->chessDown(pos, chess_type);
                m_single_player->addChessNum();
                if(m_chessboard->checkOver())
                {
                    SDL_Event event;
                    event.type = PLAYER_WIN_EVENT;
                    INFOLOG("handleMouseClick||push event||event_type={}", event.type);
                    SDL_PushEvent(&event);
                }
                else
                {
                    m_chessboard->setPlayerFlagType(MACHINE_PLAYER);
                }
            }
            return is_valid_click;
        }
        else if (m_chessboard->getPlayerFlagType() == BLACK_PLAYER || m_chessboard->getPlayerFlagType() == WHITE_PLAYER)  // 双人游戏
        {
            //获取鼠标位置
            int x, y;
            SDL_GetMouseState(&x, &y);
            ChessPos pos;
            // 检查是否有效落子
            bool is_valid_click = m_chessboard->clickBoard(x, y, &pos);
            if (is_valid_click)
            {
                chess_kind_type chess_type = CHESS_DEFAULT;
                // 判断落子颜色
                if (m_chessboard->getPlayerFlagType() == WHITE_PLAYER)
                {
                    chess_type = CHESS_WHITE;
                }
                else
                {
                    chess_type = CHESS_BLACK;
                }
                Mix_PlayChannel(-1, g_chess_down_sound, 0);
                m_chessboard->chessDown(pos, chess_type);
                // m_single_player->addChessNum();  // 双人游戏暂时不需要计算玩家棋子
                if(m_chessboard->checkOver())
                {
                    SDL_Event event;
                    std::string msg = "TWO_PLAYER_WIN_EVENT";
                    // 判断赢方
                    if (m_chessboard->getPlayerFlagType() == WHITE_PLAYER)
                    {
                        event.type = TWO_PLAYER_WHITE_WIN_EVENT;
                        msg = "TWO_PLAYER_WHITE_WIN_EVENT";
                    }
                    else
                    {
                        event.type = TWO_PLAYER_BLACK_WIN_EVENT;
                        msg = "TWO_PLAYER_BLACK_WIN_EVENT";
                    }
                    INFOLOG("handleMouseClick||push event||event_type={}", msg);
                    SDL_PushEvent(&event);
                }
                else if (m_chessboard->getPlayerFlagType() == WHITE_PLAYER)               
                {
                    m_chessboard->setPlayerFlagType(BLACK_PLAYER);
                }
                else
                {
                    m_chessboard->setPlayerFlagType(WHITE_PLAYER);
                }
            }
            return is_valid_click;
        }
        
    }
    
    return false;
}

// TODO 网络对战
void PlaychessManage::handleEvents(SDL_Event* event)
{
    // 更新游戏比分
    // 单人游戏，玩家赢
    if (event->type == PLAYER_WIN_EVENT)
    {
        m_chess_data_board->updateScoreInfo(SINGLE_PLAYER_WIN);
    }
    // 单人游戏机器人赢
    else if (event->type == PLAYER_LOSE_EVENT)
    {
        m_chess_data_board->updateScoreInfo(SINGLE_PLAYER_LOSE);
    }
    // 双人游戏黑方赢
    else if (event->type == TWO_PLAYER_BLACK_WIN_EVENT)
    {
        m_chess_data_board->updateScoreInfo(TWO_PLAYER_BLACK_WIN);
    }
    // 双人游戏白方赢
    else if (event->type == TWO_PLAYER_WHITE_WIN_EVENT)
    {
        m_chess_data_board->updateScoreInfo(TWO_PLAYER_WHITE_WIN);
    }

    // 单人游戏悔棋事件
    else if (event->type == PLAYER_WITHDRAW_EVENT)
    {
        if(m_chessboard->isCanWithdraw())
        {
            m_chessboard->setChessboardWithdraw();
        }
    }
    // 根据选择的先后手设置数据板，初始化棋盘，启动定时器，设置当前下棋方
    else if (event->type == SINGLE_PLAYER_WHITE_EVENT)
    {
        is_machine_first = true;  // 设置机器人先手
        if (m_is_reset_single_player_chess_data_board)
        {
            m_chess_data_board->initDataBoard(WHITE_COLOR_TYPE);
            m_is_reset_single_player_chess_data_board = false;
        }
        m_single_player->resetPlayer();
        m_machine->resetMachine();
        m_chessboard->setPlayerFlagType(MACHINE_PLAYER);
        m_chessboard->initChessMap();
        m_chess_data_board->startSingleGame(WHITE_COLOR_TYPE);
        machine_thread = SDL_CreateThread(machineChessDown, "machine player", m_machine);
    }
    else if (event->type == SINGLE_PLAYER_BLACK_EVENT)
    {
        is_machine_first = false;  // 设置机器人后手
        if (m_is_reset_single_player_chess_data_board)
        {
            m_chess_data_board->initDataBoard(BLACK_COLOR_TYPE);
            m_is_reset_single_player_chess_data_board = false;
        }
        m_single_player->resetPlayer();
        m_machine->resetMachine();
        m_chessboard->setPlayerFlagType(SINGLE_PLAYER);
        m_chessboard->initChessMap();
        m_chess_data_board->startSingleGame(BLACK_COLOR_TYPE);
    }
    // 双人游戏
    else if (event->type == TWO_PLAYER_EVENT || event->type == TWO_PLAYER_AGAIN_GAME_EVENT)
    {
        if (m_is_reset_two_players_chess_data_board)
        {
            m_chess_data_board->initDataBoard(TWO_PLAYERS_COLOR_TYPE);
            m_is_reset_two_players_chess_data_board = false;
        }
        m_single_player->resetPlayer();
        m_single_player2->resetPlayer();
        m_chessboard->setPlayerFlagType(BLACK_PLAYER);
        m_chessboard->initChessMap();
        m_chess_data_board->startSingleGame(TWO_PLAYERS_COLOR_TYPE);
    }
    // 鼠标点击事件
    if(this->handleMouseClick(event) && m_chessboard->getPlayerFlagType() == MACHINE_PLAYER)
    {
        machine_thread = SDL_CreateThread(machineChessDown, "machine player", m_machine);
    }
    // 遍历渲染当前按键的状态
    for (int i = 0; i < m_array_length; i++)
    {
        m_playchess_buttons[i]->handleButtonEvent(event);
    }
    // 检查悔棋键状态
    if (m_playchess_buttons[0]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = PLAYER_WITHDRAW_EVENT;
        SDL_PushEvent(&event);
        m_playchess_buttons[0]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=PLAYER_WITHDRAW_EVENT");
    }
    // 检查返回主菜单键状态
    if (m_playchess_buttons[1]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = BACK_MANU_EVENT;
        SDL_PushEvent(&event);
        m_playchess_buttons[1]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=BACK_MANU_EVENT");
    }
}

int PlaychessManage::getChessboardCenterX()
{
    return m_chessboard->getCenterX();
}

int PlaychessManage::getChessboardCenterY()
{
    return m_chessboard->getCenterY();
}
