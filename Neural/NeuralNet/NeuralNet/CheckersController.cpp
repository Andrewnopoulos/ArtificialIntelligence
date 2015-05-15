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

void CheckersController::UpdateKeyboardInput(float deltaTime)
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
	UpdateKeyboardInput(deltaTime);

	Colour turn = FREEBLACK;

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