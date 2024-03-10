#pragma once

#include <iostream>
#include "../render/sdl_texture.h"
#include "../logger/logger.h"
#include "../utils/config.h"
#include "../render/sdl_window.h"

class Chess
{
private:
    // 棋子名字
    std::string chess_name;
    // 棋子路径
    std::string chess_resource_path;
    // 棋子相对于原始图片缩放的倍数
    float chess_multiple;

    SDLTexture* sdl_texture;

    // 棋子的原始尺寸
    int chess_origin_width;
    int chess_origin_height;
    // 棋盘每个格子大小
    int lattice_size;
    // 棋盘左上角坐标
    int origin_x, origin_y;

    // 棋子直接被渲染到该位置
    int chess_x;
    int chess_y;

    // 主窗口
    SDLWindow* chess_window;

    // 标志该chess是否已经加载资源，以前出现了未加载资源去渲染结果没有数据，找了好久原因
    bool is_load_resource;
public:
    Chess(const Config& config, const std::string& chess_name, const int& origin_x, const int& origin_y, const int& lattice_size);

    Chess(const Config& config, const std::string& chess_name);

    ~Chess();

    // 初始化，传入渲染所需参数
	void init(SDLWindow* chess_window);

    // 将棋子渲染在棋盘的x,y坐标，其中x，y是棋盘上以左上角为坐标原点的相对坐标
    bool chessRender(const int& x, const int& y);

    // 渲染棋子, 坐标在使用set_chess_coordinate函数传入,缩放倍数使用set_chess_multiple传入
    bool chessRender();

    // 设置棋子的坐标，设置的是棋子中心点的坐标，渲染该棋子时无需传入坐标参数
    void set_chess_coordinate(const int& x, const int& y);

    // 返回棋子的坐标
    std::pair<int, int> get_chess_coordinate();

    // 设置棋子的缩放倍数
    void set_chess_multiple(const float& multiple);

    // 获取棋子的实际大小
    int get_chess_size();
};


