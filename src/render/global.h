#pragma once
#include <SDL2/SDL.h>
#include <vector>

#if defined(__linux__)
// Linux系统
#include<unistd.h>
#elif defined(_WIN32)
// Windows系统
#include<windows.h>
#endif

SDL_SpinLock gDataLock = 0;
int gData = -1;

struct Circle
{
    int x, y;
    int r;
};

double distanceSquared( int x1, int y1, int x2, int y2 )
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX*deltaX + deltaY*deltaY;
}

//圆对圆碰撞检测器。只需检测两个圆心之间的距离平方是否小于半径平方之和。如果小于，则说明发生了碰撞。
bool checkCollision( Circle& a, Circle& b )
{
    //Calculate total radius squared
    int totalRadiusSquared = a.r + b.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distanceSquared( a.x, a.y, b.x, b.y ) < ( totalRadiusSquared ) )
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

bool checkCollision( Circle& a, SDL_Rect& b )
{
    //Closest point on collision box
    int cX, cY;

    //如果圆心在方框的左边，那么最近点的 x 位置就在方框的左边
    if( a.x < b.x )
    {
        cX = b.x;
    }
    // 如果圆心位于方框右侧，则最近点的 x 位置位于方框右侧。
    else if( a.x > b.x + b.w )
    {
        cX = b.x + b.w;
    }
    // 如果圆心在方框内，则最近点的 x 位置与圆的 x 位置相同(即圆心位置)。
    else
    {
        cX = a.x;
    }
    //Find closest y offset
    if( a.y < b.y )
    {
        cY = b.y;
    }
    else if( a.y > b.y + b.h )
    {
        cY = b.y + b.h;
    }
    else
    {
        cY = a.y;
    }

    //如果盒子上最近的点与圆心之间的距离平方小于圆半径的平方，那么就发生了碰撞
    if( distanceSquared( a.x, a.y, cX, cY ) < a.r * a.r )
    {
        //This box and the circle have collided
        return true;
    }

    //If the shapes have not collided
    return false;
}

//检查两个方框组之间是否碰撞
bool checkCollision( std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b )
{
    //矩形的的x，y轴投影
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //查看A组碰撞框
    for( int Abox = 0; Abox < a.size(); Abox++ )
    {
        //Calculate the sides of rect A
        leftA = a[ Abox ].x;
        rightA = a[ Abox ].x + a[ Abox ].w;
        topA = a[ Abox ].y;
        bottomA = a[ Abox ].y + a[ Abox ].h;

        //查看B组碰撞框
        for( int Bbox = 0; Bbox < b.size(); Bbox++ )
        {
            //计算矩形B的的x，y轴投影
            leftB = b[ Bbox ].x;
            rightB = b[ Bbox ].x + b[ Bbox ].w;
            topB = b[ Bbox ].y;
            bottomB = b[ Bbox ].y + b[ Bbox ].h;

            //If no sides from A are outside of B
            if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
            {
                //检测到碰撞
                return true;
            }
        }
    }

    //如果两组碰撞盒都没有碰到
    return false;
}

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

//回调函数的第一个参数必须是一个 32 位整数，第二个参数必须是一个 void 指针，并且必须返回一个 32 位整数。
//在给定时间后向控制台打印一条信息。这里没有使用间隔参数，但通常用于需要重复的定时器回调。由于 void 指针可以指向任何内容，因此该函数将接收一个字符串并将其打印到控制台。
Uint32 callback( Uint32 interval, void* param )
{
    //Print callback message
    std::cout << "Callback called back with message: " << reinterpret_cast<char*>(param) << std::endl;
    return 0;
}

// 与回调函数一样，线程函数也需要以特定方式声明。需接收一个 void 指针作为参数，并返回一个整数。
int threadFunction( void* data )
{
    //接收整数数据，并用它向控制台打印一条信息
    //此处需要将data转化为long，因为为64为系统，void*为8字节，int为4字节，转化会丢失精度，转化为long就可以
    std::cout << "Running thread with value = " << (long)data << std::endl;

    return 0;
}

// semaphores锁可以允许单线程以外的访问。原子操作适用于需要严格锁定/解锁状态的情况
int worker( void* data )
{
    std::cout << static_cast<char*>(data) << " starting..." << std::endl;

    //Pre thread random seeding
    // 按线程进行时间设置随机数种子
    srand( SDL_GetTicks() );

    //Work 5 times
    // 每个线程半随机延迟、打印开始工作时的数据、为数据分配一个随机数、打印分配给数据缓冲区的数，然后再延迟一段时间，然后再次工作。之所以需要锁定数据，
    // 是因为不希望两个线程同时读写共享数据。
    for( int i = 0; i < 5; ++i )
    {
        //Wait randomly 半随机延迟
        SDL_Delay( 16 + rand() % 32 );
        
        //Lock  加锁  
        SDL_AtomicLock( &gDataLock );

        //Print pre work data  打印开始工作时的数据
        std::cout << static_cast<char*>(data) << " gets " << gData << std::endl;

        //"Work"  为数据分配一个随机数
        gData = rand() % 256;

        //Print post work data  打印分配给数据缓冲区的数
        std::cout << static_cast<char*>(data) << " sets " << gData << std::endl;
        
        //Unlock  释放锁  
        SDL_AtomicUnlock( &gDataLock );

        //Wait randomly  再延迟一段时间
        SDL_Delay( 16 + rand() % 640 );
    }

    std::cout << static_cast<char*>(data) << " finished!" << std::endl;

    return 0;
}

void my_p_sleep(int time) {
#if defined(__linux__)
	// Linux系统
	sleep(time);
#elif defined(_WIN32)
	// Windows系统
	Sleep((time*1000));
#endif
}
