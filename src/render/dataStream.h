
//A test animation stream
class DataStream
{
    public:
        //Initializes internals
        DataStream();

        //Loads initial data
        bool loadMedia();

        //Deallocator
        void free();

        //从数据缓冲区获取当前像素
        void* getBuffer();

    private:
        //Internal data
        SDL_Surface* mImages[ 4 ];
        int mCurrentImage;
        int mDelayFrames;
};
