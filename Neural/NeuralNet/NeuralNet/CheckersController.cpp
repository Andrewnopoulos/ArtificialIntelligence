#include "CheckersController.h"

using namespace CheckersGame;

CheckersController::CheckersController()
{
	m_game = new Checkers();
}

CheckersController::~CheckersController()
{
	delete m_game;
}