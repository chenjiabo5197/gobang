#include "chess.h"

Chess::Chess(const Config& config, const std::string& chess_name, const std::string& chess_resource_path)
{
    this->chess_resource_path = chess_resource_path;
    this->chess_name = chess_name;
    this->chess_multiple = config.Read("chess_multiple", 0.0);
    this->sdl_texture = new SDLTexture(this->chess_name);
    INFOLOG("Chess construct success||chess_resource_path={}||chess_name={}||chess_multiple={}", chess_resource_path, chess_name, chess_multiple);
}

Chess::~Chess()
{
    delete(sdl_texture);
    INFOLOG("~Chess");
}

bool Chess::loadResource(SDL_Window * gWindow, SDL_Renderer* gRenderer)
{
    //Loading success flag
    bool success = true;
    //Load data stream
    if(this->sdl_texture->loadPixelsFromFile(gWindow, this->chess_resource_path))
    {        
        ERRORLOG("Failed to load texture!");
        success = false;
    }
    else
    {
        INFOLOG("load texture success!");
        //Get pixel data
        Uint32* pixels = this->sdl_texture->getPixels32();
        int pixelCount = this->sdl_texture->getPitch32() * this->sdl_texture->getHeight();
        //Map colors
        Uint32 colorKey = 28095;   //取出棋子周围的颜色，用下面的值将其设置为透明色
        Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(gWindow)->format, 0xFF, 0xFF, 0xFF, 0x00);
        INFOLOG("loadMedia||pixelCount={}||colorKey={}||transparent={}", pixelCount, std::to_string(colorKey), std::to_string(transparent));
        //Color key pixels
        for(int i = 0; i < pixelCount; ++i)
        {
            // DEBUGLOG("loadMedia||pixelCount={}", pixels[i]);
            if(pixels[i] == colorKey)
            {
                pixels[i] = transparent;
            }
        }
        //Create texture from manually color keyed pixels
        if(!this->sdl_texture->loadFromPixels(gRenderer))
        {
            ERRORLOG("Unable to load texture from surface!");
        }
    }
    return success;
}

bool Chess::chessRender(const int& x, const int& y)
{
    return false;
}
