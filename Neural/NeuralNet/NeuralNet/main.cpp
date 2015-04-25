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

	cout << "Iterating " << endl;

	for (int i = 0; i < 50; i++)
	{
		controller->Iterate();
		cout << ".";
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
			boardsize *= boardsize;

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
	else if (game->winnerIs() == -1)
	{
		cout << "You Lose!!!" << endl;
	}
	else
	{
		cout << "Draw!!" << endl;
	}

	delete game;

}

#include <assert.h>

bool UnitTesting()
{
	Neuron n = Neuron(4);
	assert(n.m_inputs = 4);

	NeuronLayer layer = NeuronLayer(3, 3);
	vector<Neuron> neuronsInLayer = layer.m_vecNeurons;
	assert(neuronsInLayer.size() == 3);

	cout << neuronsInLayer[0].m_inputs;
	// require additional weight value for bias
	assert(neuronsInLayer[0].m_inputs == 4);
	assert(neuronsInLayer[1].m_inputs == 4);
	assert(neuronsInLayer[2].m_inputs == 4);

	// 4 inputs, 2 outputs, 3 hidden layers, 5 nodes per hidden layer
	NeuralNet net = NeuralNet(4, 2, 3, 5);
	cout << "number of weights in neural net: " << net.GetNumberOfWeights() << endl;
	

	return true;
}

int main()
{

	srand(time(NULL));

	RunGenAlg();

	return 0;
}