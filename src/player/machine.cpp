#include "machine.h"

Machine::Machine(Chessboard* chessboard)
{
    m_chessboard = chessboard;
    this->init();
    INFOLOG("Machine construct success");
}

Machine::~Machine()
{
    INFOLOG("~Machine success");
}

void Machine::init()
{
	m_chess_num = 0;
	int size = m_chessboard->getChessBoardSize();
	for (int i = 0; i < size; i++)
	{
		std::vector<int> row;
		// 方棋盘，遍历可以都使用size
		for (int j = 0; j < size; j++)
		{
			row.push_back(0);
		}
		m_score_map.push_back(row);
	}
	INFOLOG("init||init Machine success||size={}", size);
}

void Machine::addChessNum()
{
    INFOLOG("addChessNum||chess_num={}", ++m_chess_num);
}

int Machine::getChessNum()
{
	return m_chess_num;
}

void Machine::resetMachine()
{
	m_chess_num = 0;
	INFOLOG("resetMachine||reset machine success");
}

// int go(void* data)
// {
// 	Machine* machine = (Machine *)(data);
	// std::vector<std::vector<int>> chessboard_data;
	// int index = 0;
	// for (int i = 0; i < 15; i++)
	// {
	// 	std::vector<int> row;
	// 	// 方棋盘，遍历可以都使用size
	// 	for (int j = 0; j < 15; j++)
	// 	{
	// 		row.push_back(*(array+index++));
	// 	}
	// 	chessboard_data.push_back(row);
	// }
	// delete[] data;

	// 	// 打印计算的分数
	// for (int i = 0; i < machine->chessboard_data.size(); i++)
	// {
	// 	std::vector<int> temp = machine->chessboard_data[i];
	// 	std::stringstream ss;
	// 	std::string str;
	// 	std::copy(temp.begin(), temp.end(), std::ostream_iterator<int>(ss, ", "));
	// 	str = ss.str();
	// 	INFOLOG("Machine::calculateScore||row={}||value={}", i, str);
	// }
	// ChessPos pos = think();
    // 程序休眠1s，假装在思考
    // sleepSeconds(1);
	// mciSendString("play res/g_chess_down_sound.mp3", 0, 0, 0);
	// m_chessboard->chessDown(pos, CHESS_WHITE);
// 	DEBUGLOG("Machine::go||Machine chess down success");
// 	return 0;
// }

void Machine::calculateScore()
{
	int playerChessNum = 0; //棋手方（黑棋）多少个连续的棋子
	int AIChessNum = 0; //AI方（白棋）连续有多少个连续的棋子
	int emptyNum = 0; // 该方向上空白位的个数

	// 评分向量数组清零
	for (int i = 0; i < m_score_map.size(); i++) {
		for (int j = 0; j < m_score_map[i].size(); j++) {
			m_score_map[i][j] = 0;
		}
	}

	int size = m_chessboard->getChessBoardSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			//对每个点进行计算
			if (m_chessboard->getChessData(row, col)) continue;

			for (int y = -1; y <= 0; y++) {        //Y的范围还是-1， 0
				for (int x = -1; x <= 1; x++) {    //X的范围是 -1,0,1
					if (y == 0 && x == 0) continue;
					if (y == 0 && x != 1) continue; //当y=0时，仅允许x=1

					playerChessNum = 0;
					AIChessNum = 0;
					emptyNum = 0;

					// 假设黑棋在该位置落子，会构成什么棋型
					for (int i = 1; i <= 4; i++) {
						int curRow = row + i * y;
						int curCol = col + i * x;
						/*DEBUGLOG("Machine::calculateScore||start calculate black chess score||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
							curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chessboard->getChessData(curRow, curCol) == 2) {
							playerChessNum++;
							/*DEBUGLOG("Machine::calculateScore||calculate black chess score||chessData(curRow, curCol)=1||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
								curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chessboard->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							/*DEBUGLOG("Machine::calculateScore||calculate black chess score||chessData(curRow, curCol)=0||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
								curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/
							break;
						}
						else {
							break;
						}
					}

					// 反向继续计算
					for (int i = 1; i <= 4; i++) {
						int curRow = row - i * y;
						int curCol = col - i * x;
						/*DEBUGLOG("Machine::calculateScore||start calculate black chess score||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
							curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chessboard->getChessData(curRow, curCol) == 2) {
							playerChessNum++;
							/*DEBUGLOG("Machine::calculateScore||calculate black chess score||chessData(curRow, curCol)=1||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
								curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chessboard->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							/*DEBUGLOG("Machine::calculateScore||calculate black chess score||chessData(curRow, curCol)=0||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
								curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/
							break;
						}
						else {
							break;
						}
					}

					if (playerChessNum == 1) { //连2
						//CSDN  程序员Rock
						m_score_map[row][col] += 10;
						/*DEBUGLOG("Machine::calculateScore||calculate black chess score||playerChessNum=1||row={}||col={}||m_score_map[row][col]={}",
							row, col, m_score_map[row][col]);*/
					}
					else if (playerChessNum == 2) {
						if (emptyNum == 1) {
							m_score_map[row][col] += 30;
							/*DEBUGLOG("Machine::calculateScore||calculate black chess score||playerChessNum=2||emptyNum=1||row={}||col={}||m_score_map[row][col]={}",
								row, col, m_score_map[row][col]);*/
						}
						else if (emptyNum == 2) {
							m_score_map[row][col] += 40;
							/*DEBUGLOG("Machine::calculateScore||calculate black chess score||playerChessNum=2||emptyNum=2||row={}||col={}||m_score_map[row][col]={}",
								row, col, m_score_map[row][col]);*/
						}
					}
					else if (playerChessNum == 3) {
						if (emptyNum == 1) {
							m_score_map[row][col] = 60;
							/*DEBUGLOG("Machine::calculateScore||calculate black chess score||playerChessNum=3||emptyNum=1||row={}||col={}||m_score_map[row][col]={}",
								row, col, m_score_map[row][col]);*/
						}
						else if (emptyNum == 2) {
							m_score_map[row][col] = 5000; //200
							/*DEBUGLOG("Machine::calculateScore||calculate black chess score||playerChessNum=3||emptyNum=2||row={}||col={}||m_score_map[row][col]={}",
								row, col, m_score_map[row][col]);*/
						}
					}
					else if (playerChessNum == 4) {
						m_score_map[row][col] = 20000;
						/*DEBUGLOG("Machine::calculateScore||calculate black chess score||playerChessNum=4||row={}||col={}||m_score_map[row][col]={}",
							row, col, m_score_map[row][col]);*/
					}

					// 假设白棋在该位置落子，会构成什么棋型
					emptyNum = 0;

					for (int i = 1; i <= 4; i++) {
						int curRow = row + i * y;
						int curCol = col + i * x;
						/*DEBUGLOG("Machine::calculateScore||start calculate white chess score||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
							curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chessboard->getChessData(curRow, curCol) == 1) {
							AIChessNum++;
							/*DEBUGLOG("Machine::calculateScore||calculate white chess score||chessData(curRow, curCol)=-1||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
								curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chessboard->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							/*DEBUGLOG("Machine::calculateScore||calculate white chess score||chessData(curRow, curCol)=0||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
								curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/
							break;
						}
						else {
							break;
						}
					}

					for (int i = 1; i <= 4; i++) {
						int curRow = row - i * y;
						int curCol = col - i * x;
						/*DEBUGLOG("Machine::calculateScore||start calculate white chess score||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
							curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/

						if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chessboard->getChessData(curRow, curCol) == 1) {
							AIChessNum++;
							/*DEBUGLOG("Machine::calculateScore||calculate white chess score||chessData(curRow, curCol)=-1||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
								curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/
						}
						else if (curRow >= 0 && curRow < size &&
							curCol >= 0 && curCol < size &&
							m_chessboard->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							/*DEBUGLOG("Machine::calculateScore||calculate white chess score||chessData(curRow, curCol)=0||curRow={}||curCol={}||playerChessNum={}||AIChessNum={}||emptyNum={}",
								curRow, curCol, playerChessNum, AIChessNum, emptyNum);*/
							break;
						}
						else {
							break;
						}
					}

					if (AIChessNum == 0) {
						m_score_map[row][col] += 5;
						/*DEBUGLOG("Machine::calculateScore||calculate white chess score||AIChessNum=0||row={}||col={}||m_score_map[row][col]={}",
							row, col, m_score_map[row][col]);*/
					}
					else if (AIChessNum == 1) {
						m_score_map[row][col] += 10;
						/*DEBUGLOG("Machine::calculateScore||calculate white chess score||AIChessNum=1||row={}||col={}||m_score_map[row][col]={}",
							row, col, m_score_map[row][col]);*/
					}
					else if (AIChessNum == 2) {
						if (emptyNum == 1) {
							m_score_map[row][col] += 25;
							/*DEBUGLOG("Machine::calculateScore||calculate white chess score||AIChessNum=2||emptyNum=1||row={}||col={}||m_score_map[row][col]={}",
								row, col, m_score_map[row][col]);*/
						}
						else if (emptyNum == 2) {
							m_score_map[row][col] += 50;
							/*DEBUGLOG("Machine::calculateScore||calculate white chess score||AIChessNum=2||emptyNum=2||row={}||col={}||m_score_map[row][col]={}",
								row, col, m_score_map[row][col]);*/
						}
					}
					else if (AIChessNum == 3) {
						if (emptyNum == 1) {
							m_score_map[row][col] += 55;
							/*DEBUGLOG("Machine::calculateScore||calculate white chess score||AIChessNum=3||emptyNum=1||row={}||col={}||m_score_map[row][col]={}",
								row, col, m_score_map[row][col]);*/
						}
						else if (emptyNum == 2) {
							m_score_map[row][col] += 10000;
							/*DEBUGLOG("Machine::calculateScore||calculate white chess score||AIChessNum=3||emptyNum=2||row={}||col={}||m_score_map[row][col]={}",
								row, col, m_score_map[row][col]);*/
						}
					}
					else if (AIChessNum >= 4) {
						m_score_map[row][col] += 30000;
						/*DEBUGLOG("Machine::calculateScore||calculate white chess score||AIChessNum>=4||row={}||col={}||m_score_map[row][col]={}",
							row, col, m_score_map[row][col]);*/
					}
				}
			}
		}
	}
	// 打印计算的分数
	for (int i = 0; i < m_score_map.size(); i++)
	{
		std::vector<int> temp = m_score_map[i];
		std::stringstream ss;
		std::string str;
		std::copy(temp.begin(), temp.end(), std::ostream_iterator<int>(ss, ", "));
		str = ss.str();
		INFOLOG("Machine::calculateScore||row={}||value={}", i, str);
	}
}

ChessPos Machine::think()
{
	// 计算评分
	calculateScore();

	// 从评分中找出最大分数的位置
	int maxScore = 0;

	std::vector<ChessPos> maxPoints;
	int k = 0;

	int size = m_chessboard->getChessBoardSize();
	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			// 此位置为空
			if (m_chessboard->getChessData(row, col) == 0)
			{
				// 最大的分数和坐标
				if (m_score_map[row][col] > maxScore)
				{
					maxScore = m_score_map[row][col];
					maxPoints.clear();
					maxPoints.push_back(ChessPos{ row, col });
				}
				// 有多个最大的数，先存起来
				else if (m_score_map[row][col] == maxScore)
				{
					maxPoints.push_back(ChessPos{ row, col });
				}
			}
		}
	}
	// 输出最大值数组
	for (std::vector<ChessPos>::iterator it = maxPoints.begin(); it != maxPoints.end(); it++)
	{
		DEBUGLOG("Machine::think||maxPoints||row={}||col={}", it->chess_row, it->chess_col);
	}

	//随机落子，多个点 
	int index = rand() % maxPoints.size();
	return maxPoints[index];
}
