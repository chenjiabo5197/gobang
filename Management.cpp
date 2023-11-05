#include "Management.h"

Management::Management(Player* player, AI* ai, Chess* chess, PictureDraw* pictureDraw)
{

	this->pictureDraw = pictureDraw;

	this->ai = ai;
	this->player = player;
	this->chess = chess;

	INFOLOG("Management::Management||construct management success");
}

void Management::play()
{
	this->pictureDraw->drawGraph(MAIN_MENU);
	MOUSEMSG msg;
	while (true)
	{
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			DEBUGLOG("Management::play||mouse click||x={}||y={}", msg.x, msg.y);
			// 可以选择开始游戏、查看排行榜、退出游戏
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->startGamePicture))
			{
				DEBUGLOG("Management::play||select start game");
				this->chooseGame();
			}
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->bestScoresPicture))
			{
				DEBUGLOG("Management::play||view best scores");
			}
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->exitGamePicture))
			{
				DEBUGLOG("Management::play||select exit game||game over");
				break;
			}
		}
	}
	INFOLOG("Management::play||game over");
}

void Management::chooseGame()
{
	this->pictureDraw->drawGraph(PLAYER_NUM_MENU);
	MOUSEMSG msg;
	while (true)
	{
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			DEBUGLOG("Management::chooseGame||mouse click||x={}||y={}", msg.x, msg.y);
			// 可以选择单人游戏、双人游戏、网络对战
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->onePlayerPicture))
			{
				DEBUGLOG("Management::chooseGame||select one player");
				this->onePlayerGame();
				if (GlobalVar::instance()->getValue("exitGame"))
				{
					INFOLOG("Management::chooseGame||player return to MAIN_MENU");
					break;
				}
			}
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->twoPlayersPicture))
			{
				DEBUGLOG("Management::chooseGame||select two player");
			}
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->playerInternetPicture))
			{
				DEBUGLOG("Management::chooseGame||select play on internet");
			}
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->backwardMenu))
			{
				DEBUGLOG("Management::chooseGame||select backward main menu");
				break;
			}
		}
	}
	this->pictureDraw->drawGraph(MAIN_MENU);
	INFOLOG("Management::chooseGame||return to MAIN_MENU");
}

void Management::onePlayerGame()
{
	this->onePlayerInit();
	this->pictureDraw->drawGraph(CHESSBOARD_MENU);
	MOUSEMSG msg;
	while (true)
	{
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			DEBUGLOG("Management::onePlayerGame||mouse click||x={}||y={}", msg.x, msg.y);

			// 判断点击区域是否在棋盘图片的尺寸内
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->chessBoardPicture))
			{
				DEBUGLOG("Management::onePlayerGame||click to chess||x={}||y={}", msg.x, msg.y);
				// 判断点击的区域是否有效，可以下棋
				if (this->player->go(msg.x, msg.y))  
				{
					if (chess->checkOver())
					{
						INFOLOG("Management::play||player win||init chess again");
						if (this->isAgainGame())
						{
							this->pictureDraw->drawGraph(CHESSBOARD_MENU);
							chess->init();
							continue;
						}
						else
						{
							break;
						}
					}

					ai->go();

					if (chess->checkOver())
					{
						INFOLOG("Management::play||AI win||init chess again");
						if (this->isAgainGame())
						{
							this->pictureDraw->drawGraph(CHESSBOARD_MENU);
							chess->init();
							continue;
						}
						else
						{
							break;
						}
					}
				}
			}

			// 可以选择悔棋或者直接返回主菜单
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->withDrawPicture))
			{
				DEBUGLOG("Management::onePlayerGame||select withDraw");
				this->chess->playerWithDraw();
			}
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->backwardMenu))
			{
				DEBUGLOG("Management::onePlayerGame||select backward main menu");
				GlobalVar::instance()->setValue("exitGame", true);
				break;
			}
		}
	}
	INFOLOG("Management::onePlayerGame||return to MAIN_MENU");
}

void Management::onePlayerInit()
{
	this->ai->init(this->chess);
	this->player->init(this->chess);
	this->chess->init();
}

bool Management::isAgainGame()
{
	auto flag = GlobalVar::instance()->getResultFlag();
	if (flag == PLAYER_WIN)   // 黑棋赢，玩家赢
	{
		mciSendString("play res/clap.mp3", 0, 0, 0);
		this->pictureDraw->drawGraph(WIN_MENU);
	}
	else
	{
		mciSendString("play res/失败.mp3", 0, 0, 0);
		this->pictureDraw->drawGraph(LOSE_MENU);
	}
	// 循环，接收选手选择
	while (1)
	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			DEBUGLOG("Management::isAgainGame||mouse click||x={}||y={}", msg.x, msg.y);
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->againGamePicture))
			{
				return true;
			}
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->backwardMenu))
			{

				GlobalVar::instance()->setValue("exitGame", true);
				return false;
			}
		}
	}
}
