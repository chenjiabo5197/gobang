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
	Management(Player*, AI*, Chess*, PictureDraw*);
	void play();

private:
	Player* player;
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
};


