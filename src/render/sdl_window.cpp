#include "sdl_window.h"

//显示屏都有一个整数 ID 和一个与之相关的矩形，因此知道每个显示屏在桌面上的位置和尺寸
int gTotalDisplays = 0;
SDL_Rect* gDisplayBounds = nullptr; 

SDLWindow::SDLWindow(const Config& config, const std::string& name)
{
    std::string temp;
    mWindow = nullptr;
    mRenderer = nullptr;
    mMouseFocus = false;
    mKeyboardFocus = false;
    mFullScreen = false;
    mMinimized = false;
    this->name = name;
    this->mWidth = config.Read(name+"_screen_width", 0);
    this->mHeight = config.Read(name+"_screen_height", 0);
    this->title = config.Read(name+"_title", temp);
    this->background_path = config.Read(name+"_background_path", temp);
    this->background_texture = new SDLTexture(this->name);
    INFOLOG("SDLWindow construct success||name={}||title={}||background_path={}||width={}||height={}", this->name, this->title, this->background_path, this->mWidth, this->mHeight);
}

SDLWindow::~SDLWindow()
{
    delete background_texture;
    INFOLOG("~SDLWindow success||name={}||release resource", this->name);
}

void SDLWindow::free()
{
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    mWindow = nullptr;
    mRenderer = nullptr;
    INFOLOG("free success||name={}||destroy window and renderer", this->name);
}

bool SDLWindow::init()
{
    //Create window
    // SDL_WINDOW_RESIZABLE 标志的窗口，该标志允许调整窗口的大小
    mWindow = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->mWidth, this->mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(mWindow == nullptr)
    {
        ERRORLOG("init||Window could not be created||SDL_Error: ", SDL_GetError());
        return false;
    }
    DEBUGLOG("init||Create window success||name={}||width={}||height={}", this->name, this->mWidth, this->mHeight);
    mMouseFocus = true;
    mKeyboardFocus = true;

    //Create renderer for window
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(mRenderer == nullptr)
    {
        ERRORLOG("init||Renderer could not be created||SDL Error={}", SDL_GetError());
        return false;
    }
    DEBUGLOG("init||Create renderer success!");

    if (this->background_path != "")
    {
        if(!this->background_texture->loadPixelsFromFile(mWindow, this->background_path))
        {        
            ERRORLOG("Failed to load texture!"); 
            return false;
        }
        if(!this->background_texture->loadFromPixels(mRenderer))
        {
            ERRORLOG("Unable to load texture from surface!");
            return false;
        }
        DEBUGLOG("init||background_path={}||load resource success!", this->background_path);
    }
    
    //Grab window identifier
    // 在创建窗口后获取窗口 ID
    window_id = SDL_GetWindowID(mWindow);
    //SDL_GetWindowDisplayIndex，获取窗口是在哪个显示屏上创建
    window_display_id = SDL_GetWindowDisplayIndex(mWindow);
    //Flag as opened
    mShown = true;
    INFOLOG("init||name={}||window_id={}||window_display_id={}", this->name, this->window_id, window_display_id);
    return mWindow != nullptr && mRenderer != nullptr;;
}

void SDLWindow::handleEvent(SDL_Event& e)
{
    //Caption update flag
    bool updateCaption = false;
    //Window event occured
    // 查找 SDL_WINDOWEVENT 类型的事件。SDL_WindowEvents 实际上是一系列事件。根据事件的不同，需要更新窗口的标题，因此有一个标记来跟踪该事件
    // 来自所有窗口的所有事件都会进入同一个事件队列，因此要知道哪些事件属于哪个窗口，需要检查事件的窗口 ID 是否与窗口 ID 一致。
    if(e.type == SDL_WINDOWEVENT && e.window.windowID == window_id)
    {
        // 有了窗口事件后，需要检查 SDL_WindowEventID 以确定事件的类型。
        switch(e.window.event)
        {
            //Window moved
            //处理 SDL_WINDOWEVENT_MOVED 事件，以便使用 SDL_GetWindowDisplayIndex 更新窗口所在的显示屏
            case SDL_WINDOWEVENT_MOVED:
            window_display_id = SDL_GetWindowDisplayIndex(mWindow);
            updateCaption = true;
            break;

            /*
            当有多个窗口时，Xing out 窗口并不一定意味着要退出程序。要做的是让每个窗口在退出时隐藏起来。
            因此，需要通过检查 SDL_WINDOWEVENT_SHOWN/SDL_WINDOWEVENT_HIDDEN 事件来跟踪窗口何时隐藏/显示。
            */
            //Window appeared
            case SDL_WINDOWEVENT_SHOWN:
            mShown = true;
            break;

            //Window disappeared
            case SDL_WINDOWEVENT_HIDDEN:
            mShown = false;
            break;

            //Get new dimensions and repaint on window size change
            // SDL_WINDOWEVENT_SIZE_CHANGED 是一个调整大小的事件，因此要获取新的尺寸并刷新屏幕上的图像
            case SDL_WINDOWEVENT_SIZE_CHANGED:
            mWidth = e.window.data1;
            mHeight = e.window.data2;
            SDL_RenderPresent(mRenderer);
            break;

            //Repaint on exposure
            // SDL_WINDOWEVENT_EXPOSED 仅表示该窗口以某种方式被遮挡，而现在没有被遮挡，因此要重新绘制该窗口。
            case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(mRenderer);
            break;

            //Mouse entered window
            // SDL_WINDOWEVENT_ENTER用于处理鼠标移入窗口的情况
            case SDL_WINDOWEVENT_ENTER:
            mMouseFocus = true;
            updateCaption = true;
            break;
            
            //Mouse left window
            //SDL_WINDOWEVENT_LEAVE 于处理鼠标移出窗口的情况
            case SDL_WINDOWEVENT_LEAVE:
            mMouseFocus = false;
            updateCaption = true;
            break;

            //Window has keyboard focus
            // SDL_WINDOWEVENT_FOCUS_GAINED用于处理窗口从键盘获取输入的情况。会跟踪键盘焦点，因此当这些事件发生时，设置更新标题标志
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            mKeyboardFocus = true;
            updateCaption = true;
            break;

            //Window lost keyboard focus
            //SDL_WINDOWEVENT_FOCUS_LOST 用于处理窗口从键盘获取输入的情况。会跟踪键盘焦点，因此当这些事件发生时，设置更新标题标志
            case SDL_WINDOWEVENT_FOCUS_LOST:
            mKeyboardFocus = false;
            updateCaption = true;
            break;

             //Window minimized
             //窗口最小化
            case SDL_WINDOWEVENT_MINIMIZED:
            mMinimized = true;
            break;

            //Window maximized
            //最大化
            case SDL_WINDOWEVENT_MAXIMIZED:
            mMinimized = false;
            break;
            
            //Window restored
            //最小化状态恢复
            case SDL_WINDOWEVENT_RESTORED:
            mMinimized = false;
            break;

            //Hide on close
            // 当有多个窗口时，"Xing out "窗口会被解释为 SDL_WINDOWEVENT_CLOSE 窗口事件。当收到这些事件时，使用 SDL_HideWindow 隐藏窗口
            case SDL_WINDOWEVENT_CLOSE:
            SDL_HideWindow(mWindow);
            break;
        }
    }
    // else if(e.type == SDL_KEYDOWN)
    // {
    //     //Display change flag
    //     bool switchDisplay = false;

    //     //当按向上或向下键时，就会改变显示索引，移动到下一个显示器显示
    //     switch(e.key.keysym.sym)
    //     {
    //         case SDLK_UP:
    //         ++window_display_id;
    //         switchDisplay = true;
    //         break;

    //         case SDLK_DOWN:
    //         --window_display_id;
    //         switchDisplay = true;
    //         break;
    //     }
        //如果要移动到下一个显示窗口，首先要确定该显示窗口是一个有效的索引。然后使用 SDL_SetWindowPosition 更新窗口的位置。此处的调用将使窗口在下一个显示屏中居中
        // if(switchDisplay)
        // {
        //     //一直按上或者下也能让窗口在不同显示器之间循环起来
        //     if(window_display_id < 0)
        //     {
        //         window_display_id = gTotalDisplays - 1;
        //     }
        //     else if(window_display_id >= gTotalDisplays)
        //     {
        //         window_display_id = 0;
        //     }
        //     // mWindow为创建的窗口指针，第二个参数为将要得到的窗口X坐标，第三个参数为将要得到的窗口Y坐标
        //     SDL_SetWindowPosition(mWindow, gDisplayBounds[ window_display_id ].x + (gDisplayBounds[ window_display_id ].w - mWidth) / 2, gDisplayBounds[ window_display_id ].y + (gDisplayBounds[ window_display_id ].h - mHeight) / 2);
        //     updateCaption = true;
        // }
    // }
    //Update window caption with new data
    // if(updateCaption)
    // {
    //     std::stringstream caption;
    //     caption << "SDL Tutorial - ID: " << window_id << " Display: " << window_display_id << " MouseFocus:" << ((mMouseFocus) ? "On" : "Off") << " KeyboardFocus:" << ((mKeyboardFocus) ? "On" : "Off");
    //     SDL_SetWindowTitle(mWindow, caption.str().c_str());
    // }
}
  
//为窗口获取焦点的函数。首先，检查窗口是否正在显示，如果没有显示，则使用 SDL_ShowWindow 将其显示出来。接下来，调用 SDL_RaiseWindow 来聚焦窗口。
void SDLWindow::focus()
{
    //Restore window if needed
    if(!mShown)
    {
        SDL_ShowWindow(mWindow);
    }

    //将窗口向前移动
    SDL_RaiseWindow(mWindow);
}

// 在窗口未最小化的情况下进行渲染
void SDLWindow::render()
{
    if(!mMinimized)
    {    
        //Clear screen
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(mRenderer);
        //Update screen
        SDL_RenderPresent(mRenderer);
    }
}

SDL_Window* SDLWindow::getWindow()
{
    return this->mWindow;
}
    
SDL_Renderer* SDLWindow::getRenderer()
{
    return this->mRenderer;
}

int SDLWindow::getWidth()
{
    return mWidth;
}

int SDLWindow::getHeight()
{
    return mHeight;
}

bool SDLWindow::hasMouseFocus()
{
    return mMouseFocus;
}

bool SDLWindow::hasKeyboardFocus()
{
    return mKeyboardFocus;
}

bool SDLWindow::isMinimized()
{
    return mMinimized;
}

bool SDLWindow::isShown()
{
    return mShown;
}

int SDLWindow::getWindowId()
{
    return window_id;
}

void SDLWindow::render_background()
{
    if (this->background_path != "")
    {
        // 使背景图片铺满整个窗口
        SDL_Rect render_resource= {0, 0, this->background_texture->getWidth(), this->background_texture->getHeight()};
        SDL_Rect render_target = {0, 0, this->mWidth, this->mHeight};
        DEBUGLOG("render_background||w={}||h={}", render_target.w, render_target.h);
        this->background_texture->render(this->mRenderer, 0, 0, 1.0, &render_resource, &render_target);
    }
}
