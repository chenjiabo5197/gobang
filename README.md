# gobang

#### 介绍
用c++写的五子棋对战，基于SDL2图像库、spdlog日志库，使用CMakeLists.txt方式编译代码
#### 软件架构
软件架构说明


#### 安装教程

1.  安装SDL2  
        sudo apt-get install libsdl2-dev  
        sudo apt-cache search libsdl2-image
        sudo apt-cache search libsdl2-ttf-dev
        sudo apt-cache search libsdl2-mixer-dev.
2.  在本地/usr/lib/x86_64-linux-gnu/cmake下新建sdl2_image、sdl2_ttf、sdl2_mixer等文件夹
3.  各自文件夹里面新建文件,例sdl2_image下新建文件sdl2_image-config.cmake  
        文件内容:  
        set(prefix "/usr")  
        set(exec_prefix "\\\${prefix}")    
        set(libdir "\\\${prefix}/lib/x86_64-linux-gnu")  
        set(SDL2_IMAGE_PREFIX "/usr")  
        set(SDL2_IMAGE_EXEC_PREFIX "/usr")  
        set(SDL2_IMAGE_INCLUDE_DIRS "\\\${prefix}/include/SDL2_image")  
        set(SDL2_IMAGE_LIBRARIES "-lSDL2_image")  
4.  安装spdlog  
        代码库:https://github.com/gabime/spdlog/tree/v1.x  
        使用代码自带的CMakeLists.txt安装，命令:make install

#### 使用说明

1.  xxxx
2.  xxxx
3.  xxxx

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

#### 疑问
1.  在下棋中，机器人方使用多线程下棋，但是为什么加了SDL_AtomicLock(&player_machine_lock)原子锁反而在执行机器人思考步骤时，玩家方也会暂停，去除原子锁之后恢复正常？？？    
    因为机器人下棋时加了原子锁，玩家虽然是多线程，但是在等待原子锁过程中一直卡在等待锁那一行，所以这时候表现为玩家这段卡顿。可以删除原子锁，因为使用player_flag参数确保玩家和机器人是轮流下棋的。


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
