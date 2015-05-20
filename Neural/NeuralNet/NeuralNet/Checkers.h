#pragma once

#include <vector>
#include <Windows.h>

typedef unsigned int uint;

namespace CheckersGame
{

	const int boardIndices[64] = {	 5, -1,  6, -1,  7, -1,  8, -1,
									-1, 10, -1, 11, -1, 12, -1, 13,
									14, -1, 15, -1, 16, -1, 17, -1,
									-1, 19, -1, 20, -1, 21, -1, 22,
									23, -1, 24, -1, 25, -1, 26, -1,
									-1, 28, -1, 29, -1, 30, -1, 31,
									32, -1, 33, -1, 34, -1, 35, -1,
									-1, 37, -1, 38, -1, 39, -1, 40
	};

	const std::vector<int> validMoves = { 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17,
		19, 20, 21, 22, 23, 24, 25, 26, 28, 29, 30, 31, 32, 33, 34, 35, 37, 38, 39, 40 };

	enum ConsoleColours : WORD
	{
		DARK_BLACK = 0,
		DARK_BLUE = 1,
		DARK_GREEN = 2,
		AQUA = 3,
		DARK_RED = 4,
		DARK_PURPLE = 5,
		DARK_YELLOW = 6,
		DARK_WHITE = 7,
		GREY = 8,
		MED_BLUE = 9,
		LIGHT_GREEN = 10,
		LIGHT_BLUE = 11,
		LIGHT_RED = 12,
		LIGHT_PURPLE = 13,
		LIGHT_YELLOW = 14,
		LIGHT_WHITE = 15
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

	enum Direction
	{
		UPLEFT = 4,
		UPRIGHT = 5,
		DOWNLEFT = -5,
		DOWNRIGHT = -4
	};

	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	class Checkers
	{

	private:
		Board m_board;

		static WORD generateConsoleColour(WORD fgColour, WORD bgColour);

	public:
		Checkers();
		Checkers(Board a_inputBoard);

		bool SetPosition(Colour inputColour, uint xPos, uint yPos);
		Colour GetPosition(uint xPos, uint yPos);
		Colour GetPosition(long long boardLocation);

		void SetBoardState(Board a_inputBoard);

		void ResetBoard();

		bool isValidMove(uint xPos, uint yPos, Direction a_direction);
		bool isValidJump(uint xPos, uint yPos, Direction a_direction);

		static bool isValidMove(Board& a_board, uint xPos, uint yPos, Direction a_direction);
		static bool isValidJump(Board& a_board, uint xPos, uint yPos, Direction a_direction);

		static Colour GetPosition(Board& a_board, uint xPos, uint yPos);
		static Colour GetPosition(Board& a_board, long long boardLocation);

		static bool SetPosition(Board& a_board, Colour inputColour, uint xPos, uint yPos);

		void DrawBoard();
		static void DrawBoard(Board& a_board);

		Colour DrawBoard(uint xPos, uint yPos, bool showMoves = true, bool showJumps = true);
		static Colour DrawBoard(Board& a_board, uint xPos, uint yPos, bool showMoves = true, bool showJumps = true);

		void Move(uint xPos, uint yPos, Direction a_direction);
		static void Move(Board& a_board, uint xPos, uint yPos, Direction a_direction);

		void Jump(uint xPos, uint yPos, Direction a_direction);
		static void Jump(Board& a_board, uint xPos, uint yPos, Direction a_direction);

		inline Board GetBoardState()
		{
			return m_board;
		}
	};
};