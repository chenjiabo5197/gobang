#include <iostream>
#include "Management.h"
#include "spdlog/spdlog.h"
#include "logger.h"

// ��ʼ����־
void initLogger()
{
    // ������־������
    LogConfig conf = {
        "trace",
        "logs/gobang.log",
        100 * 1024 * 1024,
        10
    };
    INITLOG(conf);
    INFOLOG("init logger success!");
    // ��־��ʼ����Ϊtrace,д��־��demo
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



