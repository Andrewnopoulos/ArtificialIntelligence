#include "GameController.h"

GameController::GameController()
{
	m_playerNumber = DEFAULTPOPULATIONSIZE;
	Initialize(DEFAULTSIZE, DEFAULTNODELAYERS, DEFAULTLAYERSIZE);
}

GameController::GameController(uint popsize, uint boardSize, uint layers, uint nodesPerLayer)
{
	m_playerNumber = popsize;
	Initialize(boardSize, layers, nodesPerLayer);
}

void GameController::Initialize(uint boardSize, uint layers, uint nodesPerLayer)
{
	for (int i = 0; i < m_playerNumber; i++)
	{
		m_players.push_back(TicTacAI(boardSize, layers, nodesPerLayer));
	}

	m_numOfWeights = m_players[0].GetNumberOfWeights();

	m_geneticAlg = new GeneticAlg(m_playerNumber, 
		DEFAULTMUTATIONRATE, 
		DEFAULTCROSSOVERRATE, 
		m_numOfWeights);

	m_population = m_geneticAlg->GetChromosomes();

	for (int i = 0; i < m_playerNumber; i++)
	{
		m_players[i].PutWeights(m_population[i].m_vecWeights);
	}
}


GameController::~GameController()
{
	delete m_game;
	delete m_geneticAlg;
}

bool GameController::Iterate()
{

}