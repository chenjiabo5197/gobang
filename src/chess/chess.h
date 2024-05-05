#pragma once

#include <iostream>
#include "sdl_texture.h"
#include "logger.h"
#include "config.h"
#include "sdl_window.h"

class Chess
{
private:
    // 棋子名字
    std::string m_chess_name;
    // 棋子路径
    std::string m_chess_resource_path;
    // 棋子相对于原始图片缩放的倍数
    float m_chess_multiple;

    SDLTexture* m_sdl_texture;

    // 棋子的原始尺寸
    int m_chess_origin_width;
    int m_chess_origin_height;
    // 棋盘每个格子大小
    int m_lattice_size;
    // 棋盘左上角坐标
    int m_origin_x, m_origin_y;

    // 棋子直接被渲染到该位置
    int m_chess_x;
    int m_chess_y;

    //  渲染棋子窗口
    SDLWindow* m_chess_window;

    // 标志该chess是否已经加载资源，以前出现了未加载资源去渲染结果没有数据，找了好久原因
    bool m_is_load_resource;
public:
    Chess(const Config& config, const std::string& chess_name, const int& origin_x, const int& origin_y, const int& lattice_size);

    Chess(const Config& config, const std::string& chess_name);

    ~Chess();

    // 初始化，传入渲染所需参数
	void init(SDLWindow* chess_window);

    // 将棋子渲染在棋盘的x,y坐标，其中x，y是棋盘上以左上角为坐标原点的相对坐标
    bool chessRender(const int& x, const int& y);

    // 渲染棋子, 坐标在使用setChessCoordinate函数传入,缩放倍数使用setChessMultiple传入
    bool chessRender();

    // 设置棋子的坐标，设置的是棋子中心点的坐标，渲染该棋子时无需传入坐标参数
    void setChessCoordinate(const int& x, const int& y);

    // 返回棋子的坐标
    std::pair<int, int> getChessCoordinate();

    // 设置棋子的缩放倍数
    void setChessMultiple(const float& multiple);

    // 获取棋子的实际大小
    int getChessSize();
};


