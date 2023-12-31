#include <iostream>

//位图字体类  包装字体的精灵表  当位图字体创建完成后，浏览纹理，找到 256 个字符的所有字符精灵（存储在 mChars 数组中），并计算新行和空格的距离
class LBitmapFont
{
    public:
        //The default constructor
        LBitmapFont();

        //构建字体
        bool buildFont( std::string path );

        //Deallocates font
        void free();

        //渲染文本
        void renderText( int x, int y, std::string text );

    private:
        //The font texture
        LTexture mFontTexture;

        //The individual characters in the surface
        SDL_Rect mChars[ 256 ];

        //Spacing Variables
        int mNewLine, mSpace;
};