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

    // 标志该chess是否已经加载资源，以前出现了未加载资源去渲染结果没有数据，找了好久原因
    bool is_load_resource;
public:
    Chess(const Config& config, const std::string& chess_name, const std::string& chess_resource_path, const int& origin_x, const int& origin_y, const int& lattice_size);

    ~Chess();

    // 加载资源文件
    bool loadResource(SDL_Window * gWindow, SDL_Renderer* gRenderer);

    // 将棋子渲染在棋盘的x,y坐标，其中x，y是棋盘上以左上角为坐标原点的相对坐标
    bool chessRender(SDL_Renderer* gRenderer, const int& x, const int& y);
};


