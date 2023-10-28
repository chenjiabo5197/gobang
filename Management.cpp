#include "Management.h"

Management::Management(Player* player, AI* ai, Chess* chess)
{
	this->player = player;
	this->ai = ai;
	this->chess = chess;

	ai->init(chess);
	player->init(chess);  // ��ʼ������
	INFOLOG("Management::Management||construct management success");
}

void Management::play()
{
	chess->init();
	while (1)
	{
		if (GlobalVar::instance()->get_value("exit_game"))
		{
			INFOLOG("Management::play||player exit game");
			break;
		}
		player->go();
		if (chess->checkOver())
		{
			INFOLOG("Management::play||player win||init chess again");
			chess->init();
			continue;
		}

		ai->go();
		if (chess->checkOver())
		{
			INFOLOG("Management::play||AI win||init chess again");
			chess->init();
			continue;
		}
	}
	INFOLOG("Management::play||game over");
}
