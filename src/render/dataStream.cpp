
#include "dataStream.h"

//Animation stream
DataStream gDataStream;

DataStream::DataStream()
{
	mImages[ 0 ] = nullptr;
	mImages[ 1 ] = nullptr;
	mImages[ 2 ] = nullptr;
	mImages[ 3 ] = nullptr;

	mCurrentImage = 0;
	mDelayFrames = 4;
}

bool DataStream::loadMedia()
{
	bool success = true;
	
	for( int i = 0; i < 4; ++i )
	{
		std::stringstream path;
		path << "foo_walk_" << i << ".png";

		SDL_Surface* loadedSurface = IMG_Load( path.str().c_str() );
		if( loadedSurface == nullptr )
		{
			std::cout << "Unable to load " << path.str().c_str() << "! SDL_image error: " << IMG_GetError() << std::endl;
			success = false;
		}
		else
		{
            // 将现有的表面复制到指定格式的新的表面 第一个参数为源表面指针，第二个参数为像素格式，第三个参数设置为0即可,返回为创建的新 SDL_Surface 结构
			mImages[ i ] = SDL_ConvertSurfaceFormat( loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0 );
		}

		SDL_FreeSurface( loadedSurface );
	}

	return success;
}

void DataStream::free()
{
	for( int i = 0; i < 4; ++i )
	{
		SDL_FreeSurface( mImages[ i ] );
		mImages[ i ] = nullptr;
	}
}

void* DataStream::getBuffer()
{
	--mDelayFrames;
	if( mDelayFrames == 0 )
	{
		++mCurrentImage;
		mDelayFrames = 4;
	}

	if( mCurrentImage == 4 )
	{
		mCurrentImage = 0;
	}

	return mImages[ mCurrentImage ]->pixels;
}
