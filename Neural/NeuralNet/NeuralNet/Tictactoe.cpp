#include "Tictactoe.h"
#include <iostream>

Tictactoe::Tictactoe(uint dimensions)
{
	resetBoard(dimensions);
}

bool Tictactoe::makeMove(uint xPos, uint yPos, int value)
{
	if (xPos >= m_dimensions || yPos >= m_dimensions)
	{
		return false;
	}

	if (m_board[xPos + m_dimensions * yPos] != 0)
	{
		return false;
	}
	else
	{
		m_board[xPos + m_dimensions * yPos] = value;
		return true;
	}
}

bool Tictactoe::makeMove(uint position, int value)
{
	if (position >= m_dimensions * m_dimensions)
	{
		return false;
	}

	if (m_board[position] != 0)
	{
		return false;
	}
	else
	{
		m_board[position] = value;
		return true;
	}
}

void Tictactoe::resetBoard(uint dimensions)
{
	m_dimensions = dimensions;
	m_board = new int[m_dimensions * m_dimensions];
	memset(m_board, 0, sizeof(int) * dimensions * dimensions);
}

Tictactoe::~Tictactoe()
{
	delete m_board;
}

int Tictactoe::checkCols()
{

	for (uint i = 0; i < m_dimensions; i++)
	{
		int colOutput = 0;
		for (uint j = 0; j < m_dimensions; j++)
		{
			colOutput += m_board[i + j * m_dimensions];
		}
		if (abs(colOutput) == m_dimensions)
		{
			return colOutput / abs(colOutput);
		}
	}
	return 0;
}

int Tictactoe::checkRows()
{
	for (uint i = 0; i < m_dimensions; i++)
	{
		int rowOutput = 0;
		for (uint j = 0; j < m_dimensions; j++)
		{
			rowOutput += m_board[j + i * m_dimensions];
		}
		if (abs(rowOutput) == m_dimensions)
		{
			return rowOutput / abs(rowOutput);
		}
	}
	return 0;
}

int Tictactoe::checkDiags()
{
	int diagOutput = 0;
	for (uint i = 0; i < m_dimensions; i++)
	{
		diagOutput += m_board[i + i * m_dimensions];
	}
	if (abs(diagOutput) == m_dimensions)
	{
		return diagOutput / abs(diagOutput);
	}

	diagOutput = 0;

	for (uint i = 0; i < m_dimensions; i++)
	{
		diagOutput += m_board[(m_dimensions-1 - i) + i * m_dimensions];
	}
	if (abs(diagOutput) == m_dimensions)
	{
		return diagOutput / abs(diagOutput);
	}

	return 0;
}

int Tictactoe::winnerIs()
{
	int colWinner = checkCols();
	if (colWinner)
	{
		return colWinner;
	}

	int rowWinner = checkRows();
	if (rowWinner)
	{
		return rowWinner;
	}

	int diagWinner = checkDiags();
	if (diagWinner)
	{
		return diagWinner;
	}

	return 0;
}

void Tictactoe::printBoard()
{
	using namespace std;
	for (uint i = 0; i < m_dimensions; i++)
	{
		cout << " | ";
		for (uint j = 0; j < m_dimensions; j++)
		{
			cout << m_board[i + j * m_dimensions] << " | ";
		}
		cout << endl << "  ";
		for (int k = 0; k < m_dimensions; k++)
		{
			cout << "___ ";
		}
		cout << endl;
		
	}
}