/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2023-12-30
* Description: This is main file
* Copyright (c) 2023, All rights reserved
=============================================*/

#include <iostream>
// #include "management.h"
#include "logger/logger.h"
// #include "pictureDraw.h"

// 初始化日志
void initLogger()
{
    // 定义日志配置项
    LogConfig conf = {
        "trace",
        //"info",
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

    // PictureDraw pictureDraw(13, 44, 43, 67.4);

	// Chess chess(13, 44, 43, 67.4, &pictureDraw);
	// Player player1;
	// Player player2;
	// AI ai;
	// Management game(&player1, &player2, &ai, &chess, &pictureDraw);

	// game.play();

	return 0;
}



