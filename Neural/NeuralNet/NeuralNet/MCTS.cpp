#include "MCTS.h"

#include "utility.h"

Movement MCTS::MakeDecision(const Board& a_board)
{
	Board newBoard = a_board;
	std::vector<Movement> possibleMovements = Checkers::GetValidMoves(newBoard, m_colour);

	float bestFitness = 0;
	uint bestMoveIndex = -1;

	float fitnessValue;
	int playouts = possibleMovements.size();

	for (int i = 0; i < possibleMovements.size(); i++)
	{
		fitnessValue = 0;
		Board nextBoard = newBoard;
		Checkers::RunMove(nextBoard, possibleMovements[i]);
		
		// play a number of simulated games
		for (int j = 0; j < m_playouts; j++)
		{
			// create a board copy for each simulation
			Board tempBoard = nextBoard;

			// ensure it starts on the other players' turn
			Colour turn = BLACK;
			if (m_colour == BLACK)
			{
				turn = WHITE;
			}

			// play the game all the way through
			while (!Checkers::GameOver(tempBoard))
			{

				std::vector<Movement> moves = Checkers::GetValidMoves(tempBoard, turn);

				if (moves.size() == 0)
				{
					break;
				}

				int index = rand() % moves.size();

				Movement chosenMove = moves[index];

				Checkers::RunMove(tempBoard, chosenMove);

				// swap turns
				if (turn == WHITE)
				{
					turn = BLACK;
				}
				else
				{
					turn = WHITE;
				}
			}

			if (Checkers::GameOver(tempBoard) == m_colour)
			{
				fitnessValue++;
			}
		}

		if (fitnessValue >= bestFitness)
		{
			bestFitness = fitnessValue;
			bestMoveIndex = i;
		}
	}

	return possibleMovements[bestMoveIndex];
}