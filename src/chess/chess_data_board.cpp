#include "chess_data_board.h"

ChessDataBoard::ChessDataBoard(const Config& config)
{
    this->symbol_white_chess = new Chess(config, "symbol_white_chess");
    this->symbol_black_chess = new Chess(config, "symbol_black_chess");
    this->data_board_ttf = new SDLTTF("chess_data_ttf");
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
    INFOLOG("~ChessDataBoard success||release resources");
}

void ChessDataBoard::init(SDL_Window * global_window, SDL_Renderer* global_renderer, TTF_Font* normal_font, TTF_Font* art_font)
{
    this->global_window = global_window;
	this->global_renderer = global_renderer;
	this->normal_ttf = normal_font;
    this->art_ttf = art_font;
    this->symbol_white_chess->init(global_window, global_renderer);
	this->symbol_black_chess->init(global_window, global_renderer);
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
    // 第一行
    std::string machine_name = this->score_info["machine_name"];
    std::string machine_score = this->score_info["machine_score"];
    this->renderText(machine_score, this->normal_ttf, this->data_board_x-140, this->data_board_y-50, 0.7);  //比分信息
	this->symbol_white_chess->chessRender(this->data_board_x-80, this->data_board_y-50, 0.8);
    this->renderText(machine_name, this->art_ttf, this->data_board_x+30, this->data_board_y-50, 0.08);
	// 中间行
    this->renderText("VS", this->art_ttf, this->data_board_x, this->data_board_y, 0.08);
    //最后一行
    std::string player_name = this->score_info["player_name"];
    std::string player_score = this->score_info["player_score"];
    this->renderText(player_score, this->normal_ttf, this->data_board_x-140, this->data_board_y+50, 0.7);  //比分信息
    this->symbol_black_chess->chessRender(this->data_board_x-80, this->data_board_y+50, 0.8);
	this->renderText(player_name, this->art_ttf, this->data_board_x+30, this->data_board_y+50, 0.08);
}

void ChessDataBoard::renderText(const std::string& texture_text, TTF_Font* texture_ttf, const int& x, const int& y, const float& multiple)
{
	SDL_Color color = {0, 0, 0, 0};
	this->data_board_ttf->loadRenderText(this->global_renderer, texture_ttf, texture_text, color);
	this->data_board_ttf->ttfRender(this->global_renderer, x, y, multiple);
}

void ChessDataBoard::updateScoreInfo(const result_info_type& type)
{
    if (type == SINGLE_PLAYER_WIN)
    {
        this->score_info["player_score"] = std::to_string(std::stoi(this->score_info["player_score"]) + 1);
    }
    else if (type == SINGLE_PLAYER_LOSE)
    {
        this->score_info["machine_score"] = std::to_string(std::stoi(this->score_info["machine_score"]) + 1);
    }
    
}
