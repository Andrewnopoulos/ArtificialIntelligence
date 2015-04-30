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

	// best AI from each generation
	vector<TicTacAI> m_history;

	// game to play
	Tictactoe* m_game;

	GeneticAlg* m_geneticAlg;

	uint m_playerNumber;

	uint m_generations;

	uint m_numOfWeights;

	vector<float> m_fitnessAverages;

	vector<float> m_fitnessBests;

	void Initialize(uint boardSize, uint layers, uint nodesPerLayer);

	int PlayGame(TicTacAI& player1, TicTacAI& player2);

public:

	GameController();
	GameController(uint popsize, uint boardSize, uint layers, uint nodesPerLayer);

	TicTacAI GetBestAI();

	~GameController();

	// plays all AI's against eachother to work out fitnesses
	void Iterate(); 

	// plays all AI's against previous best AI to work out fitnesses
	void Iterate2();

	vector<float> GetFitnessBests() { return m_fitnessBests; }

};