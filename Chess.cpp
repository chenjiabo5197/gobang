#include "Chess.h"
#include <mmsystem.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")

Chess::Chess(int chessBoardWidth, int chessBoardHeight, int chessBoardSize, int marginX, int marginY, float chessSize)
{
	this->chessBoardSize = chessBoardSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;
	this->chessBoardWidth = chessBoardWidth;
	this->chessBoardHeight = chessBoardHeight;

	// ����ͼƬ
	loadimage(&this->chessBoard.pictureFile, "res/����2.jpg");
	this->chessBoard.width = 897;
	this->chessBoard.height = 895;
	this->chessBoard.name = "chessBoard";
	this->chessBoard.isUse = false;
	loadimage(&this->withDraw.pictureFile, "res/withdraw.png");
	this->withDraw.width = 133;
	this->withDraw.height = 46;
	this->withDraw.name = "withDraw";
	this->withDraw.isUse = false;
	loadimage(&this->exitGame.pictureFile, "res/exit_game.png");
	this->exitGame.width = 131;
	this->exitGame.height = 45;
	this->exitGame.name = "exitGame";
	this->exitGame.isUse = false;
	loadimage(&this->againGame.pictureFile, "res/again_game.png");
	this->againGame.width = 132;
	this->againGame.height = 46;
	this->againGame.name = "againGame";
	this->againGame.isUse = false;
	loadimage(&this->onePlayer.pictureFile, "res/one_player.png");
	this->onePlayer.width = 132;
	this->onePlayer.height = 46;
	this->onePlayer.name = "onePlayer";
	this->onePlayer.isUse = false;
	loadimage(&this->playerInternet.pictureFile, "res/player_internet.png");
	this->playerInternet.width = 131;
	this->playerInternet.height = 45;
	this->playerInternet.name = "playerInternet";
	this->playerInternet.isUse = false;
	loadimage(&this->startGame.pictureFile, "res/start_game.png");
	this->startGame.width = 132;
	this->startGame.height = 46;
	this->startGame.name = "startGame";
	this->startGame.isUse = false;
	loadimage(&this->twoPlayers.pictureFile, "res/two_players.png");
	this->twoPlayers.width = 132;
	this->twoPlayers.height = 47;
	this->twoPlayers.name = "twoPlayers";
	this->twoPlayers.isUse = false;
	loadimage(&this->winGame.pictureFile, "res/win.jpg");
	this->winGame.width = 895;
	this->winGame.height = 625;
	this->winGame.name = "winGame";
	this->winGame.isUse = false;
	loadimage(&this->loseGame.pictureFile, "res/lose.jpg");
	this->loseGame.width = 895;
	this->loseGame.height = 624;
	this->loseGame.name = "loseGame";
	this->loseGame.isUse = false;
	loadimage(&this->chessBlack.pictureFile, "res/black.png", chessSize, chessSize, true);
	this->chessBlack.name = "chessBlack";
	this->chessBlack.isUse = false;
	loadimage(&this->chessWhite.pictureFile, "res/white.png", chessSize, chessSize, true);
	this->chessWhite.name = "chessWhite";
	this->chessWhite.isUse = false;

	playerFlag = CHESS_BLACK;

	// ��ʼ�����̣�����ÿ��λ�ö�Ϊ0����ʾ�հ�
	for (int i = 0; i < chessBoardSize; i++)
	{
		std::vector<int> row;
		for (int j = 0; j < chessBoardSize; j++)
		{
			row.push_back(0);
		}
		chessMap.push_back(row);
	}
	INFOLOG("Chess::Chess||construct chess success||chessBoardSize={}||marginX={}||marginY={}||chessSize={}", 
		chessBoardSize, marginX, marginY, chessSize);
}

void Chess::init()
{
	drawGraph(CHESSBOARD_MENU);
	
	mciSendString("play res/start.wav", 0, 0, 0);  //��Ҫ�޸��ַ���Ϊ���ַ���

	for (int i = 0; i < chessMap.size(); i++)
	{
		for (int j = 0; j < chessMap[i].size(); j++)
		{
			chessMap[i][j] = 0;
		}
	}

	playerFlag = true;
	INFOLOG("Chess::init||init chess success||graph.width={}||graph.height={}", 897, 895);
}

/*
* �ȼ�����λ�ø�����4���㣨���̵Ľ��紦�����ӳ���λ�ã���λ�ã�Ȼ���ټ�����λ�õ����ĸ���֮��ľ��룬
�����ĳ����ľ���С�ڡ���ֵ��������Ϊ�����������λ�á��������ֵ���� С�����Ӵ�С��һ�뼴�ɡ���������ȡ����С��0.4��
*/
bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	// ������������ϵ��У�����(����������ȥ���̱�Ե�ٳ�ÿ�������С)
	int row = (x - margin_x) / chessSize;  // ����
	int col = (y - margin_y) / chessSize;  // ����

	// easyx������ԭ�������Ͻǣ�y�����£�x������
	// ���������ķ������Ͻ�����
	int leftTopPosX = margin_x + chessSize * row;
	int leftTopPosY = margin_y + chessSize * col;

	// ���������ķ������Ͻ�����
	int rightTopPosX = margin_x + chessSize * (row + 1);
	int rightTopPosY = margin_y + chessSize * col;

	// ���������ķ������½�����
	int leftBottomPosX = margin_x + chessSize * row;
	int leftBottomPosY = margin_y + chessSize * (col + 1);

	// ���������ķ������½�����
	int rightBottomPosX = margin_x + chessSize * (row + 1);
	int rightBottomPosY = margin_y + chessSize * (col + 1);

	int offset = chessSize * 0.4;  // 20 �������ģ����������

	int len;
	bool selectPos = false;
	chess_coordinate flag_coord = COORD_DEFAULT;

	INFOLOG("Chess::clickBoard||margin_x={}||margin_y={}||row={}||col={}||leftTopPosX={}||leftTopPosY={}||rightTopPosX={}||rightTopPosY={}||leftBottomPosX={}||leftBottomPosY={}||rightBottomPosX={}||rightBottomPosY={}||offset={}",
		margin_x, margin_y, row, col, leftTopPosX, leftTopPosY, rightTopPosX, rightTopPosY, leftBottomPosX, leftBottomPosY, rightBottomPosX, rightBottomPosY, offset);

	do
	{
		// �����Ͻ�λ�õľ���
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset)
		{
			flag_coord = LEFTTOP;
			pos->row = row;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("Chess::clickBoard||leftTop||len={}", len);

		// �����Ͻ�λ�õľ���
		len = sqrt((x - rightTopPosX) * (x - rightTopPosX) + (y - rightTopPosY) * (y - rightTopPosY));
		if (len < offset)
		{
			flag_coord = RIGHTTOP;
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("Chess::clickBoard||rightTop||len={}", len);

		// �����½�λ�õľ���
		len = sqrt((x - leftBottomPosX) * (x - leftBottomPosX) + (y - leftBottomPosY) * (y - leftBottomPosY));
		if (len < offset)
		{
			flag_coord = LEFTBOTTOM;
			pos->row = row;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("Chess::clickBoard||leftBottom||len={}", len);

		// �����½�λ�õľ���
		len = sqrt((x - rightBottomPosX) * (x - rightBottomPosX) + (y - rightBottomPosY) * (y - rightBottomPosY));
		if (len < offset)
		{
			flag_coord = RIGHTBOTTOM;
			pos->row = row + 1;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				selectPos = true;
			}
			break;
		}
		DEBUGLOG("Chess::clickBoard||rightBottom||len={}", len);

	} while (0);
	INFOLOG("Chess::clickBoard||flag_coord={}||len={}||pos.row={}||pos.col={}", (int)flag_coord, len, pos->row, pos->col);

	return selectPos;
}

void Chess::chessDown(ChessPos* pos, chess_kind_type kind)
{
	mciSendString("play res/chess_down.mp3", 0, 0, 0);
	int x = margin_x + pos->row * chessSize - 0.5 * chessSize;
	int y = margin_y + pos->col * chessSize - 0.5 * chessSize;

	if (this->chessBoard.isUse)   // �ж��Ƿ�������ҳ��
	{
		if (kind == CHESS_WHITE)
		{
			putImagePNG(x, y, &this->chessWhite.pictureFile);
			DEBUGLOG("Chess::chessDown||CHESS_WHITE||x={}||y={}", x, y);
		}
		else
		{
			putImagePNG(x, y, &this->chessBlack.pictureFile);
			DEBUGLOG("Chess::chessDown||CHESS_BLACK||x={}||y={}", x, y);
		}
		// ������������
		updateChessMap(pos);
	}
}

int Chess::getChessBoardSize()
{
	return chessBoardSize;
}

int Chess::getChessData(ChessPos* pos)
{
	return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
	return chessMap[row][col];
}

bool Chess::checkOver()
{
	if (checkWin())
	{
		Sleep(1500);
		if (!playerFlag)   // ����Ӯ�����Ӯ
		{
			INFOLOG("Chess::checkOver||black win");
			mciSendString("play res/clap.mp3", 0, 0, 0);
			drawGraph(WIN_MENU);
			while (1)
			{
				MOUSEMSG msg = GetMouseMsg();
				if (msg.uMsg == WM_LBUTTONDOWN)
				{
					DEBUGLOG("Chess::checkOver||mouse click||x={}||y={}", msg.x, msg.y);
					if (this->isValidClick(msg.x, msg.y, this->exitGame))
					{

						GlobalVar::instance()->set_value("exit_game", true);
						return false;
					}
					if (this->isValidClick(msg.x, msg.y, this->againGame))
					{
						return true;
					}
				}
			}
		}
		else
		{
			INFOLOG("Chess::checkOver||white win");
			mciSendString("play res/ʧ��.mp3", 0, 0, 0);
			drawGraph(LOSE_MENU);
			while (1)
			{
				MOUSEMSG msg = GetMouseMsg();
				if (msg.uMsg == WM_LBUTTONDOWN)
				{
					DEBUGLOG("Chess::checkOver||mouse click||x={}||y={}", msg.x, msg.y);
					if (this->isValidClick(msg.x, msg.y, this->exitGame))
					{

						GlobalVar::instance()->set_value("exit_game", true);
						return false;
					}
					if (this->isValidClick(msg.x, msg.y, this->againGame))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void Chess::putImagePNG(int x, int y, IMAGE* picture)
{
	// ������ʼ��
	DWORD* dst = GetImageBuffer();   // GetImageBuffer()�������ڻ�ȡ��ͼ�豸���Դ�ָ�룬easyx����
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture);  // ��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth();  // ��ȡpicture��ȣ�easyx����
	int picture_height = picture->getheight();  // ��ȡpicture�߶ȣ�easyx����
	int graphWidth = getwidth();   // ��ȡ��ͼ���Ŀ�ȣ�easyx����
	int graphHeight = getheight();   // ��ȡ��ͼ���Ŀ�ȣ�easyx����
	int dstX = 0;   // ���Դ������ص�����

	// ʵ��͸����ͼ  ��ʽ Cp=��p*FP+(1-��p)*BP   ��Ҷ˹������е���ɫ�ĸ��ʼ���
	for (int ix = 0; ix < picture_height; ix++)
	{
		for (int iy = 0; iy < picture_width; iy++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}

bool Chess::checkWin()
{
	// ������б��б�ĸ�����ÿ����������ݵ�ǰ����������5�����ӣ���һ�ַ��Ͼ���Ӯ
	int row = lastPos.row;
	int col = lastPos.col;

	// ˮƽ����, ������ҷֱ�ƥ��4����
	for (int i = 0; i < 5; i++)
	{
		if (col - i >= 0 && col - i + 4 < chessBoardSize && chessMap[row][col - i] == chessMap[row][col - i + 1] && chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] && chessMap[row][col - i] == chessMap[row][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate horizontal direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	// ��ֱ���� 
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 && row - i + 4 < chessBoardSize && chessMap[row - i][col] == chessMap[row - i + 1][col] && chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] && chessMap[row - i][col] == chessMap[row - i + 4][col])
		{
			INFOLOG("Chess::checkWin||calculate vertical direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	// ��б����"/"
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 && col - i >= 0 && row - i + 4 < chessBoardSize && col - i + 4 < chessBoardSize && chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] && chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] && chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate left oblique direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	// ��б���� "\"
	for (int i = 0; i < 5; i++)
	{
		if (row + i < chessBoardSize && row + i - 4 >= 0 && col - i >= 0 && col - i + 4 < chessBoardSize && chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] && chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] && chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
		{
			INFOLOG("Chess::checkWin||calculate right oblique direction success||i={}||playFlag={}", i, playerFlag);
			return true;
		}
	}

	return false;

}

int Chess::getChessBoardWidth()
{
	return chessBoardWidth;
}

int Chess::getChessBoardHieght()
{
	return chessBoardHeight;
}

bool Chess::isValidClick(int x, int y, LoadPicture picture)
{
	if (x >= picture.x && x <= picture.x + picture.width && y >= picture.y && y <= picture.y + picture.height && picture.isUse)
	{
		mciSendString("play res/select.wav", 0, 0, 0);
		INFOLOG("Chess::isValidClick||valid click||picture name={}||x={}||y={}||leftX={}||rightX={}||smallY={}||bigY={}", picture.name, x, y, picture.x,
			picture.x + picture.width, picture.y, picture.y + picture.height);
		return true;
	}
	return false;
}

void Chess::playerWithDraw()
{
	// ɾ������ͼƬ

	// ������������
}

void Chess::updateChessMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? 1 : -1;  // ���浱ǰλ���Ǻ��廹�ǰ���
	playerFlag = !playerFlag;   // �������巽
	DEBUGLOG("Chess::updateChessMap||playerFlag={}||pos->row={}||pos->col={}", !playerFlag, pos->row, pos->col);
}

// ��switch...case...�ṹ�в�����case�ж����±���,���ǽ������±�����case�ÿ�{}��ס������ѡ������±�����switch֮ǰ
void Chess::drawGraph(menu_kind_type kind)
{
	clearLastGraph();
	switch (kind)
	{
	case MAIN_MENU:
		break;
	case PLAYER_NUM_MENU:
		break;
	case CHESSBOARD_MENU:
	{  
		// ȡ������ȵ����ֵ+���̿�� +1��Ϊ����һ������
		int graphWidth = max(this->withDraw.width, this->exitGame.width) + this->chessBoard.width + 1;
		int graphHeight = this->chessBoard.height;
		initgraph(graphWidth, graphHeight);
		// ���ñ���ɫΪ��ɫ
		setbkcolor(WHITE);
		cleardevice();  // ʹ�õ�ǰ����ɫ��ջ�ͼ�豸

		// ͼƬ�����Ͻ����꣬���ڶ�λ
		this->chessBoard.x = 0;
		this->chessBoard.y = 0;
		this->chessBoard.isUse = true;
		this->withDraw.x = this->chessBoard.width + 1;
		this->withDraw.y = 200;
		this->withDraw.isUse = true;
		this->exitGame.x = this->chessBoard.width + 1;
		this->exitGame.y = 500;
		this->exitGame.isUse = true;

		putimage(this->chessBoard.x, this->chessBoard.y, &this->chessBoard.pictureFile);
		putimage(this->withDraw.x, this->withDraw.y, &this->withDraw.pictureFile);
		putimage(this->exitGame.x, this->exitGame.y, &this->exitGame.pictureFile);
		
		INFOLOG("Chess::drawGraph||kind=CHESSBOARD_MENU||chessBoard.x={}||chessBoard.y={}||withDraw.x={}||withDraw.y={}||exitGame.x={}||exitGame.y={}", 
			chessBoard.x, chessBoard.y, withDraw.x, withDraw.y, exitGame.x, exitGame.y);
		break;
	}
	case WIN_MENU:
	{
		// ȡ������ȵ����ֵ+ͼƬ��� +1��Ϊ����һ������
		int graphWidth = max(this->againGame.width, this->exitGame.width) + this->winGame.width + 1;
		int graphHeight = this->winGame.height;
		initgraph(graphWidth, graphHeight);
		// ���ñ���ɫΪ��ɫ
		setbkcolor(WHITE);
		cleardevice();  // ʹ�õ�ǰ����ɫ��ջ�ͼ�豸

		// ͼƬ�����Ͻ����꣬���ڶ�λ
		this->winGame.x = 0;
		this->winGame.y = 0;
		this->winGame.isUse = true;
		this->againGame.x = this->winGame.width + 1;
		this->againGame.y = 150;
		this->againGame.isUse = true;
		this->exitGame.x = this->winGame.width + 1;
		this->exitGame.y = 350;
		this->exitGame.isUse = true;

		putimage(this->winGame.x, this->winGame.y, &this->winGame.pictureFile);
		putimage(this->againGame.x, this->againGame.y, &this->againGame.pictureFile);
		putimage(this->exitGame.x, this->exitGame.y, &this->exitGame.pictureFile);

		INFOLOG("Chess::drawGraph||kind=WIN_MENU||winGame.x={}||winGame.y={}||againGame.x={}||againGame.y={}||exitGame.x={}||exitGame.y={}",
			winGame.x, winGame.y, againGame.x, againGame.y, exitGame.x, exitGame.y);
		break;
	}
	case LOSE_MENU:
	{
		// ȡ������ȵ����ֵ+ͼƬ��� +1��Ϊ����һ������
		int graphWidth = max(this->againGame.width, this->exitGame.width) + this->loseGame.width + 1;
		int graphHeight = this->loseGame.height;
		initgraph(graphWidth, graphHeight);
		// ���ñ���ɫΪ��ɫ
		setbkcolor(WHITE);
		cleardevice();  // ʹ�õ�ǰ����ɫ��ջ�ͼ�豸

		// ͼƬ�����Ͻ����꣬���ڶ�λ
		this->loseGame.x = 0;
		this->loseGame.y = 0;
		this->loseGame.isUse = true;
		this->againGame.x = this->loseGame.width + 1;
		this->againGame.y = 150;
		this->againGame.isUse = true;
		this->exitGame.x = this->loseGame.width + 1;
		this->exitGame.y = 350;
		this->exitGame.isUse = true;

		putimage(this->loseGame.x, this->loseGame.y, &this->loseGame.pictureFile);
		putimage(this->againGame.x, this->againGame.y, &this->againGame.pictureFile);
		putimage(this->exitGame.x, this->exitGame.y, &this->exitGame.pictureFile);

		INFOLOG("Chess::drawGraph||kind=LOSE_MENU||loseGame.x={}||loseGame.y={}||againGame.x={}||againGame.y={}||exitGame.x={}||exitGame.y={}",
			loseGame.x, loseGame.y, againGame.x, againGame.y, exitGame.x, exitGame.y);
		break;
	}
	default:
		ERRORLOG("Chess::drawGraph||undefined menu_kind_type||kind={}", (int)kind);
		break;
	}
}

void Chess::clearLastGraph()
{
	closegraph();
	this->chessBoard.isUse = false;
	this->withDraw.isUse = false;
	this->exitGame.isUse = false;
	this->againGame.isUse = false;
	this->onePlayer.isUse = false;
	this->playerInternet.isUse = false;
	this->startGame.isUse = false;
	this->twoPlayers.isUse = false;
	this->winGame.isUse = false;
	this->loseGame.isUse = false;
	this->chessBlack.isUse = false;
	this->chessWhite.isUse = false;
}
