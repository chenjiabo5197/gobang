#include <iostream>
#include "Management.h"

int main()
{
	Chess chess(13, 44, 43, 67.4);
	Player player;
	AI ai;
	Management game(&player, &ai, &chess);

	game.play();

	return 0;
}

