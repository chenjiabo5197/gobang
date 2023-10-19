#pragma once

typedef enum {
	CHESS_WHITE = -1,   // 白方
	CHESS_BLACK = 1		// 黑方
} chess_kind_type;

struct ChessPos {   // 棋子位置
	int row;
	int col;
};

class Chess
{
public:
	// 棋盘的初始化，加载棋盘的图片资源，初始化棋盘的相关数据
	void init();

	// 判断在指定坐标(x,y)位置，是否有效点击
	// 如果是有效点击，把有效点击的位置(x,y)保存到参数pos中
	bool clickBoard(int x, int y, ChessPos* pos);

	// 在棋盘的指定位置pos，落子kind
	void chessDown(ChessPos* pos, chess_kind_type kind);

	// 获取棋盘大小（13,15,19）
	int getChessBoardSize();

	// 获取指定位置是黑棋/白棋/空白
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	// 判断棋局是否结束
	bool checkOver();
};

