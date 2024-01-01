#pragma once

#include <iostream>
#include "../render/sdl_texture.h"
#include "../logger/logger.h"
#include "../render/sdl_texture.h"
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
public:
    Chess(const Config& config, const std::string& chess_name, const std::string& chess_resource_path);

    ~Chess();

    // 加载资源文件
    bool loadResource(SDL_Window * gWindow, SDL_Renderer* gRenderer);

    // 将棋子渲染在棋盘的x,y坐标，其中x，y是棋盘上以左上角为坐标原点的相对坐标
    bool chessRender(const int& x, const int& y);
};


