#include "sdl_button_back.h"

SDLButton::SDLButton(const Config& config, const std::string button_name, const int& x, const int& y)
{
    std::string temp;
    this->button_name = button_name;
    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
    this->sdl_texture = new SDLTexture(button_name);
    this->button_x = x;
    this->button_y= y;
    this->button_width = config.Read(button_name+"_width", 0);
    this->button_height = config.Read(button_name+"_height", 0);
    this->button_multiple = config.Read(button_name+"_multiple", 0.0);
    this->button_resource_path = config.Read(button_name+"_resourece_path", temp);
    this->is_load_resource = false;
    INFOLOG("SDLButton construct success||button_name={}||button_x={}||button_y={}||width={}||height={}||multiple={}||resource_path={}", 
    this->button_name, this->button_x, this->button_y, this->button_width, this->button_height, this->button_multiple, this->button_resource_path);
}

SDLButton::~SDLButton()
{
    this->sdl_texture->free();
    delete sdl_texture;
    INFOLOG("~SDLButton, release resource||button_name={}", this->button_name);
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
        /*
        检查鼠标是否位于按钮内。由于用的是与 SDL 不同的坐标系，因此按钮的原点位于左上方。
        这意味着每一个小于 x 位置的 x 坐标都在按钮外，每一个小于 y 位置的 y 坐标也在按钮外。按钮右侧的所有内容都是 x 位置 + 宽度，按钮下方的所有内容都是 y 位置 + 高度。
        这就是这段代码的作用。如果鼠标位置超出了按钮的任何范围，它就会将内部标记标记为假。否则，它将保持初始值为真
        */
        bool inside = true;

        //Mouse is left of the button
        if(x < button_x)
        {
            inside = false;
        }
        //Mouse is right of the button
        else if(x > button_x + (int)this->button_width*this->button_multiple)
        {
            inside = false;
        }
        //Mouse above the button
        else if(y < button_y)
        {
            inside = false;
        }
        //Mouse below the button
        else if(y > button_y + (int)this->button_height*this->button_multiple)
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
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch(event->type)
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;
            
                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;
                
                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
        // DEBUGLOG("handleButtonEvent||button_name={}||mCurrentSprite={}", this->button_name, (int)this->mCurrentSprite);
    }
}

bool SDLButton::loadResource(SDL_Window * global_window, SDL_Renderer* global_renderer)
{
   //Load data
    if(!this->sdl_texture->loadPixelsFromFile(global_window, this->button_resource_path))
    {        
        ERRORLOG("Failed to load texture||button_name={}", this->button_name);
        return false;
    }
    //Get pixel data
    Uint32* pixels = this->sdl_texture->getPixels32();
    int pixelCount = this->sdl_texture->getPitch32() * this->sdl_texture->getHeight();
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
    if(!this->sdl_texture->loadFromPixels(global_renderer))
    {
        ERRORLOG("Unable to load texture from surface||button_name={}", this->button_name);
        return false;
    }
    //设置精灵
    for(int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
    {
        // 根据坐标取出button.png图像中不同区域的图像
        gSpriteClips[i].x = i * this->button_width;
        if (i == BUTTON_SPRITE_TOTAL - 1)  // 将按键释放后的图像与鼠标在按键外的图像设置为同一个
        {
            gSpriteClips[i].x = 0;
        }
        gSpriteClips[i].y = 0;
        gSpriteClips[i].w = this->button_width;
        gSpriteClips[i].h = this->button_height;
    }
    this->is_load_resource = true;
    return true;
}

// 根据当前的鼠标事件(mCurrentSprite)渲染不同的图像(gSpriteClips)
bool SDLButton::buttonRender(SDL_Renderer* global_renderer)
{
    if (!this->is_load_resource)
    {
        ERRORLOG("button not load resource, please load resource||button_name={}", this->button_name);
        return false;
    }
    int new_width = (int)(this->button_width * this->button_multiple);
    int new_height = (int)(this->button_height * this->button_multiple);
    int x_offset = new_width / 2;
    int y_offset = new_height / 2;
    int center_x = button_x - x_offset;
    int center_y = button_y - y_offset;
    //Show current button sprite
    this->sdl_texture->render(global_renderer, center_x, center_y, this->button_multiple, &gSpriteClips[mCurrentSprite]);   
    // DEBUGLOG("buttonRender||x={}||y={}||gSpriteClips.x={}||gSpriteClips.y={}||gSpriteClips.w={}||gSpriteClips.h={}", button_position.x,
    // button_position.y, gSpriteClips[mCurrentSprite].x, gSpriteClips[mCurrentSprite].y, gSpriteClips[mCurrentSprite].w, gSpriteClips[mCurrentSprite].h);
    return true;
}

sdl_button_sprite SDLButton::getButtonCurrentSprite()
{
    return this->mCurrentSprite;
}

void SDLButton::initButtonCurrentSprite()
{
    this->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}


