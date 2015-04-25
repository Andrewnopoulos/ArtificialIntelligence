#pragma once
#include "NeuralNet.h"

class TicTacAI
{
private:

	// brain
	NeuralNet m_brain;

	float m_fitness;

	// can be 1 or -1 depending on opponent
	int m_team;

public:

	TicTacAI(uint boardsize, uint layers, uint innerNodes);

	// updates Neural net with info from game
	bool Update(int* a_boardState, uint a_sizeOfBoard);

	void Reset();

	// accessors
	inline int GetTeam() const { return m_team; }
	inline float GetFitness() const { return m_fitness; }
	int GetNumberOfWeights() const { return m_brain.GetNumberOfWeights(); }

	void Win() { m_fitness++; }
	void Loss() { m_fitness--; }
	void PutWeights(vector<float> &w) { m_brain.PutWeights(w); }

};