#include "sdl_timer.h"

SDLTimer::SDLTimer(const std::string& name)
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;
    mPaused = false;
    mStarted = false;
    this->name = name;
    INFOLOG("SDLTimer construct success||name={}", this->name);
}

SDLTimer::~SDLTimer()
{
    INFOLOG("~SDLTimer success||release resources");
}

void SDLTimer::timerStart()
{
    //Start the timer
    mStarted = true;
    //Unpause the timer
    mPaused = false;
    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    // 由于可以在计时器暂停和/或运行时启动它，因此应确保清除暂停的数据
    mPausedTicks = 0;
    INFOLOG("timerStart||name={}||mStartTicks={}", this->name, mStartTicks);
}

void SDLTimer::timerStop()
{
    //Stop the timer
    mStarted = false;
    //Unpause the timer
    mPaused = false;
    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void SDLTimer::timerPause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //如果定时器正在运行，设置暂停标志，在 mPausedTicks 中存储定时器暂停的时间，并重置开始时间
        //Pause the timer
        mPaused = true;
        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
        INFOLOG("timerPause||name={}||mStartTicks={}", this->name, mStartTicks);
    }
}

void SDLTimer::timerUnpause()
{
    //当取消暂停定时器时，要确保定时器正在运行和暂停
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;
        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;
        //Reset the paused ticks
        mPausedTicks = 0;
        INFOLOG("timerUnpause||name={}||mStartTicks={}", this->name, mStartTicks);
    }
}
 
// 获取当前定时器已运行的时间
Uint32 SDLTimer::timerGetTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }
    return time;
}

bool SDLTimer::timerIsStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool SDLTimer::timerIsPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}
