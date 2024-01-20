#include "playchess_manage.h"

//多线程锁
SDL_SpinLock player_machine_lock = 0;

SDL_Thread* machine_thread = nullptr;

PlaychessManage::PlaychessManage(const Config& config)
{
    this->chessboard = new Chessboard(config);
    this->machine = new Machine(this->chessboard);
    this->single_player = new Player(this->chessboard, "single_player", CHESS_BLACK);
    this->chess_data_board = new ChessDataBoard(config);
    this->button_interval = config.Read("playchess_buttons_interval", 0);
    this->buttons_x = config.Read("playchess_buttons_x", 0);
    this->buttons_y = config.Read("playchess_buttons_y", 0);
    this->playchess_buttons[0] = new SDLButton(config, "withdraw", this->buttons_x, this->buttons_y-this->button_interval);
    this->playchess_buttons[1] = new SDLButton(config, "back_menu", this->buttons_x, this->buttons_y);
    this->array_length = sizeof(this->playchess_buttons) / sizeof(this->playchess_buttons[0]);
    DEBUGLOG("PlaychessManage construct success||button_interval={}||buttons_x={}||buttons_y={}||array_length={}", 
    this->button_interval, this->buttons_x, this->buttons_y, this->array_length);
}

PlaychessManage::~PlaychessManage()
{
    delete chessboard;
    delete machine;
    delete single_player;
    delete chess_data_board;
    SDL_WaitThread(machine_thread, nullptr);
    DEBUGLOG("~PlaychessManage success||release resource");
}

void PlaychessManage::init(SDLWindow* sdl_window, TTF_Font* normal_ttf, TTF_Font* art_ttf)
{
    this->playchess_main_window = sdl_window;
    this->normal_ttf = normal_ttf;
    this->art_ttf = art_ttf;
    this->chessboard->init(this->playchess_main_window, this->normal_ttf);
    this->chess_data_board->init(this->playchess_main_window, this->normal_ttf, this->art_ttf);
    for (int i = 0; i < this->array_length; i++)
    {
        this->playchess_buttons[i]->initButtonCurrentSprite();
    }
    INFOLOG("init||init variable success");
    for (int i = 0; i < this->array_length; i++)
    {
        this->playchess_buttons[i]->loadResource(this->playchess_main_window->getWindow(), this->playchess_main_window->getRenderer());
    }
    INFOLOG("loadResource||load resource success");
}

void PlaychessManage::startRender()
{
    this->chessboard->render();
    this->chess_data_board->render();
    for (int i = 0; i < this->array_length; i++)
    {
        this->playchess_buttons[i]->buttonRender(this->playchess_main_window->getRenderer());
    }
    // DEBUGLOG("startRender");
}

// Machine类的友元函数
int machineChessDown(void* data)
{
	Machine* machine = (Machine *)(data);
    ChessPos pos = machine->think();
    // 程序休眠1s，假装在思考
    MyUtils::sleep_seconds(1.5);
	// mciSendString("play res/chess_down.mp3", 0, 0, 0);
	machine->chessboard->chessDown(pos, CHESS_WHITE);
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
    if (event->type == SDL_MOUSEBUTTONDOWN && this->chessboard->get_player_flag_type() == SINGLE_PLAYER)  // 鼠标点击事件
    {
        //获取鼠标位置
        int x, y;
        SDL_GetMouseState(&x, &y);
        ChessPos pos;
        // 检查是否有效落子
        bool is_valid_click = this->chessboard->clickBoard(x, y, &pos);
        if (is_valid_click)
        {
            this->chessboard->chessDown(pos, CHESS_BLACK);
            this->single_player->addChessNum();
            if(this->chessboard->checkOver())
            {
                SDL_Event event;
                event.type = PLAYER_WIN_EVENT;
                SDL_PushEvent(&event);
            }
            this->chessboard->set_player_flag_type(MACHINE_PLAYER);
        }
        return is_valid_click;
    }
    return false;
}

void PlaychessManage::handleEvents(SDL_Event* event)
{
    if (event->type == PLAYER_WIN_EVENT)
    {
        this->chess_data_board->updateScoreInfo(SINGLE_PLAYER_WIN);
    }
    else if (event->type == PLAYER_LOSE_EVENT)
    {
        this->chess_data_board->updateScoreInfo(SINGLE_PLAYER_LOSE);
    }
    else if (event->type == PLAYER_WITHDRAW_EVENT)
    {
        if(this->chessboard->is_can_withdraw())
        {
            this->chessboard->set_chessboard_withdraw();
        }
    }
    else if (event->type == SINGLE_PLAYER_WHITE_EVENT || event->type == SINGLE_PLAYER_BLACK_EVENT || event->type == AGAIN_GAME_EVENT)
    {
        this->chessboard->initChessMap();
        this->chess_data_board->startSingleGame();
        this->chessboard->set_player_flag_type(SINGLE_PLAYER);
    }
    else if(this->handleMouseClick(event) && this->chessboard->get_player_flag_type() == MACHINE_PLAYER)
    {
        machine_thread = SDL_CreateThread(machineChessDown, "machine player", this->machine);
    }
    for (int i = 0; i < this->array_length; i++)
    {
        this->playchess_buttons[i]->handleButtonEvent(event);
    }
    if (this->playchess_buttons[0]->getButtonCurrentSprite() == BUTTON_SPRITE_MOUSE_UP)
    {
        SDL_Event event;
        event.type = PLAYER_WITHDRAW_EVENT;
        SDL_PushEvent(&event);
        this->playchess_buttons[0]->initButtonCurrentSprite();
        INFOLOG("handleEvents||push event=PLAYER_WITHDRAW_EVENT");
    }
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
