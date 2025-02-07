#pragma once
#include <vector>
#include <iostream>
#include "chessboard.h"
#include "my_utils.h"

class Machine
{
private:
    // 棋盘数据，对哪个棋盘下棋
	Chessboard* m_chessboard;

	// 储存各个点的评分情况，作为AI下棋数据
	std::vector<std::vector<int>> m_score_map;

	// 棋子数量
	int m_chess_num;

	/*
	* AI先计算棋手如果在这个位置落子，会有多大的价值。然后再计算自己如果在这个位置落子，有大大价值。
	* 具体计算方法
	*	就是计算如果黑棋或者白棋在这个位置落子，那么在这个位置的某个方向上， 一共有连续几个黑子或者连续几个白子。连续的数量越多，价值越大。
	* 计算下哪个位置能得多少分，默认黑棋为玩家方，白棋为AI方
	* 目标棋型		黑棋			白棋
	*	连2			10			10
	*	死3			30			25
	*	活3			40			50
	*	死4			60			55
	*	活4			200			10000
	*	连5(赢棋)	20000		30000
	* 死棋是指白棋方堵住黑棋方一头
	*/
	// 静态函数，经过计算输入棋盘数据的值，返回当前棋盘数据的打分值
	void calculateScore();

	// AI的思考，遍历周围找出评分最高的点
	ChessPos think();
public:
    Machine(Chessboard* chessboard);
    ~Machine();

    // 初始化，评分数组初始化
    void init();

	// 下棋成功，棋子数+1
	void addChessNum();

	// 获取机器人当前对局下棋数量
	int getChessNum();

	// 每次游戏完后，重开游戏需要设置的参数
	void resetMachine();

	// 友元函数，用于做线程函数，访问Machine类内部变量
	friend int machineChessDown(void* data);
};


