#include "chess_data_board.h"

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

void ChessDataBoard::init(SDLWindow* chess_data_window, TTF_Font* normal_font, TTF_Font* art_font)
{
    this->chess_data_window = chess_data_window;
	this->normal_ttf = normal_font;
    this->art_ttf = art_font;
    this->symbol_white_chess->init(this->chess_data_window);
	this->symbol_black_chess->init(this->chess_data_window);
    INFOLOG("init||ChessDataBoard init success||load resource success");
}

void ChessDataBoard::initDataBoard(const chess_color_type& type)
{
    switch (type)
    {
    case BLACK_COLOR_TYPE:
        this->data_board_arr[0] = new ChessDataBoardIterm("机器人", this->data_board_x+30, this->data_board_y-50, 0.06, 0.7);
        this->data_board_arr[1] = new ChessDataBoardIterm("玩家", this->data_board_x+30, this->data_board_y+50, 0.06, 0.7);
        break;
    case WHITE_COLOR_TYPE:
        this->data_board_arr[0] = new ChessDataBoardIterm("机器人", this->data_board_x+30, this->data_board_y+50, 0.06, 0.7);
        this->data_board_arr[1] = new ChessDataBoardIterm("玩家", this->data_board_x+30, this->data_board_y-50, 0.06, 0.7);
        break;
    case TWO_PLAYERS_COLOR_TYPE:
        this->data_board_arr[0] = new ChessDataBoardIterm("玩家1", this->data_board_x+30, this->data_board_y+50, 0.06, 0.7);
        this->data_board_arr[1] = new ChessDataBoardIterm("玩家2", this->data_board_x+30, this->data_board_y-50, 0.06, 0.7);
        break;
    default:
        break;
    }
    this->last_chess_sequence = type;
    for (int i = 0; i < 2; i++)
    {
        this->data_board_arr[i]->init(this->chess_data_window, this->normal_ttf, this->art_ttf);
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
    this->renderText("对局开始(s): ", this->normal_ttf, color, this->data_board_x-50, this->data_board_y-110, 0.6);  
    this->renderText(time_text.str(), this->normal_ttf, color,this->data_board_x+80, this->data_board_y-110, 0.6);  //时间信息
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
        this->data_board_arr[0]->render(current_color);
        this->data_board_arr[1]->render(color);
        break;
    case WHITE_PLAYER:
        this->data_board_arr[0]->render(color);
        this->data_board_arr[1]->render(current_color);
        break;
    default:
        break;
    }
    // 第一行
    this->symbol_white_chess->set_chess_coordinate(this->data_board_x-80, this->data_board_y-50);
    this->symbol_white_chess->set_chess_multiple(0.8);
	this->symbol_white_chess->chessRender();
	// 中间行
    this->renderText("VS", this->art_ttf, color, this->data_board_x, this->data_board_y, 0.08);
    //最后一行
    this->symbol_black_chess->set_chess_coordinate(this->data_board_x-80, this->data_board_y+50);
    this->symbol_black_chess->set_chess_multiple(0.8);
    this->symbol_black_chess->chessRender();
}

void ChessDataBoard::renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple)
{
	this->data_board_ttf->loadRenderText(this->chess_data_window->getRenderer(), texture_ttf, texture_text, color);
	this->data_board_ttf->ttfRender(this->chess_data_window->getRenderer(), x, y, multiple);
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


