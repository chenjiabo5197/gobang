
#include "ttf_result_interface.h"

TTFResultInterface::TTFResultInterface(const Config& config)
{
    this->ttf_result_multiple = config.Read("ttf_result_multiple", 0.0);
    this->sdl_texture = new SDLTexture("TTFResultInterface");
    this->is_load_text =false;
    INFOLOG("TTFResultInterface construct success||ttf_result_multiple={}", this->ttf_result_multiple);
}

TTFResultInterface::~TTFResultInterface()
{
    this->sdl_texture->free();
    delete sdl_texture;
    INFOLOG("~TTFResultInterface, release resource");
}

bool TTFResultInterface::loadRenderText(SDL_Renderer* gRenderer, TTF_Font* gFont, const std::string& textureText, SDL_Color textColor)
{
    //Load data stream
    if(!this->sdl_texture->loadFromRenderedText(gRenderer, gFont, textureText, textColor))
    {        
        ERRORLOG("Failed to load TTF texture!");
        return false;
    }
    this->is_load_text = true;
    return true;
}

bool TTFResultInterface::ttfRender(SDL_Renderer* gRenderer, const int& x, const int& y)
{
    if (!this->is_load_text)
    {
        ERRORLOG("ttf not load text, please load text");
        return false;
    }
    
    int new_width = (int)(this->sdl_texture->getWidth() * this->ttf_result_multiple);
    int new_height = (int)(this->sdl_texture->getHeight() * this->ttf_result_multiple);
    int x_offset = new_width / 2;
    int y_offset = new_height / 2;
    int ttf_result_x = x - x_offset;
    int ttf_result_y = y - y_offset;
    this->sdl_texture->render(gRenderer, ttf_result_x, ttf_result_y, this->ttf_result_multiple);
    // DEBUGLOG("chessRender||chess_name={}||x={}||y={}||chess_x={}||chess_y={}",this->chess_name, x, y, chess_x, chess_y);
    return false;
}
