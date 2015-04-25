#include "TicTacAI.h"

TicTacAI::TicTacAI(uint boardsize, uint layers, uint innernodes) : 
	m_fitness(0),
	m_team(0),			// outputs x, y co-ordinates
	m_brain(NeuralNet(boardsize, 2, layers, innernodes))
{

}

// resets fitness
void TicTacAI::Reset()
{
	m_fitness = 0;
}

bool TicTacAI::Update(int* a_boardState, uint a_sizeOfBoard)
{
	vector<float> inputs;

	for (int i = 0; i < a_sizeOfBoard; i++)
	{
		inputs.push_back(a_boardState[i]);
	}

	vector<float> outputs = m_brain.Update(inputs);

	if (outputs.size() < 2)
	{
		return false;
	}

	// todo: determine how to turn output into a move



	return true;
}