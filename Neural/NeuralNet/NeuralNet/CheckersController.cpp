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
	Colour gameOver = m_game->GameOver();

	if (gameOver)
	{
		system("cls");
		switch (gameOver)
		{
		case WHITE:
			std::cout << "White wins!!" << std::endl;
			break;
		case BLACK:
			std::cout << "Black wins!!" << std::endl;
			break;
		default:
			break;
		}
		
		system("pause");
		m_game->ResetBoard();
	}

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

// checks if any possible jumps and forces the player to take one
void CheckersController::ChooseJump(Colour a_playerTurn, uint xPos, uint yPos)
{
	std::vector<Board> potentialJumps;
	std::vector<Move> potentialMoves;

	bool validJumps = m_game->ValidJumpExists(a_playerTurn);

	if (!validJumps)
	{
		return;
	}

	if (m_game->isValidJump(xPos, yPos, Direction::UPLEFT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, xPos, yPos, UPLEFT);
		potentialJumps.push_back(newBoard);
		potentialMoves.push_back(Move(xPos, yPos, UPLEFT, JUMP));
	}
	if (m_game->isValidJump(xPos, yPos, Direction::UPRIGHT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, xPos, yPos, UPRIGHT);
		potentialJumps.push_back(newBoard);
		potentialMoves.push_back(Move(xPos, yPos, UPRIGHT, JUMP));
	}
	if (m_game->isValidJump(xPos, yPos, Direction::DOWNLEFT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, xPos, yPos, DOWNLEFT);
		potentialJumps.push_back(newBoard);
		potentialMoves.push_back(Move(xPos, yPos, DOWNLEFT, JUMP));
	}
	if (m_game->isValidJump(xPos, yPos, Direction::DOWNRIGHT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, xPos, yPos, DOWNRIGHT);
		potentialJumps.push_back(newBoard);
		potentialMoves.push_back(Move(xPos, yPos, DOWNRIGHT, JUMP));
	}

	int selection = 0;
	int maxSelection = potentialJumps.size();

	if (maxSelection == selection)
	{
		return;
	}

	system("cls");
	m_game->DrawBoard(potentialJumps[0]);

	while (true)
	{
		UpdateKeyboardInput();

		if (GetKeyPressed(VK_A))
		{
			selection--;
			if (selection < 0)
			{
				selection = maxSelection - 1;
			}
			system("cls");
			m_game->DrawBoard(potentialJumps[selection]);
		}

		if (GetKeyPressed(VK_D))
		{
			selection++;
			if (selection >= maxSelection)
			{
				selection = 0;
			}
			system("cls");
			m_game->DrawBoard(potentialJumps[selection]);
		}

		if (GetKeyPressed(VK_RETURN))
		{
			m_game->SetBoardState(potentialJumps[selection]);

			Move selected = potentialMoves[selection];

			if (selected.m_move == MoveType::JUMP)
			{
				switch (selected.m_direction)
				{
				case UPLEFT:
					ChooseJump(a_playerTurn, m_activeX - 2, m_activeY + 2);
					break;
				case UPRIGHT:
					ChooseJump(a_playerTurn, m_activeX + 2, m_activeY + 2);
					break;
				case DOWNLEFT:
					ChooseJump(a_playerTurn, m_activeX - 2, m_activeY - 2);
					break;
				case DOWNRIGHT:
					ChooseJump(a_playerTurn, m_activeX + 2, m_activeY - 2);
					break;
				default:
					break;
				}
			}
			break;
		}

		if (GetKeyPressed(VK_ESCAPE))
		{
			break;
		}
	}
}

void CheckersController::ChooseMove(Colour a_playerTurn)
{
	std::vector<Board> potentialTurns;
	std::vector<Board> potentialJumps;

	std::vector<Movement> potentialMoves;

	bool validJumps = m_game->ValidJumpExists(a_playerTurn);

	if (m_game->isValidMove(m_activeX, m_activeY, Direction::UPLEFT) && !validJumps)
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Move(newBoard, m_activeX, m_activeY, UPLEFT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidMove(m_activeX, m_activeY, Direction::UPRIGHT) && !validJumps)
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Move(newBoard, m_activeX, m_activeY, UPRIGHT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidMove(m_activeX, m_activeY, Direction::DOWNLEFT) && !validJumps)
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Move(newBoard, m_activeX, m_activeY, DOWNLEFT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidMove(m_activeX, m_activeY, Direction::DOWNRIGHT) && !validJumps)
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Move(newBoard, m_activeX, m_activeY, DOWNRIGHT);
		potentialTurns.push_back(newBoard);
	}
	if (m_game->isValidJump(m_activeX, m_activeY, Direction::UPLEFT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, m_activeX, m_activeY, UPLEFT);
		potentialJumps.push_back(newBoard);
		potentialMoves.push_back(Movement(m_activeX, m_activeY, UPLEFT, JUMP));
	}
	if (m_game->isValidJump(m_activeX, m_activeY, Direction::UPRIGHT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, m_activeX, m_activeY, UPRIGHT);
		potentialJumps.push_back(newBoard);
		potentialMoves.push_back(Movement(m_activeX, m_activeY, UPRIGHT, JUMP));
	}
	if (m_game->isValidJump(m_activeX, m_activeY, Direction::DOWNLEFT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, m_activeX, m_activeY, DOWNLEFT);
		potentialJumps.push_back(newBoard);
		potentialMoves.push_back(Movement(m_activeX, m_activeY, DOWNLEFT, JUMP));
	}
	if (m_game->isValidJump(m_activeX, m_activeY, Direction::DOWNRIGHT))
	{
		Board newBoard = m_game->GetBoardState();
		m_game->Jump(newBoard, m_activeX, m_activeY, DOWNRIGHT);
		potentialJumps.push_back(newBoard);
		potentialMoves.push_back(Movement(m_activeX, m_activeY, DOWNRIGHT, JUMP));
	}
	// potentialTurns now holds a series of potential destination boards
	// use A and D to cycle between them

	if (potentialJumps.size() > 0)
	{
		potentialTurns = potentialJumps;
	}

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

			if (potentialJumps.size() != 0)
			{
				Movement selected = potentialMoves[selection];

				if (selected.m_move == MoveType::JUMP)
				{
					switch (selected.m_direction)
					{
					case UPLEFT:
						ChooseJump(a_playerTurn, m_activeX - 2, m_activeY + 2);
						break;
					case UPRIGHT:
						ChooseJump(a_playerTurn, m_activeX + 2, m_activeY + 2);
						break;
					case DOWNLEFT:
						ChooseJump(a_playerTurn, m_activeX - 2, m_activeY - 2);
						break;
					case DOWNRIGHT:
						ChooseJump(a_playerTurn, m_activeX + 2, m_activeY - 2);
						break;
					default:
						break;
					}
				}
			}
			
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