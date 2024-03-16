#include "chess_data_board.h"

// 主窗口
extern SDLWindow* g_main_window; 
// 艺术字体 行楷
extern TTF_Font* g_art_font;
// 普通字体
extern TTF_Font* g_normal_font;

ChessDataBoard::ChessDataBoard(const Config& config)
{
    this->symbol_white_chess = new Chess(config, "symbol_white_chess");
    this->symbol_black_chess = new Chess(config, "symbol_black_chess");
    this->data_board_ttf = new SDLTTF("chess_data_ttf");
    this->top_timer = new SDLTimer("top_timer");
    this->data_board_x = config.Read("data_board_x", 0);
    this->data_board_y = config.Read("data_board_y", 0);
    INFOLOG("Chess1 construct success||data_board_x={}||data_board_y={}", 
    this->data_board_x, this->data_board_y);
}

ChessDataBoard::~ChessDataBoard()
{
    delete symbol_white_chess;
	delete symbol_black_chess;
    delete data_board_ttf;
    delete top_timer;
    INFOLOG("~ChessDataBoard success||release resources");
}

void ChessDataBoard::init()
{
    this->symbol_white_chess->init(g_main_window);
	this->symbol_black_chess->init(g_main_window);
    INFOLOG("init||ChessDataBoard init success||load resource success");
}

void ChessDataBoard::initDataBoard(const chess_color_type& type)
{
    for (int i = 0; i < 2; i++)
    {
        this->data_board_arr[i*2] = new ChessDataBoardIterm("temp", this->data_board_x+30, this->data_board_y-50, 0.06, 0.7);
        this->data_board_arr[i*2+1] = new ChessDataBoardIterm("temp", this->data_board_x+30, this->data_board_y+50, 0.06, 0.7);
    }
    switch (type)
    {
    case BLACK_COLOR_TYPE:
        this->data_board_arr[0]->changeItermName("机器人");
        this->data_board_arr[1]->changeItermName("玩家");
        break;
    case WHITE_COLOR_TYPE:
        this->data_board_arr[0]->changeItermName("玩家");
        this->data_board_arr[1]->changeItermName("机器人");
        break;
    case TWO_PLAYERS_COLOR_TYPE:
        this->data_board_arr[2]->changeItermName("白方");
        this->data_board_arr[3]->changeItermName("黑方");
        break;
    default:
        break;
    }
    this->last_chess_sequence = type;
    for (int i = 0; i < 4; i++)
    {
        this->data_board_arr[i]->init();
    }
    INFOLOG("initDataBoard success||chess_color_type={}", (int)type);
}

void ChessDataBoard::resetDataBoard()
{
    INFOLOG("resetDataBoard");
}

// 数据板默认第一行为白棋(后手)，第二行为黑棋(先手)
void ChessDataBoard::render(const player_flag_type& type)
{
    // 渲染右上角解释窗口
    // 默认字体黑色
    SDL_Color color = {0, 0, 0, 0};
    SDL_Color current_color = {0, 255, 0, 0};
    // 窗口头显示时间
    std::stringstream time_text;
    time_text.str("");
    //  除1000，是因为需要的是秒，而每秒有1000毫秒,小数点后取1位小数
    time_text << std::setiosflags(std::ios::fixed) << std::setprecision(1) << (this->top_timer->timerGetTicks() / 1000.f); 
    this->renderText("对局开始(s): ", g_normal_font, color, this->data_board_x-50, this->data_board_y-110, 0.6);  
    this->renderText(time_text.str(), g_normal_font, color,this->data_board_x+80, this->data_board_y-110, 0.6);  //时间信息
    switch (type)
    {
    case MACHINE_PLAYER:
        this->data_board_arr[0]->render(current_color);
        this->data_board_arr[1]->render(color);
        break;
    case SINGLE_PLAYER:
        this->data_board_arr[0]->render(color);
        this->data_board_arr[1]->render(current_color);
        break;
    case BLACK_PLAYER:
        this->data_board_arr[3]->render(current_color);
        this->data_board_arr[2]->render(color);
        break;
    case WHITE_PLAYER:
        this->data_board_arr[3]->render(color);
        this->data_board_arr[2]->render(current_color);
        break;
    default:
        break;
    }
    // 第一行
    this->symbol_white_chess->set_chess_coordinate(this->data_board_x-80, this->data_board_y-50);
    this->symbol_white_chess->set_chess_multiple(0.8);
	this->symbol_white_chess->chessRender();
	// 中间行
    this->renderText("VS", g_art_font, color, this->data_board_x, this->data_board_y, 0.08);
    //最后一行
    this->symbol_black_chess->set_chess_coordinate(this->data_board_x-80, this->data_board_y+50);
    this->symbol_black_chess->set_chess_multiple(0.8);
    this->symbol_black_chess->chessRender();
}

void ChessDataBoard::renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple)
{
	this->data_board_ttf->loadRenderText(g_main_window->getRenderer(), texture_ttf, texture_text, color);
	this->data_board_ttf->ttfRender(g_main_window->getRenderer(), x, y, multiple);
}

void ChessDataBoard::updateScoreInfo(const result_info_type& type)
{
    if (type == SINGLE_PLAYER_WIN)
    {
        this->data_board_arr[1]->addScore();
    }
    else if (type == SINGLE_PLAYER_LOSE)
    {
        this->data_board_arr[0]->addScore();
    }
    else if (type == TWO_PLAYER_BLACK_WIN)
    {
        this->data_board_arr[2]->addScore();
    }
    else if (type == TWO_PLAYER_WHITE_WIN)
    {
        this->data_board_arr[3]->addScore();
    }
    
}

void ChessDataBoard::startSingleGame(const chess_color_type& type)
{
    if (type != this->last_chess_sequence)
    {
        this->data_board_arr[0]->swapField(*this->data_board_arr[1]);
    }
    this->last_chess_sequence = type;
    this->top_timer->timerStart();
    INFOLOG("startSingleGame||start timer||type={}", (int)type);
}

chess_color_type ChessDataBoard::getCurrentChessSequence()
{
    DEBUGLOG("getCurrentChessSequence||current_chess_sequence={}", (int)this->current_chess_sequence);
    return this->current_chess_sequence;
}


