#pragma once

#include "Tictactoe.h"
#include "TicTacAI.h"
#include "GeneticAlg.h"
#include "utility.h"

class GameController
{
private:

	// genome storage
	vector<Genome> m_population;

	// player AI's
	vector<TicTacAI> m_players;

	// game to play
	Tictactoe* m_game;

	GeneticAlg* m_geneticAlg;

	uint m_playerNumber;

	uint m_generations;

	uint m_numOfWeights;

	vector<float> m_fitnessAverages;

	vector<float> m_fitnessBests;

	void Initialize(uint boardSize, uint layers, uint nodesPerLayer);

public:

	GameController();
	GameController(uint popsize, uint boardSize, uint layers, uint nodesPerLayer);

	~GameController();

	bool Iterate();

};