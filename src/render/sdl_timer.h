#pragma once

#include <SDL2/SDL.h>
#include "logger.h"

//The application time based timer
/*
定时器类。该类具有启动/停止/暂停/取消定时器以及检查其状态的所有基本功能。
在数据成员方面，有开始时间，一个变量用于存储暂停时的时间，还有状态标志用于跟踪定时器是在运行还是暂停。
*/
class SDLTimer
{
private:
    //The clock time when the timer started
    Uint32 m_start_ticks;

    //储存当前定时器运行了多长时间
    Uint32 m_paused_ticks;

    //The timer status
    bool m_paused;
    bool m_started;

    std::string m_name;
public:
    //Initializes variables
    SDLTimer(const std::string& name);
    ~SDLTimer();

    //The various clock actions
    void timerStart();
    void timerStop();
    void timerPause();
    void timerUnpause();

    //Gets the timer's time
    Uint32 timerGetTicks();

    //Checks the status of the timer
    bool timerIsStarted();
    bool timerIsPaused();
};
