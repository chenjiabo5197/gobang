#include <iostream>
#include "management.h"
#include "spdlog/spdlog.h"
#include "logger.h"
#include "pictureDraw.h"

// ��ʼ����־
void initLogger()
{
    // ������־������
    LogConfig conf = {
        "trace",
        //"info",
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

    PictureDraw pictureDraw(13, 44, 43, 67.4);

	Chess chess(13, 44, 43, 67.4, &pictureDraw);
	Player player1;
	Player player2;
	AI ai;
	Management game(&player1, &player2, &ai, &chess, &pictureDraw);

	game.play();

	return 0;
}



