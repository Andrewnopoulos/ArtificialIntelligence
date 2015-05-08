#include "Tictactoe.h"
#include "GameController.h"
#include <iostream>
#include <fstream>
#include <time.h>

#include "Checkers.h"

using namespace std;

//unsigned char* FileToBuffer(const char* a_Name)
//{
//	// open file for text reading
//	FILE* pFile = fopen(a_Name, "rb");
//	if (pFile == nullptr)
//	{
//		printf("Error: unable to open file '%s' for reading!\n", a_Name);
//		return nullptr;
//	}
//
//	// get number of bytes in file
//	fseek(pFile, 0, SEEK_END);
//	unsigned int fileLength = (unsigned int)ftell(pFile);
//	fseek(pFile, 0, SEEK_SET);
//
//	// allocate buffer and read file contents
//	unsigned char* buffer = new unsigned char[fileLength + 1];
//	memset(buffer, 0, fileLength + 1);
//	fread(buffer, sizeof(unsigned char), fileLength, pFile);
//
//	fclose(pFile);
//	return buffer;
//}

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

	for (int i = 0; i < DEFAULTITERATIONS; i++)
	{
		controller->Iterate2();
		cout << ".";
	}

	vector<float> fitnessBests = controller->GetFitnessBests();
	vector<float> fitnessAverages = controller->GetFitnessAverages();

	for (int i = 0; i < fitnessAverages.size(); i++)
	{
		cout << "Generation " << i << " average fitness: " << fitnessAverages[i] << endl;
	}

	TicTacAI bestAI = controller->GetBestAI();

	uint bestwins = 0;
	uint stoogewins = 0;
	uint draws = 0;

	for (int k = 0; k < 10000; k++)
	{
		Tictactoe* testGame = new Tictactoe(DEFAULTBOARDSIZE);
		TicTacAI stoogeAI = TicTacAI(DEFAULTBOARDSIZE, DEFAULTNODELAYERS, DEFAULTLAYERSIZE);

		int testTurn = (RandBool() ? 1 : -1); // first turn is random

		while (!testGame->winnerIs())
		{
			int* boardstate = testGame->getBoard();
			uint boardsize = testGame->getDimensions();
			boardsize *= boardsize;
			if (testTurn == 1)
			{
				if (testGame->makeMove(bestAI.CalcMove(boardstate, boardsize), testTurn))
				{
					testTurn = -testTurn;
				}
			}
			else if (testTurn == -1)
			{
				if (testGame->makeMove(stoogeAI.CalcMove(boardstate, boardsize), testTurn))
				{
					testTurn = -testTurn;
				}
			}
		}

		int winner = testGame->winnerIs();
		if (winner == 1)
		{
			bestwins++;
		}
		else if (winner == -1)
		{
			stoogewins++;
		}
		else if (winner == -2)
		{
			draws++;
		}

	//	testGame->printBoard();
		delete testGame;

	}

	cout << endl << "Best AI wins: " << bestwins << endl;
	cout << "Random AI wins: " << stoogewins << endl;
	cout << "Draws: " << draws << endl;

	std::ofstream myfile;
	myfile.open("potato.csv", std::ios_base::app);
	myfile << "\nBestwins " << bestwins << "\n";
	myfile << "Stoogewins " << stoogewins << "\n";
	myfile << "Draws " << draws;
	myfile.close();

	//Tictactoe* game = new Tictactoe(DEFAULTBOARDSIZE);
	//int turn = 1;

	//while (!game->winnerIs())
	//{
	//	if (turn == 1)
	//	{
	//		uint xpos = 0;
	//		uint ypos = 0;

	//		cout << "Take your turn, mortal" << endl;

	//		cout << "x position:" << endl;
	//		cin >> xpos;
	//		cout << "y position:" << endl;
	//		cin >> ypos;

	//		// if move successfully made, swap turns
	//		if (game->makeMove(ypos, xpos, turn))
	//		{
	//			turn = -turn;
	//		}
	//		else
	//		{
	//			cout << "Incompatible move" << endl;
	//		}
	//	}
	//	else
	//	{
	//		int* boardstate = game->getBoard();
	//		uint boardsize = game->getDimensions();
	//		boardsize *= boardsize;

	//		if (game->makeMove(bestAI.CalcMove(boardstate, boardsize), turn))
	//		{
	//			turn = -turn;
	//			game->printBoard();
	//		}
	//	}
	//}

	//if (game->winnerIs() == 1)
	//{
	//	cout << "You win!!" << endl;
	//}
	//else if (game->winnerIs() == -1)
	//{
	//	cout << "You Lose!!!" << endl;
	//}
	//else
	//{
	//	cout << "Draw!!" << endl;
	//}

	//cin >> turn;

	//delete game;

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


bool CheckersTesting()
{

	Checkers m_checkersGame;

	m_checkersGame.ResetBoard();

	m_checkersGame.DrawBoard();

	assert(m_checkersGame.isValidMove(0, 0, Direction::UPLEFT) == false);
	assert(m_checkersGame.isValidMove(0, 0, Direction::UPRIGHT) == false);
	assert(m_checkersGame.isValidMove(0, 2, Direction::UPRIGHT) == true);
	assert(m_checkersGame.isValidMove(0, 2, Direction::UPLEFT) == false);

	system("pause>nul");

	return true;
}

int main()
{

	srand(time(NULL));

	CheckersTesting();

	return 0;
}