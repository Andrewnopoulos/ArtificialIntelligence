#pragma once
#include "Checkers.h"

using namespace CheckersGame;

class CheckersController
{
private:
	Checkers* m_game;


public:
	CheckersController();
	~CheckersController();

	bool Turn(Colour a_colour);

};