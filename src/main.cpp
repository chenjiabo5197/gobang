/*============================================
* Author: chenjiabo
* E-mail: chen_wangyi666@163.com
* Date: 2023-12-30
* Description: This is main file
* Copyright (c) 2023, All rights reserved
=============================================*/

#include <iostream>
#include "manage/manage.h"
#include "logger/logger.h"
#include "utils/config.h"

// 初始化配置文件
Config initConfig(const std::string& config_path)
{  
    Config configSettings(config_path);
    // int port;  
    // std::string ipAddress;
    // port = configSettings.Read("port", 0);  
    // ipAddress = configSettings.Read("ipAddress", ipAddress); 
    return configSettings;
}

// 初始化日志
void initLogger(const Config& config)
{
    std::string str_temp;
    std::string log_level = config.Read("log_level", str_temp);
    std::string log_path = config.Read("log_path", str_temp);
    std::string log_filename = config.Read("log_filename", str_temp);
    int log_filesize = config.Read("log_filesize", 0);
    int log_filenums = config.Read("log_filenums", 0);
    std::string path = log_path + "/" + log_filename;
    // 定义日志配置项
    LogConfig conf = {
        log_level,
        //"info",
        path,
        log_filesize * 1024 * 1024,
        log_filenums
    };
    INITLOG(conf);
    INFOLOG("init logger success, log_level={}, path={}, log_filesize={}, log_filenums={}", log_level, path, log_filesize, log_filenums);
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
    Config config = initConfig("config/config.txt");
    initLogger(config);
    Manage manage(config);
    manage.start();

    // PictureDraw pictureDraw(13, 44, 43, 67.4);

	// Chess chess(13, 44, 43, 67.4, &pictureDraw);
	// Player player1;
	// Player player2;
	// AI ai;
	// Management game(&player1, &player2, &ai, &chess, &pictureDraw);

	// game.play();

	return 0;
}



