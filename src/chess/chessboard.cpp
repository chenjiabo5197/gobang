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
	this->chessboard_size = 15;
	this->last_chess_pos = new ChessPos(); 
	this->player_flag = DEFAULT_PLAYER;
    // 初始化棋盘，棋盘每个位置都为0，表示空白，标准五子棋盘为15*15
	for (int i = 0; i < chessboard_size; i++)
	{
		std::vector<int> row;
		for (int j = 0; j < chessboard_size; j++)
		{
			row.push_back(0);
		}
		chessMap.push_back(row);
	}
	this->initChessBoardBoundary();
    INFOLOG("Chessboard construct success||origin_x={}||origin_y={}||lattice_size={}||white_chess_path={}||black_chess_path={}", 
	this->origin_x, this->origin_y, this->lattice_size, white_chess_path, black_chess_path);
}

Chessboard::~Chessboard()
{
    delete white_chess;
    delete black_chess;
	delete chessboard_boundary;
	delete last_chess_pos;
    INFOLOG("~Chessboard success||release resources");
}

void Chessboard::initChessMap()
{
	// 重置棋盘，棋盘每个位置都为0，表示空白，标准五子棋盘为15*15
	for (int i = 0; i < chessMap.size(); i++)
	{
		for (int j = 0; j < chessMap[i].size(); j++)
		{
			chessMap[i][j] = 0;
		}
	}
    INFOLOG("Chessboard init success");
}

void Chessboard::initChessBoardBoundary()
{
	this->chessboard_boundary = new ChessBoardBoundary();
	this->chessboard_boundary->left_top_x = this->origin_x-this->lattice_size/2;
	this->chessboard_boundary->left_top_y = this->origin_y-this->lattice_size/2;
	this->chessboard_boundary->right_top_x = this->origin_x+this->lattice_size*14+this->lattice_size/2;
	this->chessboard_boundary->right_top_y = this->origin_y-this->lattice_size/2;
	this->chessboard_boundary->left_bottom_x = this->origin_x-this->lattice_size/2;
	this->chessboard_boundary->left_bottom_y = this->origin_y+this->lattice_size*14+this->lattice_size/2;
	this->chessboard_boundary->right_bottom_x = this->origin_x+this->lattice_size*14+this->lattice_size/2;
	this->chessboard_boundary->right_bottom_y = this->origin_y+this->lattice_size*14+this->lattice_size/2;
	INFOLOG("initChessBoardBoundary||left_top_x={}||left_top_y={}||right_top_x={}||right_top_y={}||left_bottom_x={}||left_bottom_y={}||right_bottom_x={}||right_bottom_y={}", 
	this->chessboard_boundary->left_top_x, this->chessboard_boundary->left_top_y, this->chessboard_boundary->right_top_x, 
	this->chessboard_boundary->right_top_y, this->chessboard_boundary->left_bottom_x, this->chessboard_boundary->left_bottom_y,
	this->chessboard_boundary->right_bottom_x, this->chessboard_boundary->right_bottom_y);
}

bool Chessboard::renderPlayChessInterface(SDL_Renderer* gRenderer)
{
    // 渲染棋盘背景色
    SDL_Rect chessboard_backgroud = {this->chessboard_boundary->left_top_x, this->chessboard_boundary->left_top_y, 
	this->chessboard_boundary->right_top_x-this->chessboard_boundary->left_top_x, this->chessboard_boundary->left_bottom_y-this->chessboard_boundary->left_top_y};
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
    // DEBUGLOG("renderPlayChessInterface||render chessboard success!");
    return true;
}

void Chessboard::renderCircle(SDL_Renderer* gRenderer, const int& x, const int& y, const int& radius)
{
    // DEBUGLOG("renderCircle||x={}||y={}||radius={}", x, y, radius);
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
                // DEBUGLOG("renderCircle||temp_x={}||temp_y={}||row_point_nums={}", temp_x, temp_y, row_point_nums);
            }
        }
    }     
}

bool Chessboard::isClickOnChessBoard(const int& x, const int& y)
{
	if (x < this->chessboard_boundary->left_top_x)
	{
		DEBUGLOG("isClickOnChessBoard||x too small");
		return false;
	}
	else if (x > this->chessboard_boundary->right_top_x)
	{
		DEBUGLOG("isClickOnChessBoard||x too big");
		return false;
	}
	else if (y < this->chessboard_boundary->left_top_y)
	{
		DEBUGLOG("isClickOnChessBoard||y too small");
		return false;
	}
	else if (y > this->chessboard_boundary->left_bottom_y)
	{
		DEBUGLOG("isClickOnChessBoard||y too big");
		return false;
	}
	INFOLOG("isClickOnChessBoard||click on chessboard");
	return true;
}

bool Chessboard::clickBoard(const int& x, const int& y, ChessPos* pos)
{
	if (!this->isClickOnChessBoard(x, y))
	{
		return false;
	}
	
    // 鼠标点击在棋盘上的行，列数(用鼠标坐标减去棋盘边缘再除每个方格大小)
	int row = (x - this->origin_x) / this->lattice_size;  // 行数
	int col = (y - this->origin_y) / this->lattice_size;  // 列数

	DEBUGLOG("clickBoard||row={}||col={}||x={}||y={}", row, col, x, y);

	// easyx的坐标原点在左上角，y轴向下，x轴向右
	// 鼠标点击坐标的方格左上角坐标
	int leftTopPosX = this->origin_x + this->lattice_size * row;
	int leftTopPosY = this->origin_y + this->lattice_size * col;

	// 鼠标点击坐标的方格右上角坐标
	int rightTopPosX = this->origin_x + this->lattice_size * (row + 1);
	int rightTopPosY = this->origin_y + this->lattice_size * col;

	// 鼠标点击坐标的方格左下角坐标
	int leftBottomPosX = this->origin_x + this->lattice_size * row;
	int leftBottomPosY = this->origin_y + this->lattice_size * (col + 1);

	// 鼠标点击坐标的方格右下角坐标
	int rightBottomPosX = this->origin_x + this->lattice_size * (row + 1);
	int rightBottomPosY = this->origin_y + this->lattice_size * (col + 1);

	int offset = this->lattice_size * 0.4;  // 20 鼠标点击的模糊距离上限

	int len;
	bool selectPos = false;
	chess_coordinate flag_coord = COORD_DEFAULT;

	INFOLOG("clickBoard||row={}||col={}||leftTopPosX={}||leftTopPosY={}||rightTopPosX={}||rightTopPosY={}||leftBottomPosX={}||leftBottomPosY={}||rightBottomPosX={}||rightBottomPosY={}||offset={}",
		row, col, leftTopPosX, leftTopPosY, rightTopPosX, rightTopPosY, leftBottomPosX, leftBottomPosY, rightBottomPosX, rightBottomPosY, offset);

	do
	{
		// 距左上角位置的距离
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset)
		{
			flag_coord = LEFTTOP;
			pos->chess_row = row;
			pos->chess_col = col;
			if (chessMap[pos->chess_row][pos->chess_col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("clickBoard||leftTop||len={}", len);

		// 距右上角位置的距离
		len = sqrt((x - rightTopPosX) * (x - rightTopPosX) + (y - rightTopPosY) * (y - rightTopPosY));
		if (len < offset)
		{
			flag_coord = RIGHTTOP;
			pos->chess_row = row + 1;
			pos->chess_col = col;
			if (chessMap[pos->chess_row][pos->chess_col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("clickBoard||rightTop||len={}", len);

		// 距左下角位置的距离
		len = sqrt((x - leftBottomPosX) * (x - leftBottomPosX) + (y - leftBottomPosY) * (y - leftBottomPosY));
		if (len < offset)
		{
			flag_coord = LEFTBOTTOM;
			pos->chess_row = row;
			pos->chess_col = col + 1;
			if (chessMap[pos->chess_row][pos->chess_col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("clickBoard||leftBottom||len={}", len);

		// 距右下角位置的距离
		len = sqrt((x - rightBottomPosX) * (x - rightBottomPosX) + (y - rightBottomPosY) * (y - rightBottomPosY));
		if (len < offset)
		{
			flag_coord = RIGHTBOTTOM;
			pos->chess_row = row + 1;
			pos->chess_col = col + 1;
			if (chessMap[pos->chess_row][pos->chess_col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("clickBoard||rightBottom||len={}", len);

	} while (0);
	INFOLOG("clickBoard||flag_coord={}||len={}||pos.row={}||pos.col={}", (int)flag_coord, len, pos->chess_row, pos->chess_col);

	return selectPos;
}

void Chessboard::chessDown(const ChessPos& chessPos, const chess_kind_type& kind)
{
    if (kind == CHESS_WHITE)
    {
        this->chessMap[chessPos.chess_row][chessPos.chess_col] = 1;
        DEBUGLOG("chessDown||WHITE_CHESS||row={}||col={}", chessPos.chess_row, chessPos.chess_col);
    }
    else
    {
        this->chessMap[chessPos.chess_row][chessPos.chess_col] = 2; 
        DEBUGLOG("chessDown||BLACK_CHESS||row={}||col={}", chessPos.chess_row, chessPos.chess_col);
    }
	this->last_chess_pos->chess_row = chessPos.chess_row;
	this->last_chess_pos->chess_col = chessPos.chess_col;
	ChessData temp{chessPos.chess_row, chessPos.chess_col, kind};
	this->chessboard_data.push_back(temp);
	INFOLOG("chessDown||chessboard_data.size={}", chessboard_data.size());
}

void Chessboard::render(SDL_Window * gWindow, SDL_Renderer* gRenderer)
{
    // 先渲染棋盘，再渲染棋盘上的棋子
    this->renderPlayChessInterface(gRenderer);
    this->white_chess->loadResource(gWindow, gRenderer);
    this->black_chess->loadResource(gWindow, gRenderer);
	for (int i = 0; i < chessMap.size(); i++)
	{
		for (int j = 0; j < chessMap[i].size(); j++)
		{
			if (chessMap[i][j] == 1)
			{
				this->white_chess->chessRender(gRenderer, i, j);
			}
			else if (chessMap[i][j] == 2)
			{
				this->black_chess->chessRender(gRenderer, i, j);
			}
		}
	}
}

int Chessboard::getChessBoardSize()
{
	return chessboard_size;
}

int Chessboard::getChessData(ChessPos* pos)
{
	return chessMap[pos->chess_row][pos->chess_col];
}

int Chessboard::getChessData(int row, int col)
{
	return chessMap[row][col];
}

void Chessboard::set_player_flag_type(const player_flag_type& type)
{
	INFOLOG("set_player_flag_type||current player_flag={}||next type={}", (int)this->player_flag, (int)type);
	this->player_flag = type;
}

player_flag_type Chessboard::get_player_flag_type()
{
	// INFOLOG("get_player_flag_type||type={}", (int)this->player_flag);
	return this->player_flag;
}

bool Chessboard::checkOver()
{
	if (checkWin())
	{
		// MyUtils::sleep_seconds(1.5);
		if (player_flag == SINGLE_PLAYER)   // 玩家赢
		{
			INFOLOG("Chess::checkOver||single player win");
			// resultFlag = PLAYER_WIN;
			return true;
		}
		else if (player_flag == MACHINE_PLAYER)  // AI赢
		{
			INFOLOG("Chess::checkOver||machine win");
			// resultFlag = PLAYER_LOSE;
			return true;
		}
	}
	// 判断棋盘是否满了，13*13的棋盘，最多169颗棋子
	// if (this->chessBoardData.size() == 169)
	// {
	// 	INFOLOG("Chess::checkOver||result draw");
	// 	resultFlag = RESULT_DRAW;
	// 	return true;
	// }
	return false;
}

bool Chessboard::checkWin()
{
	// 横竖左斜右斜四个方向，每种情况都根据当前落子向后遍历5个棋子，有一种符合就算赢
	int row = last_chess_pos->chess_row;
	int col = last_chess_pos->chess_col;

	// 水平方向, 向左和右分别匹配4个子
	for (int i = 0; i < 5; i++)
	{
		if (col - i >= 0 && col - i + 4 < chessboard_size && chessMap[row][col - i] == chessMap[row][col - i + 1] && chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] && chessMap[row][col - i] == chessMap[row][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate horizontal direction success||i={}||playFlag={}", i, (int)this->player_flag);
			return true;
		}
	}

	// 竖直方向 
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 && row - i + 4 < chessboard_size && chessMap[row - i][col] == chessMap[row - i + 1][col] && chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] && chessMap[row - i][col] == chessMap[row - i + 4][col])
		{
			INFOLOG("Chess::checkWin||calculate vertical direction success||i={}||playFlag={}", i, (int)this->player_flag);
			return true;
		}
	}

	// 左斜方向"/"
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 && col - i >= 0 && row - i + 4 < chessboard_size && col - i + 4 < chessboard_size && chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] && chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] && chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate left oblique direction success||i={}||playFlag={}", i, (int)this->player_flag);
			return true;
		}
	}

	// 右斜方向 "\"
	for (int i = 0; i < 5; i++)
	{
		if (row + i < chessboard_size && row + i - 4 >= 0 && col - i >= 0 && col - i + 4 < chessboard_size && chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] && chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] && chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate right oblique direction success||i={}||playFlag={}", i, (int)this->player_flag);
			return true;
		}
	}

	return false;
}

int Chessboard::get_chessboard_center_x()
{
	int center_x = 500;
	if (this->chessboard_boundary != nullptr)
	{
		center_x = (this->chessboard_boundary->left_top_x+this->chessboard_boundary->right_top_x) / 2;
	}
	INFOLOG("get_chessboard_center_x||center_x={}", center_x);
	return center_x;
}

int Chessboard::get_chessboard_center_y()
{
	int center_y = 500;
	if (this->chessboard_boundary != nullptr)
	{
		center_y = (this->chessboard_boundary->left_top_y+this->chessboard_boundary->left_bottom_y) / 2;
	}
	INFOLOG("get_chessboard_center_y||center_y={}", center_y);
	return center_y;
}

void Chessboard::set_chessboard_withdraw()
{
	// 循环删除最后两个数据
	for (int i = 0; i < 2; i++)
	{
		ChessData temp = this->chessboard_data.back();
		this->chessMap[temp.chessPos.chess_row][temp.chessPos.chess_col] = 0;
		this->chessboard_data.pop_back();
	}
	INFOLOG("set_chessboard_withdraw success||chessboard_data.size={}", chessboard_data.size());
}

bool Chessboard::is_can_withdraw()
{
	if (this->chessboard_data.size() < 2)
	{
		INFOLOG("is_can_withdraw fail||too less data");
		return false;
	}
	INFOLOG("is_can_withdraw success||chessboard_data.size={}", chessboard_data.size());
	return true;
}
