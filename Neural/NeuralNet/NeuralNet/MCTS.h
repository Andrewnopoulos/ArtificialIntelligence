#pragma once

#include "Checkers.h"

using namespace CheckersGame;

class MCTS
{
public:
	MCTS(int playouts, Colour a_colour) : m_playouts(playouts), m_colour(a_colour)
	{
	}

	virtual ~MCTS(){}

	Movement MakeDecision(Board& a_board);

private:
	// number of times it wil simulate a game
	int m_playouts;
	Colour m_colour;
};