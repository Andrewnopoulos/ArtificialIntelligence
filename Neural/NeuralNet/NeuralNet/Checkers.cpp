#include "Checkers.h"
#include <iostream>
#include <bitset>

Checkers::Checkers()
{
	memset(&m_board, 0, sizeof(Board));

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			SetPosition(FREEBLACK, i, j);
		}
	}
}


bool Checkers::SetPosition(Colour inputColour, uint xPos, uint yPos)
{

	int index = 8 * yPos + xPos;

	int boardLocation = boardIndices[index];

	if (boardLocation == -1)
	{
		return false;
	}

	long long offset = (1 << boardLocation);

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

	long long offset = (1 << boardLocation);

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
		for (int x = 7; x >= 0; x--)
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

	bitset<64> x(m_board.m_WhitePieces);
	bitset<64> y(m_board.m_BlackPieces);
	cout << x << endl;
	cout << y << endl;
}