#pragma once
#include <stdlib.h>

//RNG is short for Random Number Generator
class RNG
{
public:
	RNG();
	~RNG();
	static int getRandomIntBetween(int from, int to);
	static float getRandomFloatBetween(int from, int to);
	static float getRandomFloatBetween(float from, float to);
	static float getRandomFloatBetween0and1();
};
