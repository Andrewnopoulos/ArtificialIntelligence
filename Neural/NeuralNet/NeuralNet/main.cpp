#include "Tictactoe.h"
#include "GameController.h"
#include <iostream>
#include <time.h>

using namespace std;

void RunMultiplayer()
{
	Tictactoe* game = new Tictactoe(3);
	int turn = 1;

	while (!game->winnerIs())
	{
		uint xpos = 0;
		uint ypos = 0;

		cout << "Turn for " << turn << endl;

		cout << "x position:" << endl;
		cin >> xpos;
		cout << "y position:" << endl;
		cin >> ypos;

		// if move successfully made, swap turns
		if (game->makeMove(ypos, xpos, turn))
		{
			turn = -turn;
			game->printBoard();
		}
		else
		{
			cout << "Incompatible move" << endl;
		}
	}

	delete game;
}

void RunGenAlg()
{
	GameController* controller = new GameController();

	for (int i = 0; i < 50; i++)
	{
		controller->Iterate();
	}

	TicTacAI bestAI = controller->GetBestAI();

	Tictactoe* game = new Tictactoe(3);
	int turn = 1;

	while (!game->winnerIs())
	{
		if (turn == 1)
		{
			uint xpos = 0;
			uint ypos = 0;

			cout << "Take your turn, mortal" << endl;

			cout << "x position:" << endl;
			cin >> xpos;
			cout << "y position:" << endl;
			cin >> ypos;

			// if move successfully made, swap turns
			if (game->makeMove(ypos, xpos, turn))
			{
				turn = -turn;
			}
			else
			{
				cout << "Incompatible move" << endl;
			}
		}
		else
		{
			int* boardstate = game->getBoard();
			uint boardsize = game->getDimensions();

			if (game->makeMove(bestAI.CalcMove(boardstate, boardsize), turn))
			{
				turn = -turn;
				game->printBoard();
			}
		}
	}

	if (game->winnerIs() == 1)
	{
		cout << "You win!!" << endl;
	}
	else
	{
		cout << "You Lose!!!" << endl;
	}

	delete game;

}

int main()
{

	srand(time(NULL));

	RunGenAlg();

	return 0;
}