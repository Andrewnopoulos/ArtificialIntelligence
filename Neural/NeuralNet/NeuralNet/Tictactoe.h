#pragma once

typedef unsigned int uint;

class Tictactoe
{
private:
	int* m_board;
	uint m_dimensions;
	bool victoryYet();
	int checkRows();
	int checkCols();
	int checkDiags();

public:
	Tictactoe(uint dimensions);
	~Tictactoe();
	int winnerIs();
	
	inline uint getDimensions() const {	return m_dimensions; }

	inline int* getBoard() const {return m_board;}

	void printBoard();

	// returns true if move successfully made
	bool makeMove(uint posX, uint posY, int value);

	void resetBoard(uint dimensions);

};