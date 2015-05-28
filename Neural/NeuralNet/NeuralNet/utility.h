#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>

// constants
const float NODEBIAS = -1.0f;
const float MAXPERTURBATION = 0.3f;
const float ACTIVATIONRESPONSE = 1.0f;
const uint NUMCOPIESELITE = 1;
const uint NUMELITE = 4;
const uint DEFAULTBOARDSIZE = 3;
const uint DEFAULTNODELAYERS = 1;
const uint DEFAULTLAYERSIZE = 15;
const float DEFAULTMUTATIONRATE = 0.07f;
const float DEFAULTCROSSOVERRATE = 0.7f;
const uint DEFAULTPOPULATIONSIZE = 30;
const uint DEFAULTITERATIONS = 1000;

const uint MONTECARLOPLAYOUTS = 30;


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

inline unsigned int BitCount(long long input)
{
	unsigned int count = 0;

	while (input > 0)
	{
		if ((input & 1LL) == 1)
			count++;

		input >>= 1LL;
	}

	return count;
}


#endif