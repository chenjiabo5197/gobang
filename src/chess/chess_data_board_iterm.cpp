#include "chess_data_board_iterm.h"

// 主窗口
extern SDLWindow* g_main_window; 
// 艺术字体 行楷
extern TTF_Font* g_art_font;
// 普通字体
extern TTF_Font* g_normal_font;

ChessDataBoardIterm::ChessDataBoardIterm(const std::string& name, const int& name_x, const int& name_y, const float& name_multiple, const float& score_multiple)
{
    m_iterm_name = name;
    m_iterm_name_x = name_x;
    m_iterm_name_y = name_y;
    m_iterm_name_multiple = name_multiple;
    m_iterm_score = 0;
    m_iterm_score_x = name_x - 170;
    m_iterm_score_y = name_y;
    m_iterm_score_multiple = score_multiple;
    m_data_board_iterm_ttf = new SDLTTF("chess_data_iterm_ttf");
    INFOLOG("ChessDataBoardIterm construct success||iterm_name={}||iterm_name_x={}||iterm_name_y={}||iterm_name_multiple={}||iterm_score={}||iterm_score_x={}||iterm_score_y={}||iterm_score_multiple={}",
    m_iterm_name, m_iterm_name_x, m_iterm_name_y, m_iterm_name_multiple, m_iterm_score, m_iterm_score_x, m_iterm_score_y, m_iterm_score_multiple);
}

ChessDataBoardIterm::~ChessDataBoardIterm()
{
    if (m_data_board_iterm_ttf != nullptr)
    {
        delete m_data_board_iterm_ttf;
        m_data_board_iterm_ttf = nullptr;
    }
    INFOLOG("~ChessDataBoardIterm success, release resource||iterm_name={}", m_iterm_name);
}

void ChessDataBoardIterm::init()
{
    DEBUGLOG("init||ChessDataBoardIterm init success||iterm_name={}", m_iterm_name);
}


void ChessDataBoardIterm::render(SDL_Color color)
{
    this->renderText(m_iterm_name, g_art_font, color, m_iterm_name_x, m_iterm_name_y, m_iterm_name_multiple);
    this->renderText(std::to_string(m_iterm_score), g_normal_font, color, m_iterm_score_x, m_iterm_score_y, m_iterm_score_multiple);
}

void ChessDataBoardIterm::addScore()
{
    m_iterm_score++;
    INFOLOG("addScore||iterm_name={}||iterm_score={}", m_iterm_name, m_iterm_score);
}

void ChessDataBoardIterm::renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple)
{
	m_data_board_iterm_ttf->loadRenderText(g_main_window->getRenderer(), texture_ttf, texture_text, color);
	m_data_board_iterm_ttf->ttfRender(g_main_window->getRenderer(), x, y, multiple);
}

void ChessDataBoardIterm::swapField(ChessDataBoardIterm& iterm)
{
    // 名字
    // swapValue(m_iterm_name, iterm.iterm_name);
    // 要渲染的文字坐标
    swapValue(m_iterm_name_x, iterm.m_iterm_name_x);
    swapValue(m_iterm_name_y, iterm.m_iterm_name_y);
    // 文字缩放倍数
    swapValue(m_iterm_name_multiple, iterm.m_iterm_name_multiple);
    // 分数
    // swapValue(m_iterm_score, iterm.iterm_score);
    // 要渲染的文字坐标
    swapValue(m_iterm_score_x, iterm.m_iterm_score_x);
    swapValue(m_iterm_score_y, iterm.m_iterm_score_y);
    // 文字缩放倍数
    swapValue(m_iterm_score_multiple, iterm.m_iterm_score_multiple);
    INFOLOG("swapField||name={}||iterm.name={}", m_iterm_name, iterm.m_iterm_name);
}

void ChessDataBoardIterm::changeItermName(const std::string& new_name)
{
    m_iterm_name = new_name;
}
