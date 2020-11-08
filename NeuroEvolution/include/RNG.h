#pragma once
#include <stdlib.h>

//RNG is short for Random Number Generator
class RNG
{
public:
	static int getRandomIntBetween(const int &from, const int &to);
	static float getRandomFloatBetween(const int &from, const int &to);
	static float getRandomFloatBetween(const float &from, const float &to);
	static float getRandomFloatBetween0and1();
};
