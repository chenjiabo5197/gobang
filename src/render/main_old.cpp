#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "global.h"
#include "ltimer.h"

/* 
sdl2 多线程 通过互斥和条件让线程互相告知何时解锁
一个填充缓冲区的生产者，一个清空缓冲区的消费者。这两个线程不仅不能同时使用同一个缓冲区，而且消费者不能从一个空缓冲区读取数据，生产者也不能填充一个已满的缓冲区。
使用互斥（mutex）来防止两个线程抓取同一段数据，并使用条件来让线程知道什么时候可以消费，什么时候可以生产。
*/

//全部面积大小
// 由于渲染的大小不再是屏幕的大小，必须有一组单独的常量来定义渲染的大小。
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//当前框大小
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

TTF_Font* gFont = nullptr; 
// 渲染的窗口
SDL_Window * gWindow = nullptr;
//The window renderer
SDL_Renderer* gRenderer = nullptr;

LTexture gTargetTexture;
LTexture gSplashTexture;

// 声明线程将使用的互斥和条件
//The protective mutex
SDL_mutex* gBufferLock = nullptr;

//The conditions
SDL_cond* gCanProduce = nullptr;
SDL_cond* gCanConsume = nullptr;

//The "data buffer"
extern int gData;

//生成缓冲区意味着生成一个随机数
void produce()
{
    //Lock
    SDL_LockMutex( gBufferLock );
    
    //If the buffer is full
    if( gData != -1 )
    {
        //Wait for buffer to be cleared
        std::cout <<"Producer encountered full buffer, waiting for consumer to empty buffer..." << std::endl;
        SDL_CondWait( gCanProduce, gBufferLock );
    }

    //Fill and show buffer
    gData = rand() % 255;
    std::cout <<"Produced " << gData << std::endl;
    
    //Unlock
    SDL_UnlockMutex( gBufferLock );
    
    //Signal consumer
    SDL_CondSignal( gCanConsume );
}

//消耗缓冲区意味着重置生成的数
void consume()
{
    //Lock
    SDL_LockMutex( gBufferLock );
    
    //If the buffer is empty
    if( gData == -1 )
    {
        //Wait for buffer to be filled
        std::cout <<"Consumer encountered empty buffer, waiting for producer to fill buffer..." << std::endl;
        SDL_CondWait( gCanConsume, gBufferLock );
    }

    //Show and empty buffer
    std::cout <<"Consumed " << gData << std::endl;
    gData = -1;
    
    //Unlock
    SDL_UnlockMutex( gBufferLock );
    
    //Signal producer
    SDL_CondSignal( gCanProduce );
}

int producer( void *data )
{
    std::cout <<"Producer started..." << std::endl;

    //Seed thread random
    srand( SDL_GetTicks() );
    
    //Produce
    for( int i = 0; i < 5; ++i )
    {
        //Wait
        SDL_Delay( rand() % 1000 );
        
        //Produce
        produce();
    }

    std::cout <<"Producer finished!" << std::endl;
    
    return 0;

}

int consumer( void *data )
{
    std::cout <<"Consumer started..." << std::endl;

    //Seed thread random
    srand( SDL_GetTicks() );

    for( int i = 0; i < 5; ++i )
    {
        //Wait
        SDL_Delay( rand() % 1000 );
        
        //Consume
        consume();
    }
    
    std::cout <<"Consumer finished!" << std::endl;

    return 0;
}

//Render text r,g,b,a
SDL_Color textColor = { 0, 0xff, 0xff, 0xff };


int main( int argc, char* args[] )
{
    
}



