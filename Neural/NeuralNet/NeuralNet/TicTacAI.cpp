#include "TicTacAI.h"

TicTacAI::TicTacAI(uint boardsize, uint layers, uint innernodes) : 
	m_fitness(0),
	m_team(0),			// outputs x, y co-ordinates
	m_brain(NeuralNet(boardsize, boardsize, layers, innernodes))
{

}

// resets fitness
void TicTacAI::Reset()
{
	m_fitness = 0;
}

uint TicTacAI::CalcMove(int* a_boardState, uint a_sizeOfBoard)
{
	vector<float> inputs;

	for (int i = 0; i < a_sizeOfBoard; i++)
	{
		inputs.push_back(a_boardState[i]);
	}

	vector<float> outputs = m_brain.Update(inputs);

	if (outputs.size() < a_sizeOfBoard)
	{
		return a_sizeOfBoard;
	}

	// todo: determine how to turn output into a move
	// return position of highest value

	uint locationOfMax = 0;
	float maxValue = outputs[0];

	for (int i = 1; i < a_sizeOfBoard; i++)
	{
		// if new max is found, set it
		if (outputs[i] > maxValue)
		{
			locationOfMax = i;
			maxValue = outputs[i];
		}
	}

	return locationOfMax;

}