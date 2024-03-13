#include "playchess_manage.h"

//多线程锁
SDL_SpinLock player_machine_lock = 0;

SDL_Thread* machine_thread = nullptr;
// 下棋音效
extern Mix_Chunk* chess_down_sound;
// 主窗口
extern SDLWindow* main_window; 

// 是否机器人先手
bool is_machine_first = false;

PlaychessManage::PlaychessManage(const Config& config)
{
    this->chessboard = new Chessboard(config); // TODO 新建优化
    this->machine = new Machine(this->chessboard);
    this->single_player = new Player(this->chessboard, "single_player", CHESS_BLACK);
    this->single_player2 = new Player(this->chessboard, "single_player2", CHESS_BLACK);
    this->chess_data_board = new ChessDataBoard(config);
    this->button_interval = config.Read("playchess_buttons_interval", 0);
    this->buttons_x = config.Read("playchess_buttons_x", 0);
    this->buttons_y = config.Read("playchess_buttons_y", 0);
    this->playchess_buttons[0] = new SDLButton(config, "withdraw", this->buttons_x, this->buttons_y-this->button_interval);
    this->playchess_buttons[1] = new SDLButton(config, "back_menu", this->buttons_x, this->buttons_y);
    this->array_length = sizeof(this->playchess_buttons) / sizeof(this->playchess_buttons[0]);
    this->is_reset_single_player_chess_data_board = true;
    this->is_reset_two_players_chess_data_board = true;
    DEBUGLOG("PlaychessManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    this->button_interval, this->buttons_x, this->buttons_y, this->array_length);
}

PlaychessManage::~PlaychessManage()
{
    delete chessboard;
    delete machine;
    delete single_player;
    delete single_player2;
    delete chess_data_board;
    SDL_WaitThread(machine_thread, nullptr);
    DEBUGLOG("~PlaychessManage success||release resource");
}

void PlaychessManage::init(TTF_Font* normal_ttf, TTF_Font* art_ttf)
{
    this->normal_ttf = normal_ttf;
    this->art_ttf = art_ttf;
    this->chessboard->init(this->normal_ttf);
    this->chess_data_board->init(this->normal_ttf, this->art_ttf);
    for (int i = 0; i < this->array_length; i++)
    {
        this->playchess_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
    for (int i = 0; i < this->array_length; i++)
    {
        this->playchess_buttons[i]->loadResource(main_window->getWindow(), main_window->getRenderer());
    }
    INFOLOG("loadResource||load resource success");
}

void PlaychessManage::startRender()
{
    this->chessboard->render();
    this->chess_data_board->render(this->chessboard->get_player_flag_type());
    for (int i = 0; i < this->array_length; i++)
    {
        this->playchess_buttons[i]->buttonRender(main_window->getRenderer());
    }
    // DEBUGLOG("startRender");
}

// Machine类的友元函数
int machineChessDown(void* data)
{
	Machine* machine = (Machine *)(data);
    ChessPos pos;
    // 程序休眠1s，假装在思考
    MyUtils::sleep_seconds(1.5);
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
    Mix_PlayChannel(-1, chess_down_sound, 0);
	machine->chessboard->chessDown(pos, chess_type);
    machine->addChessNum();
    if(machine->chessboard->checkOver())
    {
        SDL_Event event;
        event.type = PLAYER_LOSE_EVENT;
        SDL_PushEvent(&event);
    }
    machine->chessboard->set_player_flag_type(SINGLE_PLAYER);
    DEBUGLOG("Machine::go||Machine chess down success");
    return 0;
}

bool PlaychessManage::handleMouseClick(SDL_Event* event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)  // 鼠标点击事件
    {
        if (this->chessboard->get_player_flag_type() == SINGLE_PLAYER)   // 单人游戏
        {
            //获取鼠标位置
            int x, y;
            SDL_GetMouseState(&x, &y);
            ChessPos pos;
            // 检查是否有效落子
            bool is_valid_click = this->chessboard->clickBoard(x, y, &pos);
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
                Mix_PlayChannel(-1, chess_down_sound, 0);
                this->chessboard->chessDown(pos, chess_type);
                this->single_player->addChessNum();
                if(this->chessboard->checkOver())
                {
                    SDL_Event event;
                    event.type = PLAYER_WIN_EVENT;
                    INFOLOG("handleMouseClick||push event||event_type={}", event.type);
                    SDL_PushEvent(&event);
                }
                else
                {
                    this->chessboard->set_player_flag_type(MACHINE_PLAYER);
                }
            }
            return is_valid_click;
        }
        else if (this->chessboard->get_player_flag_type() == BLACK_PLAYER || this->chessboard->get_player_flag_type() == WHITE_PLAYER)  // 双人游戏
        {
            //获取鼠标位置
            int x, y;
            SDL_GetMouseState(&x, &y);
            ChessPos pos;
            // 检查是否有效落子
            bool is_valid_click = this->chessboard->clickBoard(x, y, &pos);
            if (is_valid_click)
            {
                chess_kind_type chess_type = CHESS_DEFAULT;
                // 判断落子颜色
                if (this->chessboard->get_player_flag_type() == WHITE_PLAYER)
                {
                    chess_type = CHESS_WHITE;
                }
                else
                {
                    chess_type = CHESS_BLACK;
                }
                Mix_PlayChannel(-1, chess_down_sound, 0);
                this->chessboard->chessDown(pos, chess_type);
                // this->single_player->addChessNum();  // 双人游戏暂时不需要计算玩家棋子
                if(this->chessboard->checkOver())
                {
                    SDL_Event event;
                    std::string msg = "TWO_PLAYER_WIN_EVENT";
                    // 判断赢方
                    if (this->chessboard->get_player_flag_type() == WHITE_PLAYER)
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
                else if (this->chessboard->get_player_flag_type() == WHITE_PLAYER)               
                {
                    this->chessboard->set_player_flag_type(BLACK_PLAYER);
                }
                else
                {
                    this->chessboard->set_player_flag_type(WHITE_PLAYER);
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
        this->chess_data_board->updateScoreInfo(SINGLE_PLAYER_WIN);
    }
    // 单人游戏机器人赢
    else if (event->type == PLAYER_LOSE_EVENT)
    {
        this->chess_data_board->updateScoreInfo(SINGLE_PLAYER_LOSE);
    }
    // 双人游戏黑方赢
    else if (event->type == TWO_PLAYER_BLACK_WIN_EVENT)
    {
        this->chess_data_board->updateScoreInfo(TWO_PLAYER_BLACK_WIN);
    }
    // 双人游戏白方赢
    else if (event->type == TWO_PLAYER_WHITE_WIN_EVENT)
    {
        this->chess_data_board->updateScoreInfo(TWO_PLAYER_WHITE_WIN);
    }

    // 单人游戏悔棋事件
    else if (event->type == PLAYER_WITHDRAW_EVENT)
    {
        if(this->chessboard->is_can_withdraw())
        {
            this->chessboard->set_chessboard_withdraw();
        }
    }
    // 根据选择的先后手设置数据板，初始化棋盘，启动定时器，设置当前下棋方
    else if (event->type == SINGLE_PLAYER_WHITE_EVENT)
    {
        is_machine_first = true;  // 设置机器人先手
        if (this->is_reset_single_player_chess_data_board)
        {
            this->chess_data_board->initDataBoard(WHITE_COLOR_TYPE);
            this->is_reset_single_player_chess_data_board = false;
        }
        this->single_player->resetPlayer();
        this->machine->resetMachine();
        this->chessboard->set_player_flag_type(MACHINE_PLAYER);
        this->chessboard->initChessMap();
        this->chess_data_board->startSingleGame(WHITE_COLOR_TYPE);
        machine_thread = SDL_CreateThread(machineChessDown, "machine player", this->machine);
    }
    else if (event->type == SINGLE_PLAYER_BLACK_EVENT)
    {
        is_machine_first = false;  // 设置机器人后手
        if (this->is_reset_single_player_chess_data_board)
        {
            this->chess_data_board->initDataBoard(BLACK_COLOR_TYPE);
            this->is_reset_single_player_chess_data_board = false;
        }
        this->single_player->resetPlayer();
        this->machine->resetMachine();
        this->chessboard->set_player_flag_type(SINGLE_PLAYER);
        this->chessboard->initChessMap();
        this->chess_data_board->startSingleGame(BLACK_COLOR_TYPE);
    }
    // 双人游戏
    else if (event->type == TWO_PLAYER_EVENT || event->type == TWO_PLAYER_AGAIN_GAME_EVENT)
    {
        if (this->is_reset_two_players_chess_data_board)
        {
            this->chess_data_board->initDataBoard(TWO_PLAYERS_COLOR_TYPE);
            this->is_reset_two_players_chess_data_board = false;
        }
        this->single_player->resetPlayer();
        this->single_player2->resetPlayer();
        this->chessboard->set_player_flag_type(BLACK_PLAYER);
        this->chessboard->initChessMap();
        this->chess_data_board->startSingleGame(TWO_PLAYERS_COLOR_TYPE);
    }
    // 鼠标点击事件
    if(this->handleMouseClick(event) && this->chessboard->get_player_flag_type() == MACHINE_PLAYER)
    {
        machine_thread = SDL_CreateThread(machineChessDown, "machine player", this->machine);
    }
    // 遍历渲染当前按键的状态
    for (int i = 0; i < this->array_length; i++)
    {
        this->playchess_buttons[i]->handleButtonEvent(event);
    }
    // 检查悔棋键状态
    if (this->playchess_buttons[0]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = PLAYER_WITHDRAW_EVENT;
        SDL_PushEvent(&event);
        this->playchess_buttons[0]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=PLAYER_WITHDRAW_EVENT");
    }
    // 检查返回主菜单键状态
    if (this->playchess_buttons[1]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = BACK_MANU_EVENT;
        SDL_PushEvent(&event);
        this->playchess_buttons[1]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=BACK_MANU_EVENT");
    }
}

int PlaychessManage::get_chessboard_center_x()
{
    return this->chessboard->get_center_x();
}

int PlaychessManage::get_chessboard_center_y()
{
    return this->chessboard->get_center_y();
}
