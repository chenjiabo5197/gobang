#include "chess_data_board.h"

// 主窗口
extern SDLWindow* g_main_window; 
// 艺术字体 行楷
extern TTF_Font* g_art_font;
// 普通字体
extern TTF_Font* g_normal_font;

ChessDataBoard::ChessDataBoard(const Config& config)
{
    m_symbol_white_chess = new Chess(config, "symbol_white_chess");
    m_symbol_black_chess = new Chess(config, "symbol_black_chess");
    m_data_board_ttf = new SDLTTF("chess_data_ttf");
    m_top_timer = new SDLTimer("top_timer");
    m_data_board_x = config.Read("data_board_x", 0);
    m_data_board_y = config.Read("data_board_y", 0);
    INFOLOG("Chess1 construct success||data_board_x={}||data_board_y={}", 
    m_data_board_x, m_data_board_y);
}

ChessDataBoard::~ChessDataBoard()
{
    delete m_symbol_white_chess;
	delete m_symbol_black_chess;
    delete m_data_board_ttf;
    delete m_top_timer;
    INFOLOG("~ChessDataBoard success||release resources");
}

void ChessDataBoard::init()
{
    m_symbol_white_chess->init(g_main_window);
	m_symbol_black_chess->init(g_main_window);
    INFOLOG("init||ChessDataBoard init success||load resource success");
}

void ChessDataBoard::initDataBoard(const chess_color_type& type)
{
    for (int i = 0; i < 2; i++)
    {
        m_data_board_arr[i*2] = new ChessDataBoardIterm("temp", m_data_board_x+30, m_data_board_y-50, 0.06, 0.7);
        m_data_board_arr[i*2+1] = new ChessDataBoardIterm("temp", m_data_board_x+30, m_data_board_y+50, 0.06, 0.7);
    }
    switch (type)
    {
    case BLACK_COLOR_TYPE:
        m_data_board_arr[0]->changeItermName("机器人");
        m_data_board_arr[1]->changeItermName("玩家");
        break;
    case WHITE_COLOR_TYPE:
        m_data_board_arr[0]->changeItermName("玩家");
        m_data_board_arr[1]->changeItermName("机器人");
        break;
    case TWO_PLAYERS_COLOR_TYPE:
        m_data_board_arr[2]->changeItermName("白方");
        m_data_board_arr[3]->changeItermName("黑方");
        break;
    default:
        break;
    }
    m_last_chess_sequence = type;
    for (int i = 0; i < 4; i++)
    {
        m_data_board_arr[i]->init();
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
    time_text << std::setiosflags(std::ios::fixed) << std::setprecision(1) << (m_top_timer->timerGetTicks() / 1000.f); 
    this->renderText("对局开始(s): ", g_normal_font, color, m_data_board_x-50, m_data_board_y-110, 0.6);  
    this->renderText(time_text.str(), g_normal_font, color,m_data_board_x+80, m_data_board_y-110, 0.6);  //时间信息
    switch (type)
    {
    case MACHINE_PLAYER:
        m_data_board_arr[0]->render(current_color);
        m_data_board_arr[1]->render(color);
        break;
    case SINGLE_PLAYER:
        m_data_board_arr[0]->render(color);
        m_data_board_arr[1]->render(current_color);
        break;
    case BLACK_PLAYER:
        m_data_board_arr[3]->render(current_color);
        m_data_board_arr[2]->render(color);
        break;
    case WHITE_PLAYER:
        m_data_board_arr[3]->render(color);
        m_data_board_arr[2]->render(current_color);
        break;
    default:
        break;
    }
    // 第一行
    m_symbol_white_chess->setChessCoordinate(m_data_board_x-80, m_data_board_y-50);
    m_symbol_white_chess->setChessMultiple(0.8);
	m_symbol_white_chess->chessRender();
	// 中间行
    this->renderText("VS", g_art_font, color, m_data_board_x, m_data_board_y, 0.08);
    //最后一行
    m_symbol_black_chess->setChessCoordinate(m_data_board_x-80, m_data_board_y+50);
    m_symbol_black_chess->setChessMultiple(0.8);
    m_symbol_black_chess->chessRender();
}

void ChessDataBoard::renderText(const std::string& texture_text, TTF_Font* texture_ttf, SDL_Color color, const int& x, const int& y, const float& multiple)
{
	m_data_board_ttf->loadRenderText(g_main_window->getRenderer(), texture_ttf, texture_text, color);
	m_data_board_ttf->ttfRender(g_main_window->getRenderer(), x, y, multiple);
}

void ChessDataBoard::updateScoreInfo(const result_info_type& type)
{
    switch (type)
    {
    case SINGLE_PLAYER_WIN:
        m_data_board_arr[1]->addScore();
        break;
    case SINGLE_PLAYER_LOSE:
        m_data_board_arr[0]->addScore();
        break;
    case TWO_PLAYER_BLACK_WIN:
        m_data_board_arr[2]->addScore();
        break;
    case TWO_PLAYER_WHITE_WIN:
        m_data_board_arr[3]->addScore();
        break;
    default:
        ERRORLOG("updateScoreInfo||unknown type");
        break;
    }    
}

void ChessDataBoard::startSingleGame(const chess_color_type& type)
{
    if (type != m_last_chess_sequence)
    {
        m_data_board_arr[0]->swapField(*m_data_board_arr[1]);
    }
    m_last_chess_sequence = type;
    m_top_timer->timerStart();
    INFOLOG("startSingleGame||start timer||type={}", (int)type);
}

chess_color_type ChessDataBoard::getCurrentChessSequence()
{
    DEBUGLOG("getCurrentChessSequence||current_chess_sequence={}", (int)m_current_chess_sequence);
    return m_current_chess_sequence;
}


