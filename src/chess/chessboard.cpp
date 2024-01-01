#include "chessboard.h"

Chessboard::Chessboard(const Config& config)
{
    std::string temp;
    this->origin_x = config.Read("chessboard_origin_x", 0);
    this->origin_y = config.Read("chessboard_origin_y", 0);
    this->lattice_size = config.Read("chessboard_lattice_size", 0);
    std::string white_chess_path = config.Read("white_chess_resource_path", temp);
    std::string black_chess_path = config.Read("black_chess_resource_path", temp);
    this->white_chess = new Chess(config, "white_chess", white_chess_path, origin_x, origin_y, lattice_size);
    this->black_chess = new Chess(config, "black_chess", black_chess_path, origin_x, origin_y, lattice_size);
    INFOLOG("Chessboard construct success||origin_x={}||origin_y={}||lattice_size={}||white_chess_path={}||black_chess_path={}", this->origin_x, this->origin_y, this->lattice_size, white_chess_path, black_chess_path);
}

Chessboard::~Chessboard()
{
    delete(white_chess);
    delete(black_chess);
    INFOLOG("~Chessboard success||release resources");
}

bool Chessboard::renderPlayChessInterface(SDL_Renderer* gRenderer)
{
    // 渲染棋盘背景色
    SDL_Rect chessboard_backgroud = {this->origin_x, this->origin_y, this->lattice_size*14, this->lattice_size*14};
    SDL_SetRenderDrawColor(gRenderer, 255, 246, 143, 0xFF);        
    SDL_RenderFillRect(gRenderer, &chessboard_backgroud);

    // 渲染棋盘边界正方形,五子棋盘一共有15*15行，一共是14*14个间距,此处+1是为了渲染棋盘边框两次，使棋盘边框线看起来粗一点
    SDL_Rect chessboard_boundary = {this->origin_x+1, this->origin_y+1, this->lattice_size*14+1, this->lattice_size*14+1};
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);        
    SDL_RenderDrawRect(gRenderer, &chessboard_boundary);
    
    //渲染棋盘网格线
    for (int i = 0; i < 15; i++)
    {
        // 横向的棋盘线
        SDL_RenderDrawLine(gRenderer, this->origin_x, this->lattice_size*i+this->origin_y, this->lattice_size*14+this->origin_x, this->lattice_size*i+this->origin_y);
        // 纵向的棋盘线
        SDL_RenderDrawLine(gRenderer, this->lattice_size*i+this->origin_x, this->origin_y, this->lattice_size*i+this->origin_x, this->lattice_size*14+this->origin_y);
    }
    // 渲染棋盘上五个圆点
    this->renderCircle(gRenderer, this->lattice_size*7+this->origin_x, this->lattice_size*7+this->origin_y, 5);
    this->renderCircle(gRenderer, this->lattice_size*3+this->origin_x, this->lattice_size*3+this->origin_y, 5);
    this->renderCircle(gRenderer, this->lattice_size*3+this->origin_x, this->lattice_size*11+this->origin_y, 5);
    this->renderCircle(gRenderer, this->lattice_size*11+this->origin_x, this->lattice_size*3+this->origin_y, 5);
    this->renderCircle(gRenderer, this->lattice_size*11+this->origin_x, this->lattice_size*11+this->origin_y, 5);
    return true;
}

void Chessboard::renderCircle(SDL_Renderer* gRenderer, const int& x, const int& y, const int& radius)
{
    INFOLOG("renderCircle||x={}||y={}||radius={}", x, y, radius);
    for (int k = 0; k < 2; k++)  // 向上和向下各渲染一次
    {
        for (int i = 1; i < radius; i++)
        {
            int row_point_nums = 2*radius+1-2*i;
            for (int j = 1; j <= row_point_nums; j++)
            {
                int temp_x = j-(radius+1-i) + x;
                int temp_y = 0;
                if (k == 0)
                {
                    temp_y = y-i;
                }
                else
                {
                    temp_y = y+i;
                }
                SDL_RenderDrawPoint(gRenderer, temp_x, temp_y);
                DEBUGLOG("renderCircle||temp_x={}||temp_y={}||row_point_nums={}", temp_x, temp_y, row_point_nums);
            }
        }
    }     
}
