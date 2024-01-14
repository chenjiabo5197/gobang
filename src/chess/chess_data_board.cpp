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

void ChessDataBoard::init(SDL_Window * global_window, SDL_Renderer* global_renderer, TTF_Font* global_font)
{
    this->global_window = global_window;
	this->global_renderer = global_renderer;
	this->global_ttf = global_font;
    this->symbol_white_chess->init(global_window, global_renderer);
	this->symbol_black_chess->init(global_window, global_renderer);
    INFOLOG("init||ChessDataBoard init success||load resource success");
}

void ChessDataBoard::initDataBoard()
{
    
}

void ChessDataBoard::render()
{
    // 渲染右上角解释窗口
	this->symbol_white_chess->chessRender(this->data_board_x, this->data_board_y-100, 0.9);
	this->symbol_black_chess->chessRender(this->data_board_x, this->data_board_y, 0.9);
	this->renderText("机器人", this->data_board_x+150, this->data_board_y-100, 1.0);
	this->renderText("玩家", this->data_board_x+150, this->data_board_y, 1.0);
}

void ChessDataBoard::renderText(const std::string& texture_text, const int& x, const int& y, const float& multiple)
{
	SDL_Color color = {0, 0, 0, 0};
	this->data_board_ttf->loadRenderText(global_renderer, global_ttf, texture_text, color);
	this->data_board_ttf->ttfRender(global_renderer, x, y, multiple);
}
