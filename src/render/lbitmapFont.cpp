#include "lbitmapFont.h"

LBitmapFont::LBitmapFont()
{
    //Initialize variables
    mNewLine = 0;
    mSpace = 0;
}

// 通过位图字体定义所有精灵的剪辑矩形
bool LBitmapFont::buildFont( std::string path )
{
    //Get rid of preexisting texture
    free();

    //加载位图字体像素
    bool success = true;
    if( !mFontTexture.loadPixelsFromFile( path ) )
    {
        std::cout << "Unable to load bitmap font surface!" << std::endl;
        success = false;
    }
    else
    {
        /*
        位图字体图中所有单元格都具有相同的宽度和高度，按 16 列和 16 行排列，并按 ASCII 顺序排列。
        位图字体加载器将遍历每个单元格，找到字形精灵的边，并为精灵设置夹角矩形。
        首先，需要获得背景颜色，以便找到字形精灵的边缘。然后计算单元格的宽度和高度。使用名为 top 的变量来跟踪精灵表中最高字形的顶部。
        变量 baseA 将记录大写 A 字形底部的偏移量，作为渲染字符的基准线。最后是 currentChar，它将记录正在查找的当前字符字形
        */
        //Get the background color
        Uint32 bgColor = mFontTexture.getPixel32( 0, 0 );

        //Set the cell dimensions
        int cellW = mFontTexture.getWidth() / 16;
        int cellH = mFontTexture.getHeight() / 16;

        //New line variables
        int top = cellH;
        int baseA = cellH;

        //The current character we're setting
        int currentChar = 0;

        /*
        for 循环用于遍历单元格行/列。
        在每个单元格循环的顶部，初始化位于单元格顶部的字形精灵位置，并将精灵尺寸设置为单元格尺寸。
        这意味着默认情况下，字形精灵是整个单元格。
        */
        //Go through the cell rows
        for( int rows = 0; rows < 16; ++rows )
        {
            //Go through the cell columns
            for( int cols = 0; cols < 16; ++cols )
            {
                //Set the character offset
                mChars[ currentChar ].x = cellW * cols;
                mChars[ currentChar ].y = cellH * rows;

                //Set the dimensions of the character
                mChars[ currentChar ].w = cellW;
                mChars[ currentChar ].h = cellH;

                /*
                对于每个单元格，都需要遍历单元格中的所有像素，以找到字形精灵的边缘。
                在这个循环中，从上到下查看每一列，寻找第一个不是背景色的像素。一旦找到不是背景色的像素，就意味着找到了精灵的左边缘
                找到字形左侧后，将其设置为精灵的 x 位置，然后中断循环
                */
                //Find Left Side
                //Go through pixel columns
                for( int pCol = 0; pCol < cellW; ++pCol )
                {
                    //Go through pixel rows
                    for( int pRow = 0; pRow < cellH; ++pRow )
                    {
                        //Get the pixel offsets
                        int pX = ( cellW * cols ) + pCol;
                        int pY = ( cellH * rows ) + pRow;

                        //If a non colorkey pixel is found
                        if( mFontTexture.getPixel32( pX, pY ) != bgColor )
                        {
                            //Set the x offset
                            mChars[ currentChar ].x = pX;

                            //Break the loops
                            pCol = cellW;
                            pRow = cellH;
                        }
                    }
                }
                /*
                查找右侧的像素。它的工作原理与查找左侧像素基本相同，只是现在是从右向左移动，而不是从左向右移动。
                找到右边的像素后，用它来设置宽度。由于像素数组从 0 开始，需要在宽度上加 1
                */
                //Find Right Side
                //Go through pixel columns
                for( int pColW = cellW - 1; pColW >= 0; --pColW )
                {
                    //Go through pixel rows
                    for( int pRowW = 0; pRowW < cellH; ++pRowW )
                    {
                        //Get the pixel offsets
                        int pX = ( cellW * cols ) + pColW;
                        int pY = ( cellH * rows ) + pRowW;

                        //If a non colorkey pixel is found
                        if( mFontTexture.getPixel32( pX, pY ) != bgColor )
                        {
                            //Set the width
                            mChars[ currentChar ].w = ( pX - mChars[ currentChar ].x ) + 1;

                            //Break the loops
                            pColW = -1;
                            pRowW = cellH;
                        }
                    }
                }

                /*
                精灵顶部。当找到一个比当前最高顶点高的顶点时，就会将其设置为新顶点。
                请注意，由于 y 轴是反向的，因此最高顶点的 y 偏移量实际上是最小的
                */
                //Find Top
                //Go through pixel rows
                for( int pRow = 0; pRow < cellH; ++pRow )
                {
                    //Go through pixel columns
                    for( int pCol = 0; pCol < cellW; ++pCol )
                    {
                        //Get the pixel offsets
                        int pX = ( cellW * cols ) + pCol;
                        int pY = ( cellH * rows ) + pRow;

                        //If a non colorkey pixel is found
                        if( mFontTexture.getPixel32( pX, pY ) != bgColor )
                        {
                            //If new top is found
                            if( pRow < top )
                            {
                                top = pRow;
                            }

                            //Break the loops
                            pCol = cellW;
                            pRow = cellH;
                        }
                    }
                }

                // 对于这个位图字体生成器，使用 A 字形精灵的底部作为基准线，这样像 "g"、"j"、"y "等悬挂在基准线以下的字符就不会定义底部
                //Find Bottom of A
                if( currentChar == 'A' )
                {
                    //Go through pixel rows
                    for( int pRow = cellH - 1; pRow >= 0; --pRow )
                    {
                        //Go through pixel columns
                        for( int pCol = 0; pCol < cellW; ++pCol )
                        {
                            //Get the pixel offsets
                            int pX = ( cellW * cols ) + pCol;
                            int pY = ( cellH * rows ) + pRow;

                            //If a non colorkey pixel is found
                            if( mFontTexture.getPixel32( pX, pY ) != bgColor )
                            {
                                //Bottom of a is found
                                baseA = pRow;

                                //Break the loops
                                pCol = cellW;
                                pRow = -1;
                            }
                        }
                    }
                }

                //Go to the next character
                ++currentChar;
            }
        }

        /*
        完成所有精灵的定义后，需要进行一些后期处理。首先，要计算一个空格有多长。这里将其定义为半个单元格的宽度。然后，利用基线和最高精灵顶计算新行的高度。
        然后，将每个字形顶部多余的空格去掉，以防止行与行之间的空格过大。最后，完成位图字体纹理的加载
        */
        //Calculate space
        mSpace = cellW / 2;

        //Calculate new line
        mNewLine = baseA - top;

        //Lop off excess top pixels
        for( int i = 0; i < 256; ++i )
        {
            mChars[ i ].y += top;
            mChars[ i ].h -= top;
        }

        //Create final texture
        if( !mFontTexture.loadFromPixels() )
        {
            std::cout << "Unable to create font texture!" << std::endl;
            success = false;
        }
    }
    return success;
}

void LBitmapFont::renderText( int x, int y, std::string text )
{
    //首先，检查是否有要渲染的位图（至少要有一个像素宽），然后声明用于渲染当前字形精灵的 x/y 偏移量。
    if( mFontTexture.getWidth() > 0 )
    {
        //Temp offsets
        int curX = x, curY = y;

        //通过字符串渲染每个字形精灵。不过，有两个 ASCII 值实际上不会进行任何渲染。当出现空格时，要做的就是在空格宽度上移动。当出现新行时，向下移动一行，然后回到基本 x 偏移量
        //Go through the text
        for( int i = 0; i < text.length(); ++i )
        {
            //If the current character is a space
            if( text[ i ] == ' ' )
            {
                //Move over
                curX += mSpace;
            }
            //If the current character is a newline
            else if( text[ i ] == '\n' )
            {
                //Move down
                curY += mNewLine;

                //Move back
                curX = x;
            }

            else
            {
                //Get the ASCII value of the character
                int ascii = (unsigned char)text[ i ];

                //Show the character
                mFontTexture.render( curX, curY, &mChars[ ascii ] );

                //Move over the width of the character with one pixel of padding
                curX += mChars[ ascii ].w + 1;
            }
        }
    }
}

void LBitmapFont::free()
{
	mFontTexture.free();
}

LBitmapFont gBitmapFont;
