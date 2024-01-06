#include "chess.h"

Chess::Chess(const Config& config, const std::string& chess_name, const std::string& chess_resource_path, const int& origin_x, const int& origin_y, const int& lattice_size)
{
    this->chess_resource_path = chess_resource_path;
    this->chess_name = chess_name;
    this->chess_multiple = config.Read("chess_multiple", 0.0);
    this->chess_origin_size = config.Read("chess_origin_size", 0);
    this->origin_x = origin_x;
    this->origin_y = origin_y;
    this->lattice_size = lattice_size;
    this->sdl_texture = new SDLTexture(this->chess_name);
    this->is_load_resource = false;
    INFOLOG("Chess construct success||chess_resource_path={}||chess_name={}||chess_multiple={}||chess_origin_size={}", this->chess_resource_path, this->chess_name, this->chess_multiple, this->chess_origin_size);
}

Chess::~Chess()
{
    this->sdl_texture->free();
    delete(sdl_texture);
    INFOLOG("~Chess, release resource||chess_name={}", this->chess_name);
}

bool Chess::loadResource(SDL_Window * gWindow, SDL_Renderer* gRenderer)
{
    //Load data stream
    if(!this->sdl_texture->loadPixelsFromFile(gWindow, this->chess_resource_path))
    {        
        ERRORLOG("Failed to load texture!");
        return false;
    }
    // INFOLOG("load texture success!");
    //Get pixel data
    Uint32* pixels = this->sdl_texture->getPixels32();
    int pixelCount = this->sdl_texture->getPitch32() * this->sdl_texture->getHeight();
    //Map colors
    Uint32 colorKey = 28095;   //取出棋子周围的颜色，用下面的值将其设置为透明色
    Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(gWindow)->format, 0xFF, 0xFF, 0xFF, 0x00);
    // INFOLOG("loadMedia||pixelCount={}||colorKey={}||transparent={}", pixelCount, std::to_string(colorKey), std::to_string(transparent));
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
        return false;
    }
    this->is_load_resource = true;
    return true;
}

bool Chess::chessRender(SDL_Renderer* gRenderer, const int& x, const int& y)
{
    if (!this->is_load_resource)
    {
        ERRORLOG("chess not load resource, please load resource");
        return false;
    }
    
    int new_width = (int)this->chess_origin_size * this->chess_multiple;
    int new_height = (int)this->chess_origin_size * this->chess_multiple;
    int x_offset = new_width / 2;
    int y_offset = new_height / 2;
    int chess_x = x * this->lattice_size + this->origin_x - x_offset;
    int chess_y = y * this->lattice_size + this->origin_y - y_offset;
    this->sdl_texture->render(gRenderer, chess_x, chess_y, this->chess_multiple);
    // DEBUGLOG("chessRender||chess_name={}||x={}||y={}||chess_x={}||chess_y={}",this->chess_name, x, y, chess_x, chess_y);
    return false;
}
