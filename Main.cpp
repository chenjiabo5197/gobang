#include <iostream>
#include "Management.h"
#include "spdlog/spdlog.h"
#include "logger.h"

// 初始化日志
void initLogger()
{
    // 定义日志配置项
    LogConfig conf = {
        "trace",
        "logs/gobang.log",
        100 * 1024 * 1024,
        10
    };
    INITLOG(conf);
    INFOLOG("init logger success!");
    // 日志初始级别为trace,写日志的demo
    /*TRACELOG("current log level is {}", GETLOGLEVEL());
    TRACELOG("this is trace log");
    DEBUGLOG("this is debug log");
    INFOLOG("this is info log");
    WARNLOG("this is warning log");
    ERRORLOG("this is a error log");
    CRITICALLOG("this is critical log");*/
}

int main()
{
    initLogger();

	Chess chess(897, 895, 13, 44, 43, 67.4);
	Player player;
	AI ai;
	Management game(&player, &ai, &chess);

	game.play();

	

	return 0;
}



