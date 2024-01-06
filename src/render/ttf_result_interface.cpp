
#include "ttf_result_interface.h"

TTFResultInterface::TTFResultInterface(const Config& config, const std::string& config_prefix)
{
    std::string temp;
    this->ttf_result_multiple = config.Read("ttf_result_multiple", 0.0);
    this->sdl_texture = new SDLTexture("TTFResultInterface");
    this->is_load_text =false;
    this->render_text = config.Read(config_prefix, temp);
    this->render_r = config.Read(config_prefix+"_r", 0);
    this->render_g = config.Read(config_prefix+"_g", 0);
    this->render_b = config.Read(config_prefix+"_b", 0);
    this->render_alpha = config.Read(config_prefix+"_alpha", 0);
    INFOLOG("TTFResultInterface construct success||ttf_result_multiple={}||render_text={}||render_r={}||render_g={}||render_b={}||render_alpah={}", 
    this->ttf_result_multiple, this->render_text, this->render_r, this->render_g, this->render_b, this->render_alpha);
}

TTFResultInterface::~TTFResultInterface()
{
    this->sdl_texture->free();
    delete sdl_texture;
    INFOLOG("~TTFResultInterface, release resource");
}

bool TTFResultInterface::loadRenderText(SDL_Renderer* gRenderer, TTF_Font* gFont)
{
    SDL_Color textColor = {(Uint8)this->render_r, (Uint8)this->render_g, (Uint8)this->render_b, (Uint8)this->render_alpha};
    //Load data stream
    if(!this->sdl_texture->loadFromRenderedText(gRenderer, gFont, this->render_text, textColor))
    {        
        ERRORLOG("Failed to load TTF texture!");
        return false;
    }
    // TTF_SetFontStyle(gFont, TTF_STYLE_BOLD);
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
