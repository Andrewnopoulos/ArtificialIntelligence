#include "GameController.h"

GameController::GameController()
{
	m_playerNumber = DEFAULTPOPULATIONSIZE;
	Initialize(DEFAULTBOARDSIZE, DEFAULTNODELAYERS, DEFAULTLAYERSIZE);
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

	m_game = new Tictactoe(DEFAULTBOARDSIZE);

	m_history.push_back(TicTacAI(boardSize, layers, nodesPerLayer));
}

GameController::~GameController()
{
	delete m_game;
	delete m_geneticAlg;
}

// plays a game of tic tac toe and returns -1 if p1 wins, 1 if p2 wins
int GameController::PlayGame(TicTacAI& player1, TicTacAI& player2)
{
	m_game->resetBoard();

	int turn = -1;

	int numOfTurns = 0;

	while (!m_game->winnerIs())
	{
		numOfTurns++;

		int* boardstate = m_game->getBoard();
		uint boardsize = m_game->getDimensions();
		boardsize = boardsize * boardsize;
		if (turn == -1)
		{
			if (m_game->makeMove(player1.CalcMove(boardstate, boardsize), turn))
			{
				turn = -turn;
			}
		}
		else // turn = 1
		{
			if (m_game->makeMove(player2.CalcMove(boardstate, boardsize), turn))
			{
				turn = -turn;
			}
		}
	}
	int winner = m_game->winnerIs();
	
	// draw
	if (winner == -2)
	{
		return 0;
	}

	return numOfTurns * m_game->winnerIs();
}

void GameController::Iterate()
{
	// iterate through all players and pit them against eachother
	for (int i = 0; i < m_playerNumber; i++)
	{
		for (int j = 0; j < m_playerNumber; j++)
		{
			if (!(i == j))
			{
				int winner = PlayGame(m_players[i], m_players[j]);

				if (winner < 0)
				{
					m_players[i].Win();
					m_players[j].Loss();
				//	m_players[i].Win(18 - abs(winner));
				//	m_players[j].Lose(18);
				}
				else if (winner > 0)
				{
					m_players[j].Win();
					m_players[i].Loss();
				//	m_players[j].Win(18 - abs(winner));
				//	m_players[i].Lose(18);
				}
				m_population[i].m_fitness = m_players[i].GetFitness();
				m_population[j].m_fitness = m_players[j].GetFitness();
			}
		}
	}

	// run through genetic algorithm
	m_fitnessAverages.push_back(m_geneticAlg->AverageFitness());
	m_fitnessBests.push_back(m_geneticAlg->BestFitness());

	m_generations++;

	m_population = m_geneticAlg->Epoch(m_population);

	// insert improved brains into AI's
	for (int i = 0; i < m_playerNumber; i++)
	{
		m_players[i].PutWeights(m_population[i].m_vecWeights);
		m_players[i].Reset();
	}
}

void GameController::Iterate2()
{
	for (int i = 0; i < m_playerNumber; i++)
	{
		for (int j = 0; j < m_history.size(); j++)
		{
			if (RandBool())
			{
				int winner = PlayGame(m_players[i], m_history[j]);

				if (winner < 0)
				{
					m_players[i].Win();
				}
				else if (winner > 0)
				{
					m_players[i].Loss();
				}
			}
			else
			{
				int winner = PlayGame(m_history[j], m_players[i]);

				if (winner > 0)
				{
					m_players[i].Win();
				}
				else if (winner < 0)
				{
					m_players[i].Loss();
				}
			}
		}
		m_population[i].m_fitness = m_players[i].GetFitness();
	}

	// run through genetic algorithm
	m_fitnessAverages.push_back(m_geneticAlg->AverageFitness());
	m_fitnessBests.push_back(m_geneticAlg->BestFitness());

	m_generations++;

	// caps maximum size of history vector to 50, replacing worst ones with better ones (hopefully)
	if (m_history.size() >= 20)
	{
		float worstFitness = 99999;
		uint worstIndex = 0;

		// determine least fittest in history
		for (int i = 0; i < m_history.size(); i++)
		{
			if (m_history[i].GetFitness() < worstFitness)
			{
				worstIndex = i;
				worstFitness = m_history[i].GetFitness();
			}
		}
		if (GetBestAI().GetFitness() > worstFitness)
		{
			m_history[worstFitness] = GetBestAI();
		}
	}
	else
	{
		m_history.push_back(GetBestAI());
	}
	

	m_population = m_geneticAlg->Epoch(m_population);

	// insert improved brains into AI's
	for (int i = 0; i < m_playerNumber; i++)
	{
		m_players[i].PutWeights(m_population[i].m_vecWeights);
		m_players[i].Reset();
	}

}

TicTacAI GameController::GetBestAI()
{
	float bestFitness = 0;
	uint bestIndex = 0;
	// determine fittest in population
	for (int i = 0; i < m_playerNumber; i++)
	{
		if (m_population[i].m_fitness > bestFitness)
		{
			bestFitness = m_population[i].m_fitness;
			bestIndex = i;
		}
	}
	return m_players[bestIndex];
}