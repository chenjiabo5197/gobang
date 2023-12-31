#include "dot.h"


int Dot::getPosX()
{
    return mPosX;
}

int Dot::getPosY()
{
    return mPosY;
}

Dot::Dot(int x, int y)
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    //Set collision circle size
    mCollider.r = DOT_WIDTH / 2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //Move collider relative to the circle
    shiftColliders();
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    // 如果每次按右键时都增加 x 位置，那么就必须反复按右键才能保持移动。而通过设置速度，只需按一次键即可
    //要检查按键重复次数是否为 0，因为按键重复次数默认是启用的，如果按住一个键，它会报告多次按键。这意味着必须检查按键是否是第一次按下，因为只关心按键第一次按下的时间
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

Dot::Dot()
{
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //Initialize particles
    // for( int i = 0; i < TOTAL_PARTICLES; ++i )
    // {
    //     particles[ i ] = new Particle( mPosX, mPosY );
    // }
}

Dot::~Dot()
{
    //Delete particles
    // for( int i = 0; i < TOTAL_PARTICLES; ++i )
    // {
    //     delete particles[ i ];
    // }
}

// 每帧移动圆点时调用的函数
void Dot::move(std::vector<SDL_Rect>& otherColliders)
{
    //Move the dot left or right
    mPosX += mVelX;
    shiftColliders();
    std::cout << "dot move" << ", mPosX=" << mPosX << std::endl;

    /*
    沿 x 轴移动圆点，同时也改变碰撞器的位置。每当改变点的位置时，碰撞器的位置也必须跟着改变。
    然后，检查点是否离开屏幕或撞到墙壁。如果有，就将点沿 x 轴向后移动。最后，再对 Y 轴上的运动做一次这样的检查。
    */
    //若点相撞或过于偏左或偏右
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision( this->mColliders, otherColliders ) )
    {
        //Move back
        mPosX -= mVelX;
        shiftColliders();
    }

    //Move the dot up or down
    mPosY += mVelY;
    shiftColliders();
    std::cout << "dot move" << ", mPosY=" << mPosY << std::endl;

    //若点相撞或过于偏上或偏下
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( this->mColliders, otherColliders )  )
    {
        //Move back
        mPosY -= mVelY;
        shiftColliders();
    }
}

void Dot::move( SDL_Rect& square, Circle& circle )
{
    //Move the dot left or right
    mPosX += mVelX;
    shiftColliders();
    std::cout << "dot move" << ", mPosX=" << mPosX << ",mVelX=" << mVelX << std::endl;
    bool temp1 = checkCollision( mCollider, square );
    bool temp2 = checkCollision( mCollider, circle );
    std::cout << "dot move" << ", temp1=" << temp1 << ",temp2=" << temp2 << std::endl;

    //If the dot collided or went too far to the left or right
    if( ( mPosX - mCollider.r < 0 ) || ( mPosX + mCollider.r > SCREEN_WIDTH ) || checkCollision( mCollider, square ) || checkCollision( mCollider, circle ) )
    {
        //Move back
        mPosX -= mVelX;
        shiftColliders();
    }

    //Move the dot up or down
    mPosY += mVelY;
    shiftColliders();
    std::cout << "dot move" << ", mPosY=" << mPosY << ",mVelY=" << mVelY << std::endl;

    //If the dot collided or went too far up or down
    if( ( mPosY - mCollider.r < 0 ) || ( mPosY + mCollider.r > SCREEN_HEIGHT ) || checkCollision( mCollider, square ) || checkCollision( mCollider, circle ) )
    {
        //Move back
        mPosY -= mVelY;
        shiftColliders();
    }
}

void Dot::move()
{
    //Move the dot left or right
    mPosX += mVelX;
    std::cout << "dot move" << ", mPosX=" << mPosX << ",mVelX=" << mVelX << std::endl;

    // 要检查圆点是否移出了水平面，而不是检查圆点是否移出了屏幕，因为屏幕会在水平面周围移动
    //If the dot went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH) )
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the dot up or down
    mPosY += mVelY;
    std::cout << "dot move" << ", mPosY=" << mPosY << ",mVelY=" << mVelY << std::endl;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Dot::move( float timeStep )
{
    //Move the dot left or right
    mPosX += mVelX * timeStep;

    //If the dot went too far to the left or right
    if( mPosX < 0 )
    {
        mPosX = 0;
    }
    else if( mPosX > SCREEN_WIDTH - DOT_WIDTH )
    {
        mPosX = SCREEN_WIDTH - DOT_WIDTH;
    }
    
    //Move the dot up or down
    mPosY += mVelY * timeStep;

    //If the dot went too far up or down
    if( mPosY < 0 )
    {
        mPosY = 0;
    }
    else if( mPosY > SCREEN_HEIGHT - DOT_HEIGHT )
    {
        mPosY = SCREEN_HEIGHT - DOT_HEIGHT;
    }
}

void Dot::shiftColliders()
{
    //行偏移量，每行碰状况高度不一致，所以需要变量记录当前行碰状况的y值
    // int r = 0;

    //查看点的碰撞框组
    // for( int set = 0; set < mColliders.size(); ++set )
    // {
    //     //将碰撞框居中
    //     mColliders[ set ].x = mPosX + ( DOT_WIDTH - mColliders[ set ].w ) / 2;

    //     //根据行偏移量设置碰撞框
    //     mColliders[ set ].y = mPosY + r;

    //     //将行偏移量向下移动碰撞框的高度
    //     r += mColliders[ set ].h;
    // }
    this->mCollider.x = this->mPosX;
    this->mCollider.y = this->mPosY;
}

std::vector<SDL_Rect>& Dot::getColliders()
{
    return mColliders;
}

Circle& Dot::getCollider()
{
    return mCollider;
}

void Dot::render()
{
     //Show the dot
    // gDotTexture.render( mPosX, mPosY );
    gDotTexture.render( (int)mPosX, (int)mPosY );

    //Show particles on top of dot
    // renderParticles();
}

// 将对象渲染到屏幕上时
void Dot::render( int camX, int camY )
{
    //Show the dot relative to the camera
    gDotTexture.render( mPosX - camX, mPosY - camY );
}

void Dot::renderParticles()
{
    //Go through particles
    // for( int i = 0; i < TOTAL_PARTICLES; ++i )
    // {
    //     //粒子渲染函数会检查是否有已死亡的粒子，并替换它们
    //     if( particles[ i ]->isDead() )
    //     {
    //         delete particles[ i ]; // 堆上的对象，用delete回收
    //         particles[ i ] = new Particle( mPosX, mPosY );
    //     }
    // }

    // //替换掉死粒子后，将所有当前粒子渲染到屏幕上
    // for( int i = 0; i < TOTAL_PARTICLES; ++i )
    // {
    //     particles[ i ]->render();
    // }
}

// 当移动圆点时，检查它是否偏离水平线或碰到墙砖
/*
void Dot::move( Tile *tiles[] )
{
    //Move the dot left or right
    mBox.x += mVelX;

    //If the dot went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
    {
        //move back
        // mBox.x -= mVelX;
        mVelX = -mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;

    //If the dot went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
    {
        //move back
        // mBox.y -= mVelY;
        mVelY = -mVelY;
    }
}*/

void Dot::setCamera( SDL_Rect& camera )
{
    //Center the camera over the dot
    camera.x = ( mBox.x + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
    camera.y = ( mBox.y + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

    //Keep the camera in bounds
    if( camera.x < 0 )
    { 
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > LEVEL_WIDTH - camera.w )
    {
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if( camera.y > LEVEL_HEIGHT - camera.h )
    {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}

void Dot::render( SDL_Rect& camera )
{
    //Show the dot
    gDotTexture.render( mBox.x - camera.x, mBox.y - camera.y );
}

