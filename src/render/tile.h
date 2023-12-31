#pragma once

#include <SDL2/SDL.h>

//瓷砖常数
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

//不同的瓷砖精灵
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

LTexture gTileTexture;

//瓷砖类
class Tile
{
    public:
        //Initializes position and type
        Tile( int x, int y, int tileType );

        //Shows the tile
        void render( SDL_Rect& camera );

        //Get the tile type
        int getType();

        //获取碰撞盒
        SDL_Rect getBox();

    private:
        //The attributes of the tile
        SDL_Rect mBox;

        //The tile type
        int mType;
};
