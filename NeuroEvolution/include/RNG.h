#pragma once
#include <stdlib.h>

namespace nev
{
	//RNG is short for Random Number Generator
	class RNG
	{
	public:
		static int getRandomVectorIndex(int vecSize);
		static int getRandomIntBetween(int from, int to);
		static float getRandomFloatBetween(int from, int to);
		static float getRandomFloatBetween(float from, float to);
		static float getRandomFloatBetween0and1();
		static double getRandomDoubleBetween(double from, double to);
	};
}
