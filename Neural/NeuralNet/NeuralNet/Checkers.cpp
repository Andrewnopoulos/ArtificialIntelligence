#include "Checkers.h"
#include <iostream>
#include <bitset>
#include <algorithm>
#include <stdio.h>
#include <WinBase.h>
#include <Windows.h>

using namespace CheckersGame;

Checkers::Checkers()
{
	ResetBoard();

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Checkers::ResetBoard()
{
	memset(&m_board, 0, sizeof(Board));

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			SetPosition(FREEBLACK, i, j);
		}
	}

	SetPosition(WHITE, 0, 0);
	SetPosition(WHITE, 2, 0);
	SetPosition(WHITE, 4, 0);
	SetPosition(WHITE, 6, 0);
	SetPosition(WHITE, 1, 1);
	SetPosition(WHITE, 3, 1);
	SetPosition(WHITE, 5, 1);
	SetPosition(WHITE, 7, 1);
	SetPosition(WHITE, 0, 2);
	SetPosition(WHITE, 2, 2);
	SetPosition(WHITE, 4, 2);
	SetPosition(WHITE, 6, 2);

	SetPosition(BLACK, 1, 7);
	SetPosition(BLACK, 3, 7);
	SetPosition(BLACK, 5, 7);
	SetPosition(BLACK, 7, 7);
	SetPosition(BLACK, 0, 6);
	SetPosition(BLACK, 2, 6);
	SetPosition(BLACK, 4, 6);
	SetPosition(BLACK, 6, 6);
	SetPosition(BLACK, 1, 5);
	SetPosition(BLACK, 3, 5);
	SetPosition(BLACK, 5, 5);
	SetPosition(BLACK, 7, 5);
}

/*

Currently checking if the desired move is to a square that's on the board and it isn't occupied already

TODO
Check if you can do a jump
check direction based on colour/kingship

*/
bool Checkers::isValidMove(uint xPos, uint yPos, Direction a_direction)
{
	uint index = 8 * yPos + xPos;

	// index is now the desired destination bit
	uint originalPiece = boardIndices[index];

	uint offset = boardIndices[index] + a_direction;

	if (std::find(validMoves.begin(), validMoves.end(), offset) != validMoves.end()) // if the move is at least on the board
	{
		long long bitmask = (1LL << offset);

		// if there's a black piece already on the spot
		if (m_board.m_BlackPieces & bitmask)
		{
			return false;
		}

		if (m_board.m_WhitePieces & bitmask)
		{
			return false;
		}

		return true;
	}
	return false;
}

bool Checkers::isValidJump(uint xPos, uint yPos, Direction a_direction)
{
	uint index = 8 * yPos + xPos;

	uint enemyOffset = boardIndices[index] + a_direction;

	uint destinationOffset = boardIndices[index] + 2 * a_direction;

	Colour currentPosition = GetPosition(xPos, yPos);

	Colour enemyPosition = GetPosition(enemyOffset);

	switch (enemyPosition)
	{
	case BLACK: // if enemy position is black
		if (currentPosition == BLACK) // and current position is also black
		{
			return false; // can't jump
		}
		break;
	case WHITE:
		if (currentPosition == WHITE)
		{
			return false;
		}
		break;
	default: // if enemy position isn't black or white
		return false; // can't jump
	}

	// if destination isn't even on the board
	if (std::find(validMoves.begin(), validMoves.end(), destinationOffset) == validMoves.end())
	{
		return false;
	}

	long long bitmask = (1LL << destinationOffset);
	
	// if there's a black piece already on the spot
	if (m_board.m_BlackPieces & bitmask)
	{
		return false;
	}
	if (m_board.m_WhitePieces & bitmask)
	{
		return false;
	}

	return true;

	// check if the enemy colour is opposite to yours - done
	// check if destination square is empty and valid - done
}

bool Checkers::SetPosition(Colour inputColour, uint xPos, uint yPos)
{
	int index = 8 * yPos + xPos;

	int boardLocation = boardIndices[index];

	if (boardLocation == -1)
	{
		return false;
	}

	long long offset = (1LL << boardLocation);

	if (inputColour == Colour::WHITE)
	{
		m_board.m_WhitePieces |= offset;
		m_board.m_BlackPieces &= (~offset);
	}
	else if (inputColour == Colour::BLACK)
	{
		m_board.m_BlackPieces |= offset;
		m_board.m_WhitePieces &= (~offset);
	}

	return true;
}

Colour Checkers::GetPosition(long long boardLocation)
{
	long long offset = (1LL << boardLocation);

	if (m_board.m_WhitePieces & offset)
	{
		return WHITE;
	}

	if (m_board.m_BlackPieces & offset)
	{
		return BLACK;
	}

	return FREEBLACK;
}

Colour Checkers::GetPosition(uint xPos, uint yPos)
{
	int index = 8 * yPos + xPos;

	int boardLocation = boardIndices[index];

	if (boardLocation == -1)
	{
		return FREEWHITE;
	}

	long long offset = (1LL << boardLocation);

	if (m_board.m_WhitePieces & offset)
	{
		return WHITE;
	}

	if (m_board.m_BlackPieces & offset)
	{
		return BLACK;
	}

	return FREEBLACK;
}

void Checkers::DrawBoard()
{
	WORD fgcolour;
	WORD bgcolour;

	using namespace std;

	for (int y = 7; y >= 0; y--)
	{
		fgcolour = ConsoleColours::LIGHT_WHITE;
		bgcolour = ConsoleColours::DARK_BLACK;
		SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
		cout << y << " ";
		for (int x = 0; x < 8; x++)
		{
			Colour drawOutput = GetPosition(x, y);
			switch (drawOutput)
			{
			case BLACK:
				fgcolour = ConsoleColours::LIGHT_RED;
				bgcolour = ConsoleColours::DARK_BLACK;
				SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
				cout << char(169);
				break;
			case WHITE:
				fgcolour = ConsoleColours::LIGHT_WHITE;
				bgcolour = ConsoleColours::DARK_BLACK;
				SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
				cout << char(169);
				break;
			case FREEWHITE:
				fgcolour = ConsoleColours::LIGHT_RED;
				bgcolour = ConsoleColours::LIGHT_WHITE;
				SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
				cout << " ";
				break;
			case FREEBLACK:
				fgcolour = ConsoleColours::LIGHT_RED;
				bgcolour = ConsoleColours::DARK_BLACK;
				SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
				cout << " ";
				break;
			default:
				break;
			}
		}
		cout << endl;
	}
	fgcolour = ConsoleColours::LIGHT_WHITE;
	bgcolour = ConsoleColours::DARK_BLACK;
	SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
	cout << endl << "  01234567" << endl;
}

WORD Checkers::generateConsoleColour(WORD fgColour, WORD bgColour)
{
	return fgColour + (bgColour * 16);
}