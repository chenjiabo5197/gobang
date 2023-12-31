#include "lbutton.h"

LButton gButtons[TOTAL_BUTTONS];

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}

// 该函数将在事件循环中调用，处理从事件队列中提取的单个按钮事件
void LButton::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    // 首先，检查输入的事件是否是鼠标事件，具体包括鼠标移动事件（鼠标移动时）、鼠标按键下移事件（点击鼠标按键时）或鼠标按键上移事件（释放鼠标点击时）
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        // 如果发生了这些鼠标事件之一，使用 SDL_GetMouseState 检查鼠标位置。根据鼠标是否位于按钮上方，我们需要显示不同的精灵
        int x, y;
        SDL_GetMouseState( &x, &y );

        //Check if mouse is in button
        /*
        检查鼠标是否位于按钮内。由于用的是与 SDL 不同的坐标系，因此按钮的原点位于左上方。
        这意味着每一个小于 x 位置的 x 坐标都在按钮外，每一个小于 y 位置的 y 坐标也在按钮外。按钮右侧的所有内容都是 x 位置 + 宽度，按钮下方的所有内容都是 y 位置 + 高度。
        这就是这段代码的作用。如果鼠标位置超出了按钮的任何范围，它就会将内部标记标记为假。否则，它将保持初始值为真
        */
        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }

        //Mouse is outside button
        /*
        根据鼠标是否位于按钮内和鼠标事件来设置按钮精灵。
        如果鼠标不在按钮内，设置鼠标移出精灵。如果鼠标在按钮内，将精灵设置为鼠标移动时鼠标移到上方，
        鼠标按下时鼠标移到下方，鼠标释放时鼠标移到上方
        */
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;
            
                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;
                
                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}

// 根据当前的鼠标事件(mCurrentSprite)渲染不同的图像(gSpriteClips)
void LButton::render()
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}


