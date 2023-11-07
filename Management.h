#pragma once
#include "AI.h"
#include "chess.h"
#include "player.h"
#include "logger.h"
#include "pictureDraw.h"
#include "globalVar.hpp"

class Management
{
public:
	Management(Player*, Player*, AI*, Chess*, PictureDraw*);
	void play();

private:
	Player* player1;
	Player* player2;
	AI* ai;
	Chess* chess;
	PictureDraw* pictureDraw;

	// ѡ����Ϸ��ʽ
	void chooseGame();

	// ������Ϸ
	void onePlayerGame();

	// ������Ϸ��ʼ������AI��player��chess
	void onePlayerInit();

	// ��Ϸ����������ȫ�ֱ�������չʾ�ĳɹ�����ʧ��ҳ�棬����������һ�ֻ����˳������˵�
	bool isAgainGame();

	// ˫�˶�ս
	void twoPlayersGame();

	// ˫�˶�ս��ʼ������player1, player2, chess
	void twoPlayersInit();

	// ����ѡ�ֵ����庯������������
	bool onePlayer(Player*);
};


