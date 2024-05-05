#include "sdl_button.h"

extern Mix_Chunk* g_select_button_sound;

SDLButton::SDLButton(const Config& config, const std::string button_name, const int& x, const int& y)
{
    std::string temp;
    m_button_name = button_name;
    m_current_sprite = BUTTON_SPRITE_MOUSE_OUT;
    m_sdl_texture = new SDLTexture(button_name);
    m_button_x = x;
    m_button_y= y;
    m_button_width = config.Read(button_name+"_width", 0);
    m_button_height = config.Read(button_name+"_height", 0);
    m_button_multiple = config.Read(button_name+"_multiple", 0.0);
    m_button_resource_path = config.Read(button_name+"_resourece_path", temp);
    m_is_load_resource = false;
    INFOLOG("SDLButton construct success||button_name={}||button_x={}||button_y={}||width={}||height={}||multiple={}||resource_path={}", 
    m_button_name, m_button_x, m_button_y, m_button_width, m_button_height, m_button_multiple, m_button_resource_path);
}

SDLButton::~SDLButton()
{
    m_sdl_texture->free();
    delete m_sdl_texture;
    INFOLOG("~SDLButton, release resource||button_name={}", m_button_name);
}

// 该函数将在事件循环中调用，处理从事件队列中提取的单个按钮事件
void SDLButton::handleButtonEvent(SDL_Event* event)
{
    //If mouse event happened
    // 首先，检查输入的事件是否是鼠标事件，具体包括鼠标移动事件（鼠标移动时）、鼠标按键下移事件（点击鼠标按键时）或鼠标按键上移事件（释放鼠标点击时）
    if(event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)
    {
        //Get mouse position
        // 如果发生了这些鼠标事件之一，使用 SDL_GetMouseState 检查鼠标位置。根据鼠标是否位于按钮上方，我们需要显示不同的精灵
        int x, y;
        SDL_GetMouseState(&x, &y);

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if(x < m_button_x - ((int)m_button_width*m_button_multiple) / 2)
        {
            inside = false;
        }
        //Mouse is right of the button
        else if(x > m_button_x + ((int)m_button_width*m_button_multiple) / 2)
        {
            inside = false;
        }
        //Mouse above the button
        else if(y < m_button_y - ((int)m_button_height*m_button_multiple) / 2)
        {
            inside = false;
        }
        //Mouse below the button
        else if(y > m_button_y + ((int)m_button_height*m_button_multiple) / 2)
        {
            inside = false;
        }

        //Mouse is outside button
        /*
        根据鼠标是否位于按钮内和鼠标事件来设置按钮精灵。
        如果鼠标不在按钮内，设置鼠标移出精灵。如果鼠标在按钮内，将精灵设置为鼠标移动时鼠标移到上方，
        鼠标按下时鼠标移到下方，鼠标释放时鼠标移到上方
        */
        if(!inside)
        {
            m_current_sprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch(event->type)
            {
                case SDL_MOUSEMOTION:
                m_current_sprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;
            
                case SDL_MOUSEBUTTONDOWN:
                Mix_PlayChannel(-1, g_select_button_sound, 0);  // TODO  优化音效延迟
                m_current_sprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;
                
                case SDL_MOUSEBUTTONUP:
                m_current_sprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
        // DEBUGLOG("handleButtonEvent||button_name={}||m_current_sprite={}", m_button_name, (int)this->m_current_sprite);
    }
}

bool SDLButton::loadResource(SDL_Window * global_window, SDL_Renderer* global_renderer)
{
   //Load data
    if(!m_sdl_texture->loadPixelsFromFile(global_window, m_button_resource_path))
    {        
        ERRORLOG("Failed to load texture||button_name={}", m_button_name);
        return false;
    }
    //Get pixel data
    Uint32* pixels = m_sdl_texture->getPixels32();
    int pixelCount = m_sdl_texture->getPitch32() * m_sdl_texture->getHeight();
    //Map colors
    Uint32 colorKey = SDL_MapRGBA(SDL_GetWindowSurface(global_window)->format, 255, 255, 255, 0xFF);   //取出按键周围的颜色，用下面的值将其设置为透明色
    Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(global_window)->format, 0xFF, 0xFF, 0xFF, 0x00);
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
    if(!m_sdl_texture->loadFromPixels(global_renderer))
    {
        ERRORLOG("Unable to load texture from surface||button_name={}", m_button_name);
        return false;
    }
    //设置精灵
    for(int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
    {
        // 根据坐标取出button.png图像中不同区域的图像
        m_sprite_clips[i].x = i * m_button_width;
        if (i == BUTTON_SPRITE_TOTAL - 1)  // 将按键释放后的图像与鼠标在按键外的图像设置为同一个
        {
            m_sprite_clips[i].x = 0;
        }
        m_sprite_clips[i].y = 0;
        m_sprite_clips[i].w = m_button_width;
        m_sprite_clips[i].h = m_button_height;
    }
    m_is_load_resource = true;
    return true;
}

// 根据当前的鼠标事件(m_current_sprite)渲染不同的图像(m_sprite_clips)
bool SDLButton::buttonRender(SDL_Renderer* global_renderer)
{
    if (!m_is_load_resource)
    {
        ERRORLOG("button not load resource, please load resource||button_name={}", m_button_name);
        return false;
    }
    int new_width = (int)(m_button_width * m_button_multiple);
    int new_height = (int)(m_button_height * m_button_multiple);
    int x_offset = new_width / 2;
    int y_offset = new_height / 2;
    int center_x = m_button_x - x_offset;
    int center_y = m_button_y - y_offset;
    //Show current button sprite
    m_sdl_texture->render(global_renderer, center_x, center_y, m_button_multiple, &m_sprite_clips[m_current_sprite]);   
    // DEBUGLOG("buttonRender||x={}||y={}||m_sprite_clips.x={}||m_sprite_clips.y={}||m_sprite_clips.w={}||m_sprite_clips.h={}", button_position.x,
    // button_position.y, m_sprite_clips[m_current_sprite].x, m_sprite_clips[m_current_sprite].y, m_sprite_clips[m_current_sprite].w, m_sprite_clips[m_current_sprite].h);
    return true;
}

sdl_button_sprite SDLButton::getButtonCurrentSprite()
{
    return m_current_sprite;
}

void SDLButton::initButtonCurrentSprite()
{
    m_current_sprite = BUTTON_SPRITE_MOUSE_OUT;
}


