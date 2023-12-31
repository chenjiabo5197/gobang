
LTexture gRedTexture;
LTexture gGreenTexture;
LTexture gBlueTexture;
LTexture gShimmerTexture;

//粒子数量
const int TOTAL_PARTICLES = 20;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

class Particle
{
    public:
        //Initialize position and animation
        Particle( int x, int y );

        //Shows the particle
        void render();

        //Checks if particle is dead
        bool isDead();

    private:
        //Offsets
        int mPosX, mPosY;

        //一帧动画
        int mFrame;

        //Type of particle
        LTexture *mTexture;
};
