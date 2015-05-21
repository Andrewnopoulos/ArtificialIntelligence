#pragma once

#include "Checkers.h"

using namespace CheckersGame;

class MCTS
{
public:
	MCTS(int playouts) : m_playouts(playouts)
	{
	}

	virtual ~MCTS(){}

	virtual int MakeDecision(const Board& a_board);

private:
	// number of times it wil simulate a game
	int m_playouts;
};