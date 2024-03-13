#include "chess_data_board_iterm.h"

// 主窗口
extern SDLWindow* main_window; 

ChessDataBoardIterm::ChessDataBoardIterm(const std::string& name, const int& name_x, const int& name_y, const float& name_multiple, const float& score_multiple)
{
    this->iterm_name = name;
    this->iterm_name_x = name_x;
    this->iterm_name_y = name_y;
    this->iterm_name_multiple = name_multiple;
    this->iterm_score = 0;
    this->iterm_score_x = name_x - 170;
    this->iterm_score_y = name_y;
    this->iterm_score_multiple = score_multiple;
    this->data_board_iterm_ttf = new SDLTTF("chess_data_iterm_ttf");
    INFOLOG("ChessDataBoardIterm construct success||iterm_name={}||iterm_name_x={}||iterm_name_y={}||iterm_name_multiple={}||iterm_score={}||iterm_score_x={}||iterm_score_y={}||iterm_score_multiple={}",
    this->iterm_name, this->iterm_name_x, this->iterm_name_y, this->iterm_name_multiple, this->iterm_score, this->iterm_score_x, this->iterm_score_y, this->iterm_score_multiple);
}

ChessDataBoardIterm::~ChessDataBoardIterm()
{
    if (this->data_board_iterm_ttf != nullptr)
    {
        delete this->data_board_iterm_ttf;
        this->data_board_iterm_ttf = nullptr;
    }
    INFOLOG("~ChessDataBoardIterm success, release resource||iterm_name={}", this->iterm_name);
}

void ChessDataBoardIterm::init(TTF_Font* normal_font, TTF_Font* art_font)
{
	this->normal_ttf = normal_font;
    this->art_ttf = art_font;
    DEBUGLOG("init||ChessDataBoardIterm init success||iterm_name={}", this->iterm_name);
}


void ChessDataBoardIterm::render(SDL_Color color)
{
    this->renderText(iterm_name, this->art_ttf, color, this->iterm_name_x, this->iterm_name_y, this->iterm_name_multiple);
    this->renderText(std::to_string(this->iterm_score), this->normal_ttf, color, this->iterm_score_x, this->iterm_score_y, this->iterm_score_multiple);
}

void ChessDataBoardIterm::addScore()
{
    this->iterm_score++;
    INFOLOG("addScore||iterm_name={}||iterm_score={}", this->iterm_name, this->iterm_score);
}

void ChessDataBoardIterm::renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple)
{
	this->data_board_iterm_ttf->loadRenderText(main_window->getRenderer(), texture_ttf, texture_text, color);
	this->data_board_iterm_ttf->ttfRender(main_window->getRenderer(), x, y, multiple);
}

void ChessDataBoardIterm::swapField(ChessDataBoardIterm& iterm)
{
    // 名字
    // MyUtils::swapValue(this->iterm_name, iterm.iterm_name);
    // 要渲染的文字坐标
    MyUtils::swapValue(this->iterm_name_x, iterm.iterm_name_x);
    MyUtils::swapValue(this->iterm_name_y, iterm.iterm_name_y);
    // 文字缩放倍数
    MyUtils::swapValue(this->iterm_name_multiple, iterm.iterm_name_multiple);
    // 分数
    // MyUtils::swapValue(this->iterm_score, iterm.iterm_score);
    // 要渲染的文字坐标
    MyUtils::swapValue(this->iterm_score_x, iterm.iterm_score_x);
    MyUtils::swapValue(this->iterm_score_y, iterm.iterm_score_y);
    // 文字缩放倍数
    MyUtils::swapValue(this->iterm_score_multiple, iterm.iterm_score_multiple);
    INFOLOG("swapField||name={}||iterm.name={}", this->iterm_name, iterm.iterm_name);
}
