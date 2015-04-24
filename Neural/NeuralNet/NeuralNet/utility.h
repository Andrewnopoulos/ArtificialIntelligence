#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>

// constants
const float NODEBIAS = -1.0f;
const float MAXPERTURBATION = 0.3f;
const float ACTIVATIONRESPONSE = 1.0f;
const uint NUMCOPIESELITE = 1;
const uint NUMELITE = 4;


//returns a random integer between x and y
inline int	  RandInt(int x, int y) { return rand() % (y - x + 1) + x; }

//returns a random float between zero and 1
inline double RandFloat()		   { return (rand()) / (RAND_MAX + 1.0); }

//returns a random bool
inline bool   RandBool()
{
	if (RandInt(0, 1)) return true;

	else return false;
}

//returns a random float in the range -1 < n < 1
inline double RandomClamped()	   { return RandFloat() - RandFloat(); }



#endif