//点类，可以在屏幕上移动
class Dot
{
    public:
        //点的尺寸
        static const int DOT_WIDTH = 20;
        static const int DOT_HEIGHT = 20;

        //点的最大轴速度  在每秒 60 帧的应用中，每秒 640 像素相当于每帧 10 像素多一点
        static const int DOT_VEL = 640;

        //Initializes the variables
        Dot(int x, int y);

        Dot();

        ~Dot();

        //按下按键并调整点的速度
        void handleEvent( SDL_Event& e );

        //移动点并检查碰撞 移动函数将接收一个碰撞方框组，这样就可以对两组碰撞方框进行检查
        void move(std::vector<SDL_Rect>& otherColliders);

        //Moves the dot and checks collision
        void move( SDL_Rect& square, Circle& circle );

        void move();

        //Moves the dot 为了根据帧时间移动，移动函数需要知道每帧移动的时间。移动功能需要一个时间步长，也就是自上次更新后过去了多少时间
        void move( float timeStep );

        //移动圆点并检查与瓷砖的碰撞
        // void move( Tile *tiles[] );

        //Centers the camera over the dot
        void setCamera( SDL_Rect& camera );

        //Shows the dot on the screen
        void render( SDL_Rect& camera );

        //在屏幕上渲染点
        void render();

        // 渲染屏幕上相对于当前框的点
        // 点必须相对于当前框进行渲染，因此它的渲染函数接收了当前框的位置
        void render( int camX, int camY );

        //获取每个点的碰撞方框组
        std::vector<SDL_Rect>& getColliders();

         //Gets collision circle
        Circle& getCollider();

        //Position accessors
        int getPosX();
        int getPosY();

    private:
        //The particles
        // Particle* particles[ TOTAL_PARTICLES ];

        //Shows the particles
        void renderParticles();

        //Collision box of the dot
        SDL_Rect mBox;

        //点的坐标
        // int mPosX, mPosY;
        float mPosX, mPosY;

        //点的速度
        // int mVelX, mVelY;
        float mVelX, mVelY;

        //点的碰撞方框组
        std::vector<SDL_Rect> mColliders;

        //Dot's collision circle
        Circle mCollider;

        //根据点的坐标移动碰撞框组
        void shiftColliders();
};
