#include "Management.h"

extern bool exitGame;
extern result_flag resultFlag;

Management::Management(Player* player1, Player* player2, AI* ai, Chess* chess, PictureDraw* pictureDraw)
{

	this->pictureDraw = pictureDraw;
	this->bestScores = MyUtils::initBestScores();

	this->ai = ai;
	this->player1 = player1;
	this->player2 = player2;
	this->chess = chess;

	this->isUpdateBestScoreFile = false;

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
				this->showBestScores();
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

Management::~Management()
{
	if (this->isUpdateBestScoreFile)
	{
		MyUtils::saveVectorToCsv(this->bestScores);
		DEBUGLOG("Management::~Management||isUpdateBestScoreFile=true||update bestScoresFile.csv success!");
	}
	DEBUGLOG("Management::~Management");
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
				if (exitGame)
				{
					INFOLOG("Management::chooseGame||player return to MAIN_MENU");
					break;
				}
			}
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->twoPlayersPicture))
			{
				DEBUGLOG("Management::chooseGame||select two player");
				this->twoPlayersGame();
				if (exitGame)
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

void Management::showBestScores()
{
	this->pictureDraw->drawGraph(BEST_SCORE_MENU);
	this->pictureDraw->drawText(this->bestScores);
	MOUSEMSG msg;
	while (true)
	{
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			DEBUGLOG("Management::showBestScores||mouse click||x={}||y={}", msg.x, msg.y);
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->backwardMenu))
			{
				DEBUGLOG("Management::showBestScores||select backward main menu");
				break;
			}
		}
	}
	this->pictureDraw->drawGraph(MAIN_MENU);
	INFOLOG("Management::showBestScores||return to MAIN_MENU");
}

void Management::onePlayerGame()
{
	this->onePlayerInit();
	this->pictureDraw->drawGraph(CHESSBOARD_MENU);
	MOUSEMSG msg;
	while (true)
	{
		if (this->onePlayer(this->player1))
		{
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
		else {
			break;
		}
	}
	INFOLOG("Management::onePlayerGame||return to MAIN_MENU");
}

void Management::onePlayerInit()
{
	this->chess->init();
	this->chess->gameKind = ONE_PLAYER_GAME;
	this->ai->init(this->chess);
	this->player1->init(this->chess, "player1");
}

bool Management::isAgainGame()
{
	if (resultFlag == PLAYER_WIN)   // ����Ӯ�����Ӯ
	{
		mciSendString("play res/clap.mp3", 0, 0, 0);
		this->pictureDraw->drawGraph(WIN_MENU);
	}
	else if (resultFlag == PLAYER_LOSE)
	{
		mciSendString("play res/ʧ��.mp3", 0, 0, 0);
		this->pictureDraw->drawGraph(LOSE_MENU);
	}
	else if (resultFlag == RESULT_DRAW)
	{
		this->pictureDraw->drawGraph(DRAW_MENU);
	}
	else if (resultFlag == BLACK_WIN)
	{
		this->pictureDraw->drawGraph(BLACK_WIN_MENU);
	}
	else if (resultFlag == WHITE_WIN)
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

				exitGame = true;
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
		if (this->onePlayer(this->player1))
		{
			if (this->onePlayer(this->player2))
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

bool Management::onePlayer(Player* player)
{
	MOUSEMSG msg;
	while (true)
	{
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)
		{
			DEBUGLOG("Management::onePlayer||{} mouse click||x={}||y={}", player->playerName, msg.x, msg.y);

			// �жϵ�������Ƿ�������ͼƬ�ĳߴ���
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->chessBoardPicture))
			{
				DEBUGLOG("Management::onePlayer||{} click to chess||x={}||y={}", player->playerName, msg.x, msg.y);
				// �жϵ���������Ƿ���Ч����������
				if (player->go(msg.x, msg.y, player->chessKind))
				{
					if (chess->checkOver())
					{
						DEBUGLOG("Management::onePlayer||player win||chessNum={}", player->chessNum);
						// ������Ϸ���Ҵ�ʱ��ҵ���������С�����а������ֵ
						if (this->chess->gameKind == ONE_PLAYER_GAME && player->chessNum < std::stoi(this->bestScores.back().userScore))
						{
							MyUtils::updateBestScore(this->bestScores, MyUtils::getIBestScoreUser(this->pictureDraw->getInputString(), player->chessNum));
						}
						if (this->isAgainGame())
						{
							this->pictureDraw->drawGraph(CHESSBOARD_MENU);
							player->resetPlayer();
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
				DEBUGLOG("Management::onePlayer||{} select withDraw", player->playerName);
				this->chess->playerWithDraw();
			}
			if (this->pictureDraw->isValidClick(msg.x, msg.y, this->pictureDraw->backwardMenu))
			{
				DEBUGLOG("Management::onePlayer||{} select backward main menu", player->playerName);
				exitGame = true;
				return false;
			}
		}
	}
}
