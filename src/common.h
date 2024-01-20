#pragma once
#include "SDL2/SDL.h"

// 单人游戏玩家赢
#define PLAYER_WIN_EVENT				(SDL_USEREVENT+1)
// 单人游戏玩家输
#define PLAYER_LOSE_EVENT				(SDL_USEREVENT+2)
// 开始游戏，进入选择游戏方式界面
#define START_GAME_EVENT				(SDL_USEREVENT+3)
// 查看最好分数
#define BEST_SCORES_EVENT				(SDL_USEREVENT+4)
// 退出游戏
#define EXIT_GAME_EVENT	    			(SDL_USEREVENT+5)
// 单人游戏
#define SINGLE_PLAYER_EVENT	    		(SDL_USEREVENT+6)
// 双人游戏
#define TWO_PLAYER_EVENT	    		(SDL_USEREVENT+7)
// 网络对战
#define PLAY_INTERNET_EVENT	    		(SDL_USEREVENT+8)
// 返回主菜单
#define BACK_MANU_EVENT	    			(SDL_USEREVENT+9)
// 单人游戏悔棋
#define PLAYER_WITHDRAW_EVENT			(SDL_USEREVENT+10)
// 游戏结算页面再来一局
#define AGAIN_GAME_EVENT	    		(SDL_USEREVENT+11)
// 单人游戏玩家选择白色棋子  后手
#define SINGLE_PLAYER_WHITE_EVENT	    (SDL_USEREVENT+12)
// 单人游戏玩家选择白色棋子  先手
#define SINGLE_PLAYER_BLACK_EVENT	    (SDL_USEREVENT+13)


typedef enum {
    CHESS_DEFAULT,  // 默认
	CHESS_WHITE,    // 白方
	CHESS_BLACK		// 黑方
} chess_kind_type;

// 鼠标点击时在棋盘格子的周围四个角
typedef enum {
	COORD_DEFAULT,	   // 初始位置
	LEFTTOP,           //左上     
	RIGHTTOP,          //右上
	LEFTBOTTOM,        //左下
	RIGHTBOTTOM        //右下
} chess_coordinate;

typedef enum {
	GAME_KIND_DEFAULT,
	ONE_PLAYER_GAME,     // 单人游戏
	TWO_PLAYERS_GAME,    // 双人游戏
	PLAYER_INTERNET      // 网络对战
} game_kind;

// 棋子位置
struct ChessPos {   
	int chess_row;
	int chess_col;
};

// 储存每一步棋子位置和棋子类型的结构体
struct ChessData {
	ChessPos chessPos;
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
	PLAYER_WIN_PRE_INTERFACE,     // 单人游戏赢了前一帧，与输了保持一致
	PLAYER_LOSE_INTERFACE,        // 单人游戏输了，选择再来一局或者退出游戏
	PLAYER_LOSE_PRE_INTERFACE,    // 单人游戏输了前一帧，为了确保能将电脑最后一步下的棋渲染出来
	PLAYER_DRAW_INTERFACE,        // 平局，棋盘落满棋子还未决出胜负
	BLACK_WIN_INTERFACE,          // 黑棋赢
	WHITE_WIN_INTERFACE,          // 白棋赢
	BEST_SCORE_INTERFACE          // 输出排行榜分数
} interface_kind_type;

// 当前下棋方的枚举值
typedef enum {
    DEFAULT_PLAYER,
    MACHINE_PLAYER,         // 单人游戏，电脑
    SINGLE_PLAYER,          // 单人游戏，玩家
    BLACK_PLAYER,           // 双人游戏，黑棋
    WHITE_PLAYER            // 双人游戏，白棋
} player_flag_type;

typedef enum {
    BUTTON_SPRITE_MOUSE_OUT,            // 鼠标不在按键上，一般用于默认值
    BUTTON_SPRITE_MOUSE_OVER_MOTION,	// 鼠标在按键上
    BUTTON_SPRITE_MOUSE_DOWN,			// 鼠标在按键上，且按下
    BUTTON_SPRITE_MOUSE_UP,				// 鼠标在按键上，释放按键
    BUTTON_SPRITE_TOTAL
} sdl_button_sprite;

// 当前对战输赢信息枚举
typedef enum {
	SINGLE_PLAYER_WIN,        	// 单人游戏玩家胜
	SINGLE_PLAYER_LOSE,			// 单人游戏玩家输
	TWO_PLAYER_BLACK_WIN,		// 双人游戏黑方胜
	TWO_PLAYER_WHITE_WIN		// 双人游戏拜访胜
}result_info_type;



