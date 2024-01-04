#pragma once

typedef enum {
    CHESS_DEFAULT,  // 空白
	CHESS_WHITE,    // 白方
	CHESS_BLACK		// 黑方
} chess_kind_type;

// 鼠标点击时在棋盘格子的周围四个角
typedef enum {
	COORD_DEFAULT,		   // 初始位置
	LEFTTOP,           //左上     
	RIGHTTOP,          //右上
	LEFTBOTTOM,        //左下
	RIGHTBOTTOM       //右下
} chess_coordinate;

typedef enum {
	GAME_KIND_DEFAULT,
	ONE_PLAYER_GAME,    // 单人游戏
	TWO_PLAYERS_GAME,    // 双人游戏
	PLAYER_INTERNET    // 网络对战
} game_kind;

// 棋子位置
struct ChessPos {   
	int chess_row;
	int chess_col;
};

// 储存每一步棋子位置和棋子类型的结构体
struct ChessData {
	ChessPos* chessPos;
	chess_kind_type chess_type;
};

// 棋盘边界，储存棋盘的四个角坐标
struct ChessBoardBoundary {
	int left_top_x;
	int left_top_y;
	int right_top_x;
	int right_top_y;
	int left_bottom_x;
	int left_bottom_y;
	int right_bottom_x;
	int right_bottom_y;
};

// 枚举渲染的种类
typedef enum {
	DEFAULT_INTERFACE,
	MAIN_MENU_INTERFACE,          // 选择开始游戏和退出游戏
	SELECT_PLAY_INTERFACE,        // 选择单人游戏，双人游戏，网络对战
	PLAYCHESS_INTERFACE,          // 对战中菜单，棋盘和退出游戏，悔棋
	PLAYER_WIN_INTERFACE,         // 单人游戏赢了，选择再来一局或者退出游戏
	PLAYER_LOSE_INTERFACE,        // 单人游戏输了，选择再来一局或者退出游戏
	PLAYER_DRAW_INTERFACE,        // 平局，棋盘落满棋子还未决出胜负
	BLACK_WIN_INTERFACE,          // 黑棋赢
	WHITE_WIN_INTERFACE,          // 白棋赢
	BEST_SCORE_INTERFACE          // 输出排行榜分数
} interface_kind_type;

// 下棋方的枚举值
typedef enum {
    DEFAULT_PLAYER,
    MACHINE_PLAYER,         // 单人游戏，电脑
    SINGLE_PLAYER,          // 单人游戏，玩家
    BLACK_PLAYER,           // 双人游戏，黑棋
    WHITE_PLAYER            // 双人游戏，白棋
} player_flag_type;





