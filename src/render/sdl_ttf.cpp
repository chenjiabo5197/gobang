
#include "sdl_ttf.h"

SDLTTF::SDLTTF(const Config& config, const std::string& name)
{
    std::string temp;
    m_sdl_texture = new SDLTexture("SDLTTF");
    m_is_load_text =false;
    m_ttf_name = name;
    m_render_text = config.Read(name, temp);
    m_render_r = config.Read(name+"_r", 0);
    m_render_g = config.Read(name+"_g", 0);
    m_render_b = config.Read(name+"_b", 0);
    m_render_alpha = config.Read(name+"_alpha", 0);
    INFOLOG("SDLTTF construct success||render_text={}||render_r={}||render_g={}||render_b={}||render_alpah={}||ttf_name={}", 
    m_render_text, m_render_r, m_render_g, m_render_b, m_render_alpha, m_ttf_name);
}

SDLTTF::SDLTTF(const std::string& name)
{
    std::string temp;
    m_sdl_texture = new SDLTexture("SDLTTF");
    m_is_load_text =false;
    m_ttf_name = name;
    INFOLOG("SDLTTF construct success||ttf_name={}", m_ttf_name);
}

SDLTTF::~SDLTTF()
{
    m_sdl_texture->free();
    delete m_sdl_texture;
    INFOLOG("~SDLTTF, release resource||ttf_name={}", m_ttf_name);
}

bool SDLTTF::loadRenderText(SDL_Renderer* global_renderer, TTF_Font* global_font)
{
    SDL_Color textColor = {(Uint8)m_render_r, (Uint8)m_render_g, (Uint8)m_render_b, (Uint8)m_render_alpha};
    //Load data stream
    if(!m_sdl_texture->loadFromRenderedText(global_renderer, global_font, m_render_text, textColor))
    {        
        ERRORLOG("Failed to load TTF texture!");
        return false;
    }
    // TTF_SetFontStyle(global_font, TTF_STYLE_BOLD);
    m_is_load_text = true;
    return true;
}

bool SDLTTF::loadRenderText(SDL_Renderer* global_renderer, TTF_Font* global_font, const std::string& texture_text, SDL_Color sdl_color)
{
    //Load data stream
    if(!m_sdl_texture->loadFromRenderedText(global_renderer, global_font, texture_text, sdl_color))
    {        
        ERRORLOG("Failed to load TTF texture!");
        return false;
    }
    // TTF_SetFontStyle(global_font, TTF_STYLE_BOLD);
    m_is_load_text = true;
    return true;
}

bool SDLTTF::ttfRender(SDL_Renderer* global_renderer, const int& x, const int& y)
{
    if (!m_is_load_text)
    {
        ERRORLOG("ttf not load text, please load text");
        return false;
    }
    int x_offset = m_sdl_texture->getWidth() / 2;
    int y_offset = m_sdl_texture->getHeight() / 2;
    int ttf_result_x = x - x_offset;
    int ttf_result_y = y - y_offset;
    m_sdl_texture->render(global_renderer, ttf_result_x, ttf_result_y, 1.0);
    // DEBUGLOG("ttfRender||ttf_result_x={}||ttf_result_y={}", ttf_result_x, ttf_result_y);
    return false;
}

bool SDLTTF::ttfRender(SDL_Renderer* global_renderer, const int& x, const int& y, const float& multiple)
{
    if (!m_is_load_text)
    {
        ERRORLOG("ttf not load text, please load text");
        return false;
    }
    int new_width = (int)(m_sdl_texture->getWidth() * multiple);
    int new_height = (int)(m_sdl_texture->getHeight() * multiple);
    int x_offset = new_width / 2;
    int y_offset = new_height / 2;
    int ttf_result_x = x - x_offset;
    int ttf_result_y = y - y_offset;
    m_sdl_texture->render(global_renderer, ttf_result_x, ttf_result_y, multiple);
    // DEBUGLOG("ttfRender||ttf_result_x={}||ttf_result_y={}", ttf_result_x, ttf_result_y);
    return false;
}
