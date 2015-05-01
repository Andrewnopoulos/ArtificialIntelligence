#pragma once

struct Board
{
	unsigned int m_WhitePieces : 5; // 32 bits
	unsigned int m_BlackPieces : 5;
	unsigned int m_WhiteKings : 5;
	unsigned int m_BlackKings : 5;
};

class Checkers
{

private:
	Board m_board;
	


public:
	Checkers();


};