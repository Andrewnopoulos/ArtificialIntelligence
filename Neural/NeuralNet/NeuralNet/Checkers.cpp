#include "Checkers.h"
#include <iostream>

Checkers::Checkers()
{
	m_board = new int*[8];
	for (int i = 0; i < 8; i++)
	{
		m_board[i] = new int[8];
	}

	memset(m_board, 0, sizeof(int)* 64);
}