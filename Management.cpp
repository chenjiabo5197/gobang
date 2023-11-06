#include "Management.h"

Management::Management(Player* player1, Player* player2, AI* ai, Chess* chess, PictureDraw* pictureDraw)
{

	this->pictureDraw = pictureDraw;

	this->ai = ai;
	this->player1 = player1;
	this->player2 = player2;
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
			// ����ѡ��ʼ��Ϸ���鿴���а��˳���Ϸ
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
			// ����ѡ������Ϸ��˫����Ϸ�������ս
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
				this->twoPlayersGame();
				if (GlobalVar::instance()->getValue("exitGame"))
				{
					INFOLOG("Management::chooseGame||player return to MAIN_MENU");
					break;
				}
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

			// �жϵ�������Ƿ�������ͼƬ�ĳߴ���
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->chessBoardPicture))
			{
				DEBUGLOG("Management::onePlayerGame||click to chess||x={}||y={}", msg.x, msg.y);
				// �жϵ���������Ƿ���Ч����������
				if (this->player1->go(msg.x, msg.y))  
				{
					if (chess->checkOver())
					{
						DEBUGLOG("Management::onePlayerGame||player game over");
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
						DEBUGLOG("Management::onePlayerGame||AI game over");
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

			// ����ѡ��������ֱ�ӷ������˵�
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
	this->chess->init();
	this->chess->gameKind = ONE_PLAYER_GAME;
	this->ai->init(this->chess);
	this->player1->init(this->chess, "player");
}

bool Management::isAgainGame()
{
	auto flag = GlobalVar::instance()->getResultFlag();
	if (flag == PLAYER_WIN)   // ����Ӯ�����Ӯ
	{
		mciSendString("play res/clap.mp3", 0, 0, 0);
		this->pictureDraw->drawGraph(WIN_MENU);
	}
	else if (flag == PLAYER_LOSE)
	{
		mciSendString("play res/ʧ��.mp3", 0, 0, 0);
		this->pictureDraw->drawGraph(LOSE_MENU);
	}
	else if (flag == RESULT_DRAW)
	{
		this->pictureDraw->drawGraph(DRAW_MENU);
	}
	else if (flag == BLACK_WIN)
	{
		this->pictureDraw->drawGraph(BLACK_WIN_MENU);
	}
	else if (flag == WHITE_WIN)
	{
		this->pictureDraw->drawGraph(WHITE_WIN_MENU);
	}
	// ѭ��������ѡ��ѡ��
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

void Management::twoPlayersGame()
{
	this->twoPlayersInit();
	this->pictureDraw->drawGraph(CHESSBOARD_MENU);
	MOUSEMSG msg;
	while (true)
	{
		if (this->oneOfTwoPlayer(this->player1))
		{
			if (this->oneOfTwoPlayer(this->player2))
			{
				continue;
			}
			else
			{
				break;
			}
		}
		else {
			break;
		}
	}
	INFOLOG("Management::twoPlayersGame||return to MAIN_MENU");
}

void Management::twoPlayersInit()
{
	this->chess->init();
	this->chess->gameKind = TWO_PLAYERS_GAME;
	this->player1->init(this->chess, "black_player", CHESS_BLACK);
	this->player2->init(this->chess, "white_player", CHESS_WHITE);
}

bool Management::oneOfTwoPlayer(Player* player)
{
	MOUSEMSG msg;
	while (true)
	{
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			DEBUGLOG("Management::twoPlayersGame||{} mouse click||x={}||y={}", player->playerName, msg.x, msg.y);

			// �жϵ�������Ƿ�������ͼƬ�ĳߴ���
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->chessBoardPicture))
			{
				DEBUGLOG("Management::twoPlayersGame||{} click to chess||x={}||y={}", player->playerName, msg.x, msg.y);
				// �жϵ���������Ƿ���Ч����������
				if (this->player2->go(msg.x, msg.y, player->chessKind))
				{
					if (chess->checkOver())
					{
						DEBUGLOG("Management::twoPlayersGame||player game over");
						if (this->isAgainGame())
						{
							this->pictureDraw->drawGraph(CHESSBOARD_MENU);
							chess->init();
							continue;
						}
						else
						{
							return false;
						}
					}
					// ��ǰplayer��Ч���ӣ���δ������ʤ�����л����巽
					return true;
				}
			}


			// ����ѡ��������ֱ�ӷ������˵�
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->withDrawPicture))
			{
				DEBUGLOG("Management::twoPlayersGame||{} select withDraw", player->playerName);
				this->chess->playerWithDraw();
			}
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->backwardMenu))
			{
				DEBUGLOG("Management::twoPlayersGame||{} select backward main menu", player->playerName);
				GlobalVar::instance()->setValue("exitGame", true);
				return false;
			}
		}
	}
}
