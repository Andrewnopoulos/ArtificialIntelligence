#pragma once
#include "Checkers.h"
#include "MCTS.h"
#include <map>

using namespace CheckersGame;

#define VK_W 0x57
#define VK_S 0x53
#define VK_A 0x41
#define VK_D 0x44

static const uint NUMBEROFKEYS = 6;

static short KeyList[NUMBEROFKEYS]
{
	VK_W,
	VK_S,
	VK_A,
	VK_D,
	VK_RETURN,
	VK_ESCAPE
};

class CheckersController
{
private:
	Checkers* m_game;

	uint m_activeX;
	uint m_activeY;

	float drawTimer;

	std::map<short, SHORT> m_keystates;
	std::map<short, SHORT> m_prevstates;

	Colour m_playerTurn = WHITE;

	MCTS* m_ai;

public:
	CheckersController();
	~CheckersController();

	void Draw(float deltaTime);
	void Update(float deltaTime);

	void UpdateKeyboardInput();

	bool GetKeyDown(short a_key);

	bool GetKeyReleased(short a_key);

	bool GetKeyPressed(short a_key);

	void ChooseMove(Colour a_playerTurn);

	void ChooseJump(Colour a_playerTurn, uint xPos, uint yPos);

	void AIturn();

};