#include "MCTS.h"

Movement MCTS::MakeDecision(Board& a_board)
{
	std::vector<Movement> possibleMovements = Checkers::GetValidMoves(a_board, m_colour);

	// for each possible move
	int* values = new int[possibleMovements.size()];

	for (int i = 0; i < possibleMovements.size(); i++)
	{
		Board newBoard = a_board;
		while (!Checkers::GameOver(a_board))
		{

		}
		
	}

	return Movement(0, 0, UPLEFT, JUMP);
}