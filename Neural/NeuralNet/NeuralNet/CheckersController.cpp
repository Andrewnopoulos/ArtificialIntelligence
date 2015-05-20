#include "CheckersController.h"
#include <WinUser.h>
#include <iostream>

using namespace CheckersGame;

CheckersController::CheckersController()
{
	m_game = new Checkers();

	m_keystates[VK_A] = 0;
	m_keystates[VK_S] = 0;
	m_keystates[VK_W] = 0;
	m_keystates[VK_D] = 0;
	m_keystates[VK_RETURN] = 0;

	m_prevstates[VK_A] = 0;
	m_prevstates[VK_S] = 0;
	m_prevstates[VK_W] = 0;
	m_prevstates[VK_D] = 0;
	m_prevstates[VK_RETURN] = 0;
}

CheckersController::~CheckersController()
{
	delete m_game;

	drawTimer = 0;
}

void CheckersController::UpdateKeyboardInput()
{
	for (int i = 0; i < NUMBEROFKEYS; i++)
	{
		m_prevstates[KeyList[i]] = m_keystates[KeyList[i]];
		m_keystates[KeyList[i]] = GetKeyState(KeyList[i]);
	}
}

bool CheckersController::GetKeyDown(short a_key)
{
	return (m_keystates[a_key] >> 8) * -1;
}

bool CheckersController::GetKeyReleased(short a_key)
{
	bool previouslyHeld = (m_prevstates[a_key] >> 8) * -1;

	// return if it's not currently down, but it was previously held
	return !GetKeyDown(a_key) && previouslyHeld;
}

bool CheckersController::GetKeyPressed(short a_key)
{
	bool previouslyHeld = (m_prevstates[a_key] >> 8) * -1;

	// return if it's currently down, but wasn't previously held
	return GetKeyDown(a_key) && !previouslyHeld;
}

void CheckersController::Update(float deltaTime)
{
	UpdateKeyboardInput();

	Colour turn = m_game->GetPosition(m_activeX, m_activeY);

	if (GetKeyPressed(VK_A) && m_activeX > 0)
	{
		m_activeX--;
		system("cls");
		turn = m_game->DrawBoard(m_activeX, m_activeY);
	}
	if (GetKeyPressed(VK_W) && m_activeY < 7)
	{
		m_activeY++;
		system("cls");
		turn = m_game->DrawBoard(m_activeX, m_activeY);
	}
	if (GetKeyPressed(VK_S) && m_activeY > 0)
	{
		m_activeY--;
		system("cls");
		turn = m_game->DrawBoard(m_activeX, m_activeY);
	}
	if (GetKeyPressed(VK_D) && m_activeX < 7)
	{
		m_activeX++;
		system("cls");
		turn = m_game->DrawBoard(m_activeX, m_activeY);
	}

	if (GetKeyPressed(VK_RETURN) && turn == m_playerTurn)
	{
		ChooseMove(turn);
	}
}

void CheckersController::ChooseMove(Colour a_playerTurn)
{
	std::vector<Board> potentialTurns;

	if (m_game->isValidMove(m_activeX, m_activeY, Direction::UPLEFT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Move(newBoard, m_activeX, m_activeY, UPLEFT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidMove(m_activeX, m_activeY, Direction::UPRIGHT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Move(newBoard, m_activeX, m_activeY, UPRIGHT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidMove(m_activeX, m_activeY, Direction::DOWNLEFT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Move(newBoard, m_activeX, m_activeY, DOWNLEFT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidMove(m_activeX, m_activeY, Direction::DOWNRIGHT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Move(newBoard, m_activeX, m_activeY, DOWNRIGHT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidJump(m_activeX, m_activeY, Direction::UPLEFT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, m_activeX, m_activeY, UPLEFT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidJump(m_activeX, m_activeY, Direction::UPRIGHT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, m_activeX, m_activeY, UPRIGHT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidJump(m_activeX, m_activeY, Direction::DOWNLEFT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, m_activeX, m_activeY, DOWNLEFT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidJump(m_activeX, m_activeY, Direction::DOWNRIGHT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, m_activeX, m_activeY, DOWNRIGHT);
		potentialTurns.push_back(newBoard);
	}
	// potentialTurns now holds a series of potential destination boards
	// use A and D to cycle between them

	int selection = 0;
	int maxSelection = potentialTurns.size();

	if (maxSelection == selection)
	{
		return;
	}

	system("cls");
	m_game->DrawBoard(potentialTurns[0]);

	while (true)
	{
		UpdateKeyboardInput();

		if (GetKeyPressed(VK_A))
		{
			selection--;
			if (selection < 0)
			{
				selection = maxSelection-1;
			}
			system("cls");
			m_game->DrawBoard(potentialTurns[selection]);
		}

		if (GetKeyPressed(VK_D))
		{
			selection++;
			if (selection >= maxSelection)
			{
				selection = 0;
			}
			system("cls");
			m_game->DrawBoard(potentialTurns[selection]);
		}

		if (GetKeyPressed(VK_RETURN))
		{
			m_game->SetBoardState(potentialTurns[selection]);
			if (m_playerTurn == BLACK)
			{
				m_playerTurn = WHITE;
			}
			else if (m_playerTurn == WHITE)
			{
				m_playerTurn = BLACK;
			}
			break;
		}

		if (GetKeyPressed(VK_ESCAPE))
		{
			break;
		}
		
	}

}

void CheckersController::Draw(float deltaTime)
{
	drawTimer += deltaTime;

	if (drawTimer >= 1)
	{
		system("cls");
		drawTimer = 0;
		m_game->DrawBoard(m_activeX, m_activeY);
	}
}