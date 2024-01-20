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
    this->initDataBoard();
    INFOLOG("init||ChessDataBoard init success||load resource success");
}

void ChessDataBoard::initDataBoard()
{
    this->score_info["machine_name"] = "机器人";
    this->score_info["machine_score"] = "0";
    this->score_info["player_name"] = "玩家";
    this->score_info["player_score"] = "0";
    INFOLOG("initDataBoard success");
}

void ChessDataBoard::render()
{
    // 渲染右上角解释窗口
    // 窗口头显示时间
    std::stringstream time_text;
    time_text.str("");
    //  除1000，是因为需要的是秒，而每秒有1000毫秒,小数点后取1位小数
    time_text << std::setiosflags(std::ios::fixed) << std::setprecision(1) << (this->top_timer->timerGetTicks() / 1000.f); 
    this->renderText("对局开始(s): ", this->normal_ttf, this->data_board_x-50, this->data_board_y-110, 0.6);  
    this->renderText(time_text.str(), this->normal_ttf, this->data_board_x+80, this->data_board_y-110, 0.6);  //时间信息
    // 第一行
    std::string machine_name = this->score_info["machine_name"];
    std::string machine_score = this->score_info["machine_score"];
    this->renderText(machine_score, this->normal_ttf, this->data_board_x-140, this->data_board_y-50, 0.7);  //比分信息
    this->symbol_white_chess->set_chess_coordinate(this->data_board_x-80, this->data_board_y-50);
    this->symbol_white_chess->set_chess_multiple(0.8);
	this->symbol_white_chess->chessRender();
    this->renderText(machine_name, this->art_ttf, this->data_board_x+30, this->data_board_y-50, 0.08);
	// 中间行
    this->renderText("VS", this->art_ttf, this->data_board_x, this->data_board_y, 0.08);
    //最后一行
    std::string player_name = this->score_info["player_name"];
    std::string player_score = this->score_info["player_score"];
    this->renderText(player_score, this->normal_ttf, this->data_board_x-140, this->data_board_y+50, 0.7);  //比分信息
    this->symbol_black_chess->set_chess_coordinate(this->data_board_x-80, this->data_board_y+50);
    this->symbol_black_chess->set_chess_multiple(0.8);
    this->symbol_black_chess->chessRender();
	this->renderText(player_name, this->art_ttf, this->data_board_x+30, this->data_board_y+50, 0.08);
}

void ChessDataBoard::renderText(const std::string& texture_text, TTF_Font* texture_ttf, const int& x, const int& y, const float& multiple)
{
	SDL_Color color = {0, 0, 0, 0};
	this->data_board_ttf->loadRenderText(this->chess_data_window->getRenderer(), texture_ttf, texture_text, color);
	this->data_board_ttf->ttfRender(this->chess_data_window->getRenderer(), x, y, multiple);
}

void ChessDataBoard::updateScoreInfo(const result_info_type& type)
{
    if (type == SINGLE_PLAYER_WIN)
    {
        this->score_info["player_score"] = std::to_string(std::stoi(this->score_info["player_score"]) + 1);
        INFOLOG("updateScoreInfo||current score={}", this->score_info["player_score"]);
    }
    else if (type == SINGLE_PLAYER_LOSE)
    {
        this->score_info["machine_score"] = std::to_string(std::stoi(this->score_info["machine_score"]) + 1);
        INFOLOG("updateScoreInfo||current score={}", this->score_info["machine_score"]);
    }
}

void ChessDataBoard::startSingleGame()
{
    this->top_timer->timerStart();
    INFOLOG("startSingleGame||start timer");
}


