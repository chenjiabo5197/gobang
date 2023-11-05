#include "pictureDraw.h"

PictureDraw::PictureDraw(int chessBoardSize, int marginX, int marginY, float chessSize)
{
	// 加载图片
	loadimage(&this->chessBoardPicture.pictureFile, "res/棋盘2.jpg");
	this->chessBoardPicture.width = 897;
	this->chessBoardPicture.height = 895;
	this->chessBoardPicture.name = "chessBoard";
	this->chessBoardPicture.isUse = false;
	loadimage(&this->withDrawPicture.pictureFile, "res/withdraw.png");
	this->withDrawPicture.width = 133;
	this->withDrawPicture.height = 46;
	this->withDrawPicture.name = "withDraw";
	this->withDrawPicture.isUse = false;
	this->clickSound.push_back("withDraw");
	loadimage(&this->exitGamePicture.pictureFile, "res/exit_game.png");
	this->exitGamePicture.width = 131;
	this->exitGamePicture.height = 45;
	this->exitGamePicture.name = "exitGame";
	this->exitGamePicture.isUse = false;
	loadimage(&this->againGamePicture.pictureFile, "res/again_game.png");
	this->againGamePicture.width = 132;
	this->againGamePicture.height = 46;
	this->againGamePicture.name = "againGame";
	this->againGamePicture.isUse = false;
	this->clickSound.push_back("againGame");
	loadimage(&this->onePlayerPicture.pictureFile, "res/one_player.png");
	this->onePlayerPicture.width = 132;
	this->onePlayerPicture.height = 46;
	this->onePlayerPicture.name = "onePlayer";
	this->onePlayerPicture.isUse = false;
	this->clickSound.push_back("onePlayer");
	loadimage(&this->playerInternetPicture.pictureFile, "res/player_internet.png");
	this->playerInternetPicture.width = 131;
	this->playerInternetPicture.height = 45;
	this->playerInternetPicture.name = "playerInternet";
	this->playerInternetPicture.isUse = false;
	this->clickSound.push_back("playerInternet");
	loadimage(&this->startGamePicture.pictureFile, "res/start_game.png");
	this->startGamePicture.width = 132;
	this->startGamePicture.height = 46;
	this->startGamePicture.name = "startGame";
	this->startGamePicture.isUse = false;
	this->clickSound.push_back("startGame");
	loadimage(&this->twoPlayersPicture.pictureFile, "res/two_players.png");
	this->twoPlayersPicture.width = 132;
	this->twoPlayersPicture.height = 47;
	this->twoPlayersPicture.name = "twoPlayers";
	this->twoPlayersPicture.isUse = false;
	this->clickSound.push_back("twoPlayers");
	loadimage(&this->winGamePicture.pictureFile, "res/win.jpg");
	this->winGamePicture.width = 895;
	this->winGamePicture.height = 625;
	this->winGamePicture.name = "winGame";
	this->winGamePicture.isUse = false;
	loadimage(&this->loseGamePicture.pictureFile, "res/lose.jpg");
	this->loseGamePicture.width = 895;
	this->loseGamePicture.height = 624;
	this->loseGamePicture.name = "loseGame";
	this->loseGamePicture.isUse = false;
	loadimage(&this->drawGamePicture.pictureFile, "res/draw.jpg");
	this->drawGamePicture.width = 961;
	this->drawGamePicture.height = 670;
	this->drawGamePicture.name = "drawGame";
	this->drawGamePicture.isUse = false;
	loadimage(&this->bestScoresPicture.pictureFile, "res/best_scores.png");
	this->bestScoresPicture.width = 131;
	this->bestScoresPicture.height = 45;
	this->bestScoresPicture.name = "bestScores";
	this->bestScoresPicture.isUse = false;
	this->clickSound.push_back("bestScores");
	loadimage(&this->backwardMenu.pictureFile, "res/backward_menu.png");
	this->backwardMenu.width = 165;
	this->backwardMenu.height = 45;
	this->backwardMenu.name = "backwardMenu";
	this->backwardMenu.isUse = false;
	this->clickSound.push_back("backwardMenu");
	loadimage(&this->chessBlackPicture.pictureFile, "res/black.png", chessSize, chessSize, true);
	this->chessBlackPicture.name = "chessBlack";
	this->chessBlackPicture.isUse = false;
	loadimage(&this->chessWhitePicture.pictureFile, "res/white.png", chessSize, chessSize, true);
	this->chessWhitePicture.name = "chessWhite";
	this->chessWhitePicture.isUse = false;
	loadimage(&this->curBlackPicture.pictureFile, "res/black2.png", chessSize, chessSize, true);
	this->curBlackPicture.name = "curBlack";
	this->curBlackPicture.isUse = false;
	loadimage(&this->curWhitePicture.pictureFile, "res/white2.png", chessSize, chessSize, true);
	this->curWhitePicture.name = "curWhite";
	this->curWhitePicture.isUse = false;
	loadimage(&this->backgroudPicture.pictureFile, "res/menu.png");
	this->backgroudPicture.width = 897;
	this->backgroudPicture.height = 895;
	this->backgroudPicture.name = "backgroud";
	this->backgroudPicture.isUse = false;
	INFOLOG("PictureDraw::PictureDraw||load picture success!");
}

bool PictureDraw::isValidClick(int x, int y, LoadPicture picture)
{
	if (x >= picture.x && x <= picture.x + picture.width && y >= picture.y && y <= picture.y + picture.height && picture.isUse)
	{
		auto it = std::find(this->clickSound.begin(), this->clickSound.end(), picture.name);
		if (it != this->clickSound.end())   // 在列表中的图片点击需要播放声音
		{
			mciSendString("play res/select.wav", 0, 0, 0);
		}
		INFOLOG("Chess::isValidClick||valid click||picture name={}||x={}||y={}||leftX={}||rightX={}||smallY={}||bigY={}", picture.name, x, y, picture.x,
			picture.x + picture.width, picture.y, picture.y + picture.height);
		return true;
	}
	return false;
}

// 在switch...case...结构中不能在case中定义新变量,除非将定义新变量的case用块{}包住，或者选择将你的新变量在switch之前
void PictureDraw::drawGraph(menu_kind_type kind)
{
	clearLastGraph();
	switch (kind)
	{
	case MAIN_MENU:   // 主菜单，选择开始游戏，退出游戏
	{
		// 取背景图片宽度和高度
		int graphWidth = this->backgroudPicture.width;
		int graphHeight = this->backgroudPicture.height;
		initgraph(graphWidth, graphHeight);
		// 设置背景色为白色
		setbkcolor(WHITE);
		cleardevice();  // 使用当前背景色清空绘图设备

		// 图片的左上角坐标，用于定位
		this->backgroudPicture.x = 0;
		this->backgroudPicture.y = 0;
		this->backgroudPicture.isUse = true;
		this->startGamePicture.x = 380;
		this->startGamePicture.y = 200;
		this->startGamePicture.isUse = true;
		this->bestScoresPicture.x = 380;
		this->bestScoresPicture.y = 400;
		this->bestScoresPicture.isUse = true;
		this->exitGamePicture.x = 380;
		this->exitGamePicture.y = 600;
		this->exitGamePicture.isUse = true;

		putimage(this->backgroudPicture.x, this->backgroudPicture.y, &this->backgroudPicture.pictureFile);
		putimage(this->bestScoresPicture.x, this->bestScoresPicture.y, &this->bestScoresPicture.pictureFile);
		putimage(this->startGamePicture.x, this->startGamePicture.y, &this->startGamePicture.pictureFile);
		putimage(this->exitGamePicture.x, this->exitGamePicture.y, &this->exitGamePicture.pictureFile);

		INFOLOG("PictureDraw::drawGraph||kind=MAIN_MENU||backgroudPicture.x={}||backgroudPicture.y={}||startGamePicture.x={}||startGamePicture.y={}||bestScoresPicture.x={}||bestScoresPicture.y={}||exitGamePicture.x={}||exitGamePicture.y={}",
			backgroudPicture.x, backgroudPicture.y, startGamePicture.x, startGamePicture.y, bestScoresPicture.x, bestScoresPicture.y, exitGamePicture.x, exitGamePicture.y);
		break;
	}
	case PLAYER_NUM_MENU:   // 选择游戏人数菜单，单人游戏（和ai对战），双人对战，网络对战，返回主菜单
	{
		// 取背景图片宽度和高度
		int graphWidth = this->backgroudPicture.width;
		int graphHeight = this->backgroudPicture.height;
		initgraph(graphWidth, graphHeight);
		// 设置背景色为白色
		setbkcolor(WHITE);
		cleardevice();  // 使用当前背景色清空绘图设备

		// 图片的左上角坐标，用于定位
		this->backgroudPicture.x = 0;
		this->backgroudPicture.y = 0;
		this->backgroudPicture.isUse = true;
		this->onePlayerPicture.x = 380;
		this->onePlayerPicture.y = 200;
		this->onePlayerPicture.isUse = true;
		this->twoPlayersPicture.x = 380;
		this->twoPlayersPicture.y = 350;
		this->twoPlayersPicture.isUse = true;
		this->playerInternetPicture.x = 380;
		this->playerInternetPicture.y = 500;
		this->playerInternetPicture.isUse = true;
		this->backwardMenu.x = 368;
		this->backwardMenu.y = 650;
		this->backwardMenu.isUse = true;

		putimage(this->backgroudPicture.x, this->backgroudPicture.y, &this->backgroudPicture.pictureFile);
		putimage(this->onePlayerPicture.x, this->onePlayerPicture.y, &this->onePlayerPicture.pictureFile);
		putimage(this->twoPlayersPicture.x, this->twoPlayersPicture.y, &this->twoPlayersPicture.pictureFile);
		putimage(this->playerInternetPicture.x, this->playerInternetPicture.y, &this->playerInternetPicture.pictureFile);
		putimage(this->backwardMenu.x, this->backwardMenu.y, &this->backwardMenu.pictureFile);

		INFOLOG("PictureDraw::drawGraph||kind=PLAYER_NUM_MENU||backgroudPicture.x={}||backgroudPicture.y={}||onePlayerPicture.x={}||onePlayerPicture.y={}||twoPlayersPicture.x={}||twoPlayersPicture.y={}||playerInternetPicture.x={}||playerInternetPicture.y={}||backwardMenu.x={}||backwardMenu.y={}",
			backgroudPicture.x, backgroudPicture.y, onePlayerPicture.x, onePlayerPicture.y, twoPlayersPicture.x, twoPlayersPicture.y, playerInternetPicture.x, playerInternetPicture.y, backwardMenu.x, backwardMenu.y);
		break;
	}
	case CHESSBOARD_MENU:   // 单人游戏（和ai对战）
	{
		// 取按键宽度的最大值+棋盘宽度 +1是为了留一点余量
		int graphWidth = max(this->withDrawPicture.width, this->backwardMenu.width) + this->chessBoardPicture.width + 1;
		int graphHeight = this->chessBoardPicture.height;
		initgraph(graphWidth, graphHeight);
		// 设置背景色为白色
		setbkcolor(WHITE);
		cleardevice();  // 使用当前背景色清空绘图设备

		// 图片的左上角坐标，用于定位
		this->chessBoardPicture.x = 0;
		this->chessBoardPicture.y = 0;
		this->chessBoardPicture.isUse = true;
		this->withDrawPicture.x = this->chessBoardPicture.width + 1;
		this->withDrawPicture.y = 200;
		this->withDrawPicture.isUse = true;
		this->backwardMenu.x = this->chessBoardPicture.width + 1;
		this->backwardMenu.y = 500;
		this->backwardMenu.isUse = true;

		putimage(this->chessBoardPicture.x, this->chessBoardPicture.y, &this->chessBoardPicture.pictureFile);
		putimage(this->withDrawPicture.x, this->withDrawPicture.y, &this->withDrawPicture.pictureFile);
		putimage(this->backwardMenu.x, this->backwardMenu.y, &this->backwardMenu.pictureFile);

		INFOLOG("PictureDraw::drawGraph||kind=CHESSBOARD_MENU||chessBoardPicture.x={}||chessBoardPicture.y={}||withDrawPicture.x={}||withDrawPicture.y={}||backwardMenu.x={}||backwardMenu.y={}",
			chessBoardPicture.x, chessBoardPicture.y, withDrawPicture.x, withDrawPicture.y, backwardMenu.x, backwardMenu.y);
		break;
	}
	case WIN_MENU:
	{
		// 取按键宽度的最大值+图片宽度 +1是为了留一点余量
		int graphWidth = max(this->againGamePicture.width, this->backwardMenu.width) + this->winGamePicture.width + 1;
		int graphHeight = this->winGamePicture.height;
		initgraph(graphWidth, graphHeight);
		// 设置背景色为白色
		setbkcolor(WHITE);
		cleardevice();  // 使用当前背景色清空绘图设备

		// 图片的左上角坐标，用于定位
		this->winGamePicture.x = 0;
		this->winGamePicture.y = 0;
		this->winGamePicture.isUse = true;
		this->againGamePicture.x = this->winGamePicture.width + 1;
		this->againGamePicture.y = 150;
		this->againGamePicture.isUse = true;
		this->backwardMenu.x = this->winGamePicture.width + 1;
		this->backwardMenu.y = 350;
		this->backwardMenu.isUse = true;

		putimage(this->winGamePicture.x, this->winGamePicture.y, &this->winGamePicture.pictureFile);
		putimage(this->againGamePicture.x, this->againGamePicture.y, &this->againGamePicture.pictureFile);
		putimage(this->backwardMenu.x, this->backwardMenu.y, &this->backwardMenu.pictureFile);

		INFOLOG("PictureDraw::drawGraph||kind=WIN_MENU||winGamePicture.x={}||winGamePicture.y={}||againGamePicture.x={}||againGamePicture.y={}||backwardMenu.x={}||backwardMenu.y={}",
			winGamePicture.x, winGamePicture.y, againGamePicture.x, againGamePicture.y, backwardMenu.x, backwardMenu.y);
		break;
	}
	case LOSE_MENU:
	{
		// 取按键宽度的最大值+图片宽度 +1是为了留一点余量
		int graphWidth = max(this->againGamePicture.width, this->backwardMenu.width) + this->loseGamePicture.width + 1;
		int graphHeight = this->loseGamePicture.height;
		initgraph(graphWidth, graphHeight);
		// 设置背景色为白色
		setbkcolor(WHITE);
		cleardevice();  // 使用当前背景色清空绘图设备

		// 图片的左上角坐标，用于定位
		this->loseGamePicture.x = 0;
		this->loseGamePicture.y = 0;
		this->loseGamePicture.isUse = true;
		this->againGamePicture.x = this->loseGamePicture.width + 1;
		this->againGamePicture.y = 150;
		this->againGamePicture.isUse = true;
		this->backwardMenu.x = this->loseGamePicture.width + 1;
		this->backwardMenu.y = 350;
		this->backwardMenu.isUse = true;

		putimage(this->loseGamePicture.x, this->loseGamePicture.y, &this->loseGamePicture.pictureFile);
		putimage(this->againGamePicture.x, this->againGamePicture.y, &this->againGamePicture.pictureFile);
		putimage(this->backwardMenu.x, this->backwardMenu.y, &this->backwardMenu.pictureFile);

		INFOLOG("PictureDraw::drawGraph||kind=LOSE_MENU||loseGamePicture.x={}||loseGamePicture.y={}||againGamePicture.x={}||againGamePicture.y={}||backwardMenu.x={}||backwardMenu.y={}",
			loseGamePicture.x, loseGamePicture.y, againGamePicture.x, againGamePicture.y, backwardMenu.x, backwardMenu.y);
		break;
	}
	case DRAW_MENU:
	{
		// 取按键宽度的最大值+图片宽度 +1是为了留一点余量
		int graphWidth = max(this->againGamePicture.width, this->backwardMenu.width) + this->drawGamePicture.width + 1;
		int graphHeight = this->drawGamePicture.height;
		initgraph(graphWidth, graphHeight);
		// 设置背景色为白色
		setbkcolor(WHITE);
		cleardevice();  // 使用当前背景色清空绘图设备

		// 图片的左上角坐标，用于定位
		this->drawGamePicture.x = 0;
		this->drawGamePicture.y = 0;
		this->drawGamePicture.isUse = true;
		this->againGamePicture.x = this->drawGamePicture.width + 1;
		this->againGamePicture.y = 150;
		this->againGamePicture.isUse = true;
		this->backwardMenu.x = this->drawGamePicture.width + 1;
		this->backwardMenu.y = 350;
		this->backwardMenu.isUse = true;

		putimage(this->drawGamePicture.x, this->drawGamePicture.y, &this->drawGamePicture.pictureFile);
		putimage(this->againGamePicture.x, this->againGamePicture.y, &this->againGamePicture.pictureFile);
		putimage(this->backwardMenu.x, this->backwardMenu.y, &this->backwardMenu.pictureFile);

		INFOLOG("PictureDraw::drawGraph||kind=LOSE_MENU||drawGamePicture.x={}||drawGamePicture.y={}||againGamePicture.x={}||againGamePicture.y={}||backwardMenu.x={}||backwardMenu.y={}",
			drawGamePicture.x, drawGamePicture.y, againGamePicture.x, againGamePicture.y, backwardMenu.x, backwardMenu.y);
		break;
	}
	default:
		ERRORLOG("PictureDraw::drawGraph||undefined menu_kind_type||kind={}", (int)kind);
		break;
	}
}

void PictureDraw::clearLastGraph()
{
	closegraph();
	this->chessBoardPicture.isUse = false;
	this->withDrawPicture.isUse = false;
	this->exitGamePicture.isUse = false;
	this->againGamePicture.isUse = false;
	this->onePlayerPicture.isUse = false;
	this->playerInternetPicture.isUse = false;
	this->startGamePicture.isUse = false;
	this->twoPlayersPicture.isUse = false;
	this->winGamePicture.isUse = false;
	this->loseGamePicture.isUse = false;
	this->chessBlackPicture.isUse = false;
	this->chessWhitePicture.isUse = false;
	this->bestScoresPicture.isUse = false;
	this->backwardMenu.isUse = false;
}
