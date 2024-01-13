#include "playchess_manage.h"

//多线程锁
SDL_SpinLock player_machine_lock = 0;

SDL_Thread* machine_thread = nullptr;

PlaychessManage::PlaychessManage(const Config& config)
{
    this->chessboard = new Chessboard(config);
    this->machine = new Machine(this->chessboard);
    this->single_player = new Player(this->chessboard, "single_player", CHESS_BLACK);
    this->back_menu_button = new SDLButton(config, "back_menu");
    this->withdraw_button = new SDLButton(config, "withdraw");
    this->button_interval = config.Read("playchess_buttons_interval", 0);
    DEBUGLOG("PlaychessManage construct success||button_interval={}", this->button_interval);
}

PlaychessManage::~PlaychessManage()
{
    delete chessboard;
    delete machine;
    delete single_player;
    delete back_menu_button;
    delete withdraw_button;
    SDL_WaitThread(machine_thread, nullptr);
    DEBUGLOG("~PlaychessManage success||release resource");
}

void PlaychessManage::loadResource()
{
    this->back_menu_button->loadResource(this->global_window, this->global_renderer);
    this->withdraw_button->loadResource(this->global_window, this->global_renderer);
    INFOLOG("loadResource||load resource success");
}

void PlaychessManage::init(SDL_Window* global_window, SDL_Renderer* global_renderer)
{
    this->global_window = global_window;
    this->global_renderer = global_renderer;
    this->back_menu_button->initButtonCurrentSprite();
    this->withdraw_button->initButtonCurrentSprite();
    INFOLOG("init||init variable success");
}

void PlaychessManage::startRender()
{
    SDL_SetRenderDrawColor(this->global_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(this->global_renderer);
    this->chessboard->render(this->global_window, this->global_renderer);
    this->withdraw_button->buttonRender(this->global_renderer);
    this->back_menu_button->buttonRender(this->global_renderer);
    //Update screen
    SDL_RenderPresent(this->global_renderer);
    // DEBUGLOG("startRender");
}

// Machine类的友元函数
int machineChessDown(void* data)
{
    SDL_AtomicLock(&player_machine_lock);
	Machine* machine = (Machine *)(data);
    ChessPos pos = machine->think();
    // 程序休眠1s，假装在思考
    MyUtils::sleep_seconds(1);
	// mciSendString("play res/chess_down.mp3", 0, 0, 0);
	machine->chessboard->chessDown(pos, CHESS_WHITE);
    if(machine->chessboard->checkOver())
    {
        SDL_Event event;
        event.type = PLAYER_LOSE_EVENT;
        SDL_PushEvent(&event);
    }
    machine->chessboard->set_player_flag_type(SINGLE_PLAYER);
    SDL_FlushEvents(SDL_APP_TERMINATING, SDL_POLLSENTINEL);
    SDL_AtomicUnlock(&player_machine_lock);
    DEBUGLOG("Machine::go||Machine chess down success");
    return 0;
}

bool PlaychessManage::handleMouseClick(SDL_Event* event)
{
    SDL_AtomicLock(&player_machine_lock);
    if (event->type == SDL_MOUSEBUTTONDOWN && this->chessboard->get_player_flag_type() == SINGLE_PLAYER)  // 鼠标点击事件
    {
        //获取鼠标位置
        int x, y;
        SDL_GetMouseState(&x, &y);
        DEBUGLOG("handleMouseClick2||x={}||y={}", x, y);
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
        SDL_AtomicUnlock(&player_machine_lock);
        return is_valid_click;
    }
    SDL_AtomicUnlock(&player_machine_lock);
    return false;
}

bool PlaychessManage::handleEvents(SDL_Event* event)
{
    if(this->handleMouseClick(event) && this->chessboard->get_player_flag_type() == MACHINE_PLAYER)
    {
        machine_thread = SDL_CreateThread(machineChessDown, "machine player", this->machine);
    }
    this->back_menu_button->handleButtonEvent(event);
    this->withdraw_button->handleButtonEvent(event);
    return true;
}
