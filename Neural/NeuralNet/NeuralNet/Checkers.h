#pragma once

typedef unsigned int uint;

const int boardIndices[64] = {	5,	-1,	 6, -1,  7, -1,  8, -1,
								-1, 10, -1, 11, -1, 12, -1, 13,
								14, -1, 15, -1, 16, -1, 17, -1,
								-1, 19, -1, 20, -1, 21, -1, 22,
								23, -1, 24, -1, 25, -1, 26, -1,
								-1, 28, -1, 29, -1, 30, -1, 31,
								32, -1, 33, -1, 34, -1, 35, -1,
								-1, 37, -1, 38, -1, 39, -1, 40
								};

struct Board
{
	long long m_WhitePieces; // 64 bits per piece
	long long m_BlackPieces;
	long long m_Kings;
};

enum Colour
{
	FREEWHITE = 0,
	FREEBLACK = 1,
	BLACK = 2,
	WHITE = 3,
	INVALID = -1
};

class Checkers
{

private:
	Board m_board;
	


public:
	Checkers();
	
	bool SetPosition(Colour inputColour, uint xPos, uint yPos);
	Colour GetPosition(uint xPos, uint yPos);
	void DrawBoard();
};