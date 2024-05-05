#include "sdl_timer.h"

SDLTimer::SDLTimer(const std::string& name)
{
    //Initialize the variables
    m_start_ticks = 0;
    m_paused_ticks = 0;
    m_paused = false;
    m_started = false;
    m_name = name;
    INFOLOG("SDLTimer construct success||name={}", m_name);
}

SDLTimer::~SDLTimer()
{
    INFOLOG("~SDLTimer success||release resources");
}

void SDLTimer::timerStart()
{
    //Start the timer
    m_started = true;
    //Unpause the timer
    m_paused = false;
    //Get the current clock time
    m_start_ticks = SDL_GetTicks();
    // 由于可以在计时器暂停和/或运行时启动它，因此应确保清除暂停的数据
    m_paused_ticks = 0;
    INFOLOG("timerStart||name={}||m_start_ticks={}", m_name, m_start_ticks);
}

void SDLTimer::timerStop()
{
    //Stop the timer
    m_started = false;
    //Unpause the timer
    m_paused = false;
    //Clear tick variables
    m_start_ticks = 0;
    m_paused_ticks = 0;
}

void SDLTimer::timerPause()
{
    //If the timer is running and isn't already paused
    if( m_started && !m_paused )
    {
        //如果定时器正在运行，设置暂停标志，在 m_paused_ticks 中存储定时器暂停的时间，并重置开始时间
        //Pause the timer
        m_paused = true;
        //Calculate the paused ticks
        m_paused_ticks = SDL_GetTicks() - m_start_ticks;
        m_start_ticks = 0;
        INFOLOG("timerPause||name={}||m_start_ticks={}", m_name, m_start_ticks);
    }
}

void SDLTimer::timerUnpause()
{
    //当取消暂停定时器时，要确保定时器正在运行和暂停
    if( m_started && m_paused )
    {
        //Unpause the timer
        m_paused = false;
        //Reset the starting ticks
        m_start_ticks = SDL_GetTicks() - m_paused_ticks;
        //Reset the paused ticks
        m_paused_ticks = 0;
        INFOLOG("timerUnpause||name={}||m_start_ticks={}", m_name, m_start_ticks);
    }
}
 
// 获取当前定时器已运行的时间
Uint32 SDLTimer::timerGetTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( m_started )
    {
        //If the timer is paused
        if( m_paused )
        {
            //Return the number of ticks when the timer was paused
            time = m_paused_ticks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - m_start_ticks;
        }
    }
    return time;
}

bool SDLTimer::timerIsStarted()
{
    //Timer is running and paused or unpaused
    return m_started;
}

bool SDLTimer::timerIsPaused()
{
    //Timer is running and paused
    return m_paused && m_started;
}
