
Particle::Particle( int x, int y )
{
    // 在给定位置周围初始化位置，并在其中加入一些随机性。然后，使用随机性初始化动画帧，这样粒子就会有不同的寿命。最后，将随机选择用于粒子的纹理类型
    //Set offsets
    mPosX = x - 5 + ( rand() % 25 );
    mPosY = y - 5 + ( rand() % 25 );

    //Initialize animation
    mFrame = rand() % 5;

    //Set type
    switch( rand() % 3 )
    {
        case 0: mTexture = &gRedTexture; break;
        case 1: mTexture = &gGreenTexture; break;
        case 2: mTexture = &gBlueTexture; break;
    }
}

// 渲染在构造函数中选择的纹理，然后每隔一帧在纹理上渲染一个半透明的微光纹理，使粒子看起来像在发光。然后更新动画帧。
void Particle::render()
{
    //Show image
    mTexture->render( mPosX, mPosY );

    //Show shimmer
    if( mFrame % 2 == 0 )
    {
        gShimmerTexture.render( mPosX, mPosY );
    }

    //Animate
    mFrame++;
}

//一旦粒子渲染了最多 10 帧，将其标记为死亡。
bool Particle::isDead()
{
    return mFrame > 10;
}
