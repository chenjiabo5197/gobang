#include "chess.h"

Chess::Chess(const Config& config, const std::string& chess_name, const int& origin_x, const int& origin_y, const int& lattice_size)
{
    std::string temp;
    this->chess_name = chess_name;
    this->chess_resource_path = config.Read(chess_name+"_resource_path", temp);
    this->chess_multiple = config.Read(chess_name+"_multiple", 0.0);
    this->chess_origin_size = config.Read(chess_name+"_origin_size", 0);
    this->origin_x = origin_x;
    this->origin_y = origin_y;
    this->lattice_size = lattice_size;
    this->sdl_texture = new SDLTexture(this->chess_name);
    this->is_load_resource = false;
    INFOLOG("Chess1 construct success||chess_resource_path={}||chess_name={}||chess_multiple={}||chess_origin_size={}", 
    this->chess_resource_path, this->chess_name, this->chess_multiple, this->chess_origin_size);
}

Chess::Chess(const Config& config, const std::string& chess_name)
{
    std::string temp;
    this->chess_name = chess_name;
    this->chess_resource_path = config.Read(chess_name+"_resource_path", temp);
    this->chess_multiple = config.Read(chess_name+"_multiple", 0.0);
    this->chess_origin_size = config.Read(chess_name+"_origin_size", 0);
    this->chess_x = config.Read(chess_name+"_x", 0);
    this->chess_y = config.Read(chess_name+"_y", 0);
    this->sdl_texture = new SDLTexture(this->chess_name);
    this->is_load_resource = false;
    INFOLOG("Chess2 construct success||chess_resource_path={}||chess_name={}||chess_multiple={}||chess_x={}||chess_y={}", 
    this->chess_resource_path, this->chess_name, this->chess_multiple, this->chess_x, this->chess_y);

}

Chess::~Chess()
{
    this->sdl_texture->free();
    delete(sdl_texture);
    INFOLOG("~Chess, release resource||chess_name={}", this->chess_name);
}

void Chess::init(SDLWindow* chess_window)
{
    this->chess_window = chess_window;

    //Load data
    if(!this->sdl_texture->loadPixelsFromFile(this->chess_window->getWindow(), this->chess_resource_path))
    {        
        ERRORLOG("Failed to load texture!");
        return;
    }
    // INFOLOG("load texture success!");
    //Get pixel data
    Uint32* pixels = this->sdl_texture->getPixels32();
    int pixelCount = this->sdl_texture->getPitch32() * this->sdl_texture->getHeight();
    //Map colors
    Uint32 colorKey = 28095;   //取出棋子周围的颜色，用下面的值将其设置为透明色
    Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(this->chess_window->getWindow())->format, 0xFF, 0xFF, 0xFF, 0x00);
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
    if(!this->sdl_texture->loadFromPixels(this->chess_window->getRenderer()))
    {
        ERRORLOG("Unable to load texture from surface!");
        return;
    }
    this->is_load_resource = true;
    // DEBUGLOG("loadResource success||name={}", this->chess_name);
    INFOLOG("init||Chessboard init success||load resource success");
}

bool Chess::chessRender(const int& x, const int& y)
{
    if (!this->is_load_resource)
    {
        ERRORLOG("chess not load resource, please load resource||name={}", this->chess_name);
        return false;
    }
    
    int new_width = (int)this->chess_origin_size * this->chess_multiple;
    int new_height = (int)this->chess_origin_size * this->chess_multiple;
    int x_offset = new_width / 2;
    int y_offset = new_height / 2;
    int new_chess_x = x * this->lattice_size + this->origin_x - x_offset;
    int new_chess_y = y * this->lattice_size + this->origin_y - y_offset;
    this->sdl_texture->render(this->chess_window->getRenderer(), new_chess_x, new_chess_y, this->chess_multiple);
    // DEBUGLOG("chessRender||chess_name={}||x={}||y={}||chess_x={}||chess_y={}",this->chess_name, x, y, chess_x, chess_y);
    return true;
}

bool Chess::chessRender(const int& x, const int& y, const float& multiple)
{
    if (!this->is_load_resource)
    {
        ERRORLOG("chess not load resource, please load resource||name={}", this->chess_name);
        return false;
    }
    int new_width = (int)this->chess_origin_size * multiple;
    int new_height = (int)this->chess_origin_size * multiple;
    int x_offset = new_width / 2;
    int y_offset = new_height / 2;
    int new_chess_x = x - x_offset;
    int new_chess_y = y - y_offset;
    this->sdl_texture->render(this->chess_window->getRenderer(), new_chess_x, new_chess_y, multiple);
    // DEBUGLOG("chessRender||chess_name={}||x={}||y={}||chess_x={}||chess_y={}",this->chess_name, x, y, new_chess_x, new_chess_y);
    return true;
}
