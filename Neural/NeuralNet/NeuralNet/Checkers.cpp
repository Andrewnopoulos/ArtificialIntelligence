#include "Checkers.h"
#include <iostream>
#include <bitset>
#include <algorithm>

Checkers::Checkers()
{
	ResetBoard();
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
	}
	else if (inputColour == Colour::BLACK)
	{
		m_board.m_BlackPieces |= offset;
	}

	return true;
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

using namespace std;

void Checkers::DrawBoard()
{
	for (int y = 7; y >= 0; y--)
	{
		for (int x = 0; x < 8; x++)
		{
			Colour drawOutput = GetPosition(x, y);
			switch (drawOutput)
			{
			case BLACK:
				cout << drawOutput;
				break;
			case WHITE:
				cout << drawOutput;
				break;
			case FREEWHITE:
				cout << drawOutput;
				break;
			case FREEBLACK:
				cout << drawOutput;
				break;
			default:
				break;
			}
			cout << " ";
		}
		cout << endl << endl;
	}

	//bitset<64> x(m_board.m_WhitePieces);
	//bitset<64> y(m_board.m_BlackPieces);
	//cout << x << endl;
	//cout << y << endl;
}