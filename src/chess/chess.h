#pragma once

#include <iostream>
#include "../render/sdl_texture.h"
#include "../logger/logger.h"
#include "../utils/config.h"

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
    int chess_origin_size;
    // 棋盘每个格子大小
    int lattice_size;
    // 棋盘左上角坐标
    int origin_x, origin_y;

    // 棋子直接被渲染到该位置
    int chess_x;
    int chess_y;

    // 渲染的窗口
    SDL_Window* global_window;
    SDL_Renderer* global_renderer;

    // 标志该chess是否已经加载资源，以前出现了未加载资源去渲染结果没有数据，找了好久原因
    bool is_load_resource;
public:
    Chess(const Config& config, const std::string& chess_name, const int& origin_x, const int& origin_y, const int& lattice_size);

    Chess(const Config& config, const std::string& chess_name);

    ~Chess();

    // 初始化，传入渲染所需参数
	void init(SDL_Window * global_window, SDL_Renderer* global_renderer);

    // 将棋子渲染在棋盘的x,y坐标，其中x，y是棋盘上以左上角为坐标原点的相对坐标
    bool chessRender(const int& x, const int& y);

    // 渲染棋子,传入坐标和缩放倍数,坐标为中心处坐标，需要在函数内转化为左上角坐标
    bool chessRender(const int& x, const int& y, const float& multiple);
};


