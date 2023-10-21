#include "Management.h"

Management::Management(Player* player, AI* ai, Chess* chess)
{
	this->player = player;
	this->ai = ai;
	this->chess = chess;

	ai->init(chess);
	player->init(chess);  // ³õÊ¼»¯ÆåÊÖ
	INFOLOG("Management::Management||construct management success");
}

void Management::play()
{
	chess->init();
	while (1)
	{
		player->go();
		if (chess->checkOver())
		{
			chess->init();
			continue;
		}

		ai->go();
		if (chess->checkOver()) 
		{
			chess->init();
			continue;
		}
	}
}
