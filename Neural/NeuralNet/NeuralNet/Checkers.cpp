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

	//for (int i = 0; i < validMoves.size(); i++)
	//{
	//	validBitField += 1LL << validMoves[i];
	//}
}

Checkers::Checkers(Board a_inputBoard)
{
	SetBoardState(a_inputBoard);
}

void Checkers::SetBoardState(Board a_inputBoard)
{
	m_board = a_inputBoard;
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

// check if it's a king. If it's a king, it's all good. Otherwise, check direction based on colour
bool Checkers::CheckMoveDirection(Board& a_board, uint xPos, uint yPos, Direction a_direction)
{
	uint index = 8 * yPos + xPos;
	uint pieceLocation = boardIndices[index];

	long long bitmask = (1LL << pieceLocation);

	// if it's a king, any direction move is valid
	if (bitmask & a_board.m_Kings)
	{
		return true;
	}

	Colour pieceColour = GetPosition(a_board, xPos, yPos);

	switch (pieceColour)
	{
	case WHITE:
		if (a_direction > 0) // if it's white, only upwards moves are valid
		{
			return true;
		}
		break;
	case BLACK:
		if (a_direction < 0) // if it's black, only downwards moves are valid
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

// BUG - returns valid jump if jumping outside the map
bool Checkers::isValidMove(Board& a_board, uint xPos, uint yPos, Direction a_direction)
{
	uint index = 8 * yPos + xPos;

	// index is now the desired destination bit
	uint originalPiece = boardIndices[index];

	uint offset = boardIndices[index] + a_direction;

	Colour current = GetPosition(a_board, xPos, yPos);

	if (current == FREEBLACK || current == FREEWHITE)
	{
		return false;
	}

	// if it's an invalid move direction based off colour/kingship
	if (!CheckMoveDirection(a_board, xPos, yPos, a_direction))
	{
		return false;
	}

	if (std::find(validMoves.begin(), validMoves.end(), offset) != validMoves.end()) // if the move is at least on the board
	{
		long long bitmask = (1LL << offset);

		// if there's a black piece already on the spot
		if (a_board.m_BlackPieces & bitmask)
		{
			return false;
		}

		if (a_board.m_WhitePieces & bitmask)
		{
			return false;
		}

		return true;
	}
	return false;
}

bool Checkers::isValidJump(Board& a_board, uint xPos, uint yPos, Direction a_direction)
{
	uint index = 8 * yPos + xPos;

	uint enemyOffset = boardIndices[index] + a_direction;

	uint destinationOffset = boardIndices[index] + 2 * a_direction;

	Colour currentPosition = GetPosition(a_board, xPos, yPos);

	Colour enemyPosition = GetPosition(a_board, enemyOffset);

	if (currentPosition == FREEBLACK || currentPosition == FREEWHITE)
	{
		return false;
	}

	// if it's an invalid move direction based off colour/kingship
	if (!CheckMoveDirection(a_board, xPos, yPos, a_direction))
	{
		return false;
	}

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
	if (a_board.m_BlackPieces & bitmask)
	{
		return false;
	}
	if (a_board.m_WhitePieces & bitmask)
	{
		return false;
	}

	return true;

	// check if the enemy colour is opposite to yours - done
	// check if destination square is empty and valid - done
}

std::vector<Movement> Checkers::GetValidMoves(Colour a_playerTurn)
{
	return GetValidMoves(m_board, a_playerTurn);
}

std::vector<Movement> Checkers::GetValidMoves(Board& a_board, Colour a_playerTurn)
{
	std::vector<Movement> possibleMoves;

	bool canJump = ValidJumpExists(a_board, a_playerTurn);

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (GetPosition(a_board, x, y) == a_playerTurn)
			{
				if (isValidMove(a_board, x, y, UPLEFT) && !canJump)
				{
					possibleMoves.push_back(Movement(x, y, UPLEFT, MoveType::MOVE));
				}
				if (isValidMove(a_board, x, y, UPRIGHT) && !canJump)
				{
					possibleMoves.push_back(Movement(x, y, UPRIGHT, MoveType::MOVE));
				}
				if (isValidMove(a_board, x, y, DOWNLEFT) && !canJump)
				{
					possibleMoves.push_back(Movement(x, y, DOWNLEFT, MoveType::MOVE));
				}
				if (isValidMove(a_board, x, y, DOWNRIGHT) && !canJump)
				{
					possibleMoves.push_back(Movement(x, y, DOWNRIGHT, MoveType::MOVE));
				}
				if (isValidJump(a_board, x, y, UPLEFT))
				{
					possibleMoves.push_back(Movement(x, y, UPLEFT, MoveType::JUMP));
				}
				if (isValidJump(a_board, x, y, UPRIGHT))
				{
					possibleMoves.push_back(Movement(x, y, UPRIGHT, MoveType::JUMP));
				}
				if (isValidJump(a_board, x, y, DOWNLEFT))
				{
					possibleMoves.push_back(Movement(x, y, DOWNLEFT, MoveType::JUMP));
				}
				if (isValidJump(a_board, x, y, DOWNRIGHT))
				{
					possibleMoves.push_back(Movement(x, y, DOWNRIGHT, MoveType::JUMP));
				}
			}
		}
	}

	return possibleMoves;
}

bool Checkers::ValidJumpExists(Colour a_colour)
{
	return ValidJumpExists(m_board, a_colour);
}

bool Checkers::ValidJumpExists(Board& a_board, Colour a_colour)
{
	long long output = 0;
	long long enemy = 0;
	long long urJumps;
	long long ulJumps;
	long long drJumps;
	long long dlJumps;
	long long king = a_board.m_Kings;

	int dl = -1 * DOWNLEFT;
	int dr = -1 * DOWNRIGHT;

	switch (a_colour)
	{
	case WHITE:
		output = a_board.m_WhitePieces;
		enemy = a_board.m_BlackPieces;
		urJumps = (((output << UPRIGHT) & enemy) << UPRIGHT) & (~(output | enemy)) & validBitField;
		ulJumps = (((output << UPLEFT) & enemy) << UPLEFT) & (~(output | enemy)) & validBitField;
		drJumps = ((((output & king) >> dr) & enemy) >> dr) & (~(output | enemy)) & validBitField;
		dlJumps = ((((output & king) >> dl) & enemy) >> dl) & (~(output | enemy)) & validBitField;
		break;
	case BLACK:
		output = a_board.m_BlackPieces;
		enemy = a_board.m_WhitePieces;
		urJumps = ((((output & king) << UPRIGHT) & enemy) << UPRIGHT) & (~(output | enemy)) & validBitField;
		ulJumps = ((((output & king) << UPLEFT) & enemy) << UPLEFT) & (~(output | enemy)) & validBitField;
		drJumps = (((output >> dr) & enemy) >> dr) & (~(output | enemy)) & validBitField;
		dlJumps = (((output >> dl) & enemy) >> dl) & (~(output | enemy)) & validBitField;
		break;
	default:
		break;
	}

	return urJumps + ulJumps + drJumps + dlJumps;
}

/*

Currently checking if the desired move is to a square that's on the board and it isn't occupied already

TODO
Check if you can do a jump
check direction based on colour/kingship

*/
bool Checkers::isValidMove(uint xPos, uint yPos, Direction a_direction)
{
	return isValidMove(m_board, xPos, yPos, a_direction);
}

bool Checkers::isValidJump(uint xPos, uint yPos, Direction a_direction)
{
	return isValidJump(m_board, xPos, yPos, a_direction);
}

bool Checkers::SetPosition(Colour inputColour, uint xPos, uint yPos)
{
	return SetPosition(m_board, inputColour, xPos, yPos);
}

bool Checkers::SetPosition(Board& a_board, Colour inputColour, uint xPos, uint yPos)
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
		a_board.m_WhitePieces |= offset;
		a_board.m_BlackPieces &= (~offset);
	}
	else if (inputColour == Colour::BLACK)
	{
		a_board.m_BlackPieces |= offset;
		a_board.m_WhitePieces &= (~offset);
	}

	return true;
}

Colour Checkers::GetPosition(long long boardLocation)
{
	return GetPosition(m_board, boardLocation);
}

Colour Checkers::GetPosition(uint xPos, uint yPos)
{
	return GetPosition(m_board, xPos, yPos);
}

Colour Checkers::GetPosition(Board& a_board, long long boardLocation)
{
	long long offset = (1LL << boardLocation);

	if (a_board.m_WhitePieces & offset)
	{
		return WHITE;
	}

	if (a_board.m_BlackPieces & offset)
	{
		return BLACK;
	}

	return FREEBLACK;
}

Colour Checkers::GetPosition(Board& a_board, uint xPos, uint yPos)
{
	int index = 8 * yPos + xPos;

	int boardLocation = boardIndices[index];

	if (boardLocation == -1)
	{
		return FREEWHITE;
	}

	long long offset = (1LL << boardLocation);

	if (a_board.m_WhitePieces & offset)
	{
		return WHITE;
	}

	if (a_board.m_BlackPieces & offset)
	{
		return BLACK;
	}

	return FREEBLACK;
}

void Checkers::King(int boardLocation)
{
	King(m_board, boardLocation);
}

void Checkers::King(Board& a_board, int boardLocation)
{
	long long bitmask = 1LL << boardLocation;

	a_board.m_Kings |= bitmask; // activate kingship for piece
}

bool Checkers::IsKing(uint xPos, uint yPos)
{
	return IsKing(m_board, xPos, yPos);
}

bool Checkers::IsKing(Board& a_board, uint xPos, uint yPos)
{
	uint index = 8 * yPos + xPos;

	int boardLocation = boardIndices[index];

	return a_board.m_Kings & (1LL << boardLocation);
}

Colour Checkers::GameOver()
{
	return GameOver(m_board);
}

Colour Checkers::GameOver(Board& a_board)
{
	if (a_board.m_BlackPieces == 0)
	{
		return WHITE;
	}

	if (a_board.m_WhitePieces == 0)
	{
		return BLACK;
	}

	return FREEWHITE;
}

void Checkers::Move(uint xPos, uint yPos, Direction a_direction)
{
	Move(m_board, xPos, yPos, a_direction);
}

void Checkers::Move(Board& a_board, uint xPos, uint yPos, Direction a_direction)
{
	uint index = 8 * yPos + xPos;

	int boardLocation = boardIndices[index];
	int destLocation = boardIndices[index] + a_direction;

	long long currentOffset = (1LL << boardLocation);
	long long futureOffset = (1LL << destLocation);

	Colour piece = GetPosition(a_board, xPos, yPos);

	// if it's a king
	if (a_board.m_Kings & currentOffset)
	{
		a_board.m_Kings |= futureOffset; // put a king on future spot
		a_board.m_Kings &= (~currentOffset); // remove old kingship
	}

	switch (piece)
	{
	case WHITE:
		a_board.m_WhitePieces |= futureOffset; // set next piece as white
		a_board.m_WhitePieces &= (~currentOffset); // get rid of previous white piece
		if (yPos == 6)
		{
			King(a_board, destLocation);
		}
		break;
	case BLACK:
		a_board.m_BlackPieces |= futureOffset; // set next piece as black
		a_board.m_BlackPieces &= (~currentOffset); // get rid of previous black piece
		if (yPos == 1)
		{
			King(a_board, destLocation);
		}
		break;
	default:
		break;
	}
}

void Checkers::Jump(uint xPos, uint yPos, Direction a_direction)
{
	Jump(m_board, xPos, yPos, a_direction);
}

void Checkers::Jump(Board& a_board, uint xPos, uint yPos, Direction a_direction)
{
	uint index = 8 * yPos + xPos;

	int currentSquare = boardIndices[index];

	int enemyLocation = currentSquare + a_direction;

	int futureLocation = enemyLocation + a_direction;

	Colour currentPiece = GetPosition(a_board, xPos, yPos);

	long long currentOffset = 1LL << currentSquare;
	long long enemyOffset = 1LL << enemyLocation;
	long long futureOffset = 1LL << futureLocation;

	// if it's a king doing the jump
	if (a_board.m_Kings & currentOffset)
	{
		a_board.m_Kings |= futureOffset; // put a king on future spot
		a_board.m_Kings &= (~currentOffset); // remove old kingship
	}

	a_board.m_Kings &= (~enemyOffset); // delete enemy kingship

	switch (currentPiece)
	{
	case WHITE:
		a_board.m_WhitePieces |= futureOffset; // set the white piece at the future point
		a_board.m_WhitePieces &= (~currentOffset); // delete previous white piece location
		a_board.m_BlackPieces &= (~enemyOffset); // delete black piece that was jumped
		if (yPos == 5)
		{
			King(a_board, futureLocation);
		}
		break;
	case BLACK:
		a_board.m_BlackPieces |= futureOffset;
		a_board.m_BlackPieces &= (~currentOffset);
		a_board.m_WhitePieces &= (~enemyOffset);
		if (yPos == 2)
		{
			King(a_board, futureLocation);
		}
		break;
	default:
		break;
	}
}

Colour Checkers::DrawBoard(uint xPos, uint yPos, bool showMoves, bool showJumps)
{
	return DrawBoard(m_board, xPos, yPos, showMoves, showJumps);
}

Colour Checkers::DrawBoard(Board& a_board, uint xPos, uint yPos, bool showMoves, bool showJumps)
{
	WORD fgcolour;
	WORD bgcolour;

	Colour output = FREEBLACK;

	bool upleft;
	bool upright;
	bool downleft;
	bool downright;

	if (showMoves)
	{
		upleft = isValidMove(a_board, xPos, yPos, Direction::UPLEFT);
		upright = isValidMove(a_board, xPos, yPos, Direction::UPRIGHT);
		downleft = isValidMove(a_board, xPos, yPos, Direction::DOWNLEFT);
		downright = isValidMove(a_board, xPos, yPos, Direction::DOWNRIGHT);
	}
	else
	{
		upleft = false;
		upright = false;
		downleft = false;
		downright = false;
	}

	bool uljump;
	bool urjump;
	bool dljump;
	bool drjump;

	if (showJumps)
	{
		uljump = isValidJump(a_board, xPos, yPos, Direction::UPLEFT);
		urjump = isValidJump(a_board, xPos, yPos, Direction::UPRIGHT);
		dljump = isValidJump(a_board, xPos, yPos, Direction::DOWNLEFT);
		drjump = isValidJump(a_board, xPos, yPos, Direction::DOWNRIGHT);
	}
	else
	{
		uljump = false;
		urjump = false;
		dljump = false;
		drjump = false;
	}

	// move destinations
	int ulX = xPos - 1;
	int ulY = yPos + 1;
	int urX = xPos + 1;
	int urY = yPos + 1;
	int dlX = xPos - 1;
	int dlY = yPos - 1;
	int drX = xPos + 1;
	int drY = yPos - 1;

	// jump destinations
	int uljumpX = xPos - 2;
	int uljumpY = yPos + 2;
	int urjumpX = xPos + 2;
	int urjumpY = yPos + 2;
	int dljumpX = xPos - 2;
	int dljumpY = yPos - 2;
	int drjumpX = xPos + 2;
	int drjumpY = yPos - 2;

	using namespace std;

	for (int y = 7; y >= 0; y--)
	{
		fgcolour = ConsoleColours::LIGHT_WHITE;
		bgcolour = ConsoleColours::DARK_BLACK;
		SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
		cout << y << " ";
		for (int x = 0; x < 8; x++)
		{
			bool selection = false;
			bool jumpspace = false;

			if (x == xPos && y == yPos)
			{
				selection = true;
			}

			bool prediction = false;

			if (x == ulX && y == ulY && upleft)
			{
				prediction = true;
			}
			if (x == urX && y == urY && upright)
			{
				prediction = true;
			}
			if (x == dlX && y == dlY && downleft)
			{
				prediction = true;
			}
			if (x == drX && y == drY && downright)
			{
				prediction = true;
			}

			if (x == uljumpX && y == uljumpY && uljump)
			{
				jumpspace = true;
			}
			if (x == urjumpX && y == urjumpY && urjump)
			{
				jumpspace = true;
			}
			if (x == dljumpX && y == dljumpY && dljump)
			{
				jumpspace = true;
			}
			if (x == drjumpX && y == drjumpY && drjump)
			{
				jumpspace = true;
			}

			Colour drawOutput = GetPosition(a_board, x, y);
			switch (drawOutput)
			{
			case BLACK:
				fgcolour = ConsoleColours::LIGHT_RED;
				bgcolour = ConsoleColours::DARK_BLACK;
				if (selection)
				{
					bgcolour = ConsoleColours::LIGHT_GREEN;
					output = BLACK;
				}
				if (prediction)
				{
					bgcolour = ConsoleColours::AQUA;
				}
				if (jumpspace)
				{
					bgcolour = ConsoleColours::DARK_PURPLE;
				}
				SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
				if (IsKing(a_board, x, y))
				{
					cout << "@";
				}
				else
				{
					cout << char(169);
				}
				break;
			case WHITE:
				fgcolour = ConsoleColours::LIGHT_WHITE;
				bgcolour = ConsoleColours::DARK_BLACK;
				if (selection)
				{
					bgcolour = ConsoleColours::LIGHT_GREEN;
					output = WHITE;
				}
				if (prediction)
				{
					bgcolour = ConsoleColours::AQUA;
				}
				if (jumpspace)
				{
					bgcolour = ConsoleColours::DARK_PURPLE;
				}
				SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
				if (IsKing(a_board, x, y))
				{
					cout << "@";
				}
				else
				{
					cout << char(169);
				}
				break;
			case FREEWHITE:
				fgcolour = ConsoleColours::LIGHT_RED;
				bgcolour = ConsoleColours::LIGHT_WHITE;
				if (selection)
				{
					bgcolour = ConsoleColours::LIGHT_GREEN;
					output = FREEWHITE;
				}
				if (prediction)
				{
					bgcolour = ConsoleColours::AQUA;
				}
				if (jumpspace)
				{
					bgcolour = ConsoleColours::DARK_PURPLE;
				}
				SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
				cout << " ";
				break;
			case FREEBLACK:
				fgcolour = ConsoleColours::LIGHT_RED;
				bgcolour = ConsoleColours::DARK_BLACK;
				if (selection)
				{
					bgcolour = ConsoleColours::LIGHT_GREEN;
					output = FREEBLACK;
				}
				if (prediction)
				{
					bgcolour = ConsoleColours::AQUA;
				}
				if (jumpspace)
				{
					bgcolour = ConsoleColours::DARK_PURPLE;
				}
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

	cout << "White can jump: " << ValidJumpExists(a_board, WHITE) << endl;

	cout << "Black can jump: " << ValidJumpExists(a_board, BLACK) << endl;

	return output;
}

void Checkers::DrawBoard()
{
	DrawBoard(m_board);
}

void Checkers::DrawBoard(Board& a_board)
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
			Colour drawOutput = GetPosition(a_board, x, y);
			switch (drawOutput)
			{
			case BLACK:
				fgcolour = ConsoleColours::LIGHT_RED;
				bgcolour = ConsoleColours::DARK_BLACK;
				SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
				if (IsKing(a_board, x, y))
				{
					cout << "@";
				}
				else
				{
					cout << char(169);
				}
				break;
			case WHITE:
				fgcolour = ConsoleColours::LIGHT_WHITE;
				bgcolour = ConsoleColours::DARK_BLACK;
				SetConsoleTextAttribute(hConsole, generateConsoleColour(fgcolour, bgcolour));
				if (IsKing(a_board, x, y))
				{
					cout << "@";
				}
				else
				{
					cout << char(169);
				}
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