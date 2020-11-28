#include "RNG.h"


int nev::RNG::getRandomIntBetween(int from, int to)
{
	if (from >= to)
		return from;

	int buf = (to - from) + 1;
	int randomNumber = (rand() % buf) + from;

	return randomNumber;
}

float nev::RNG::getRandomFloatBetween(int from, int to)
{
	if (from >= to)
		return from;

	float random = (float)rand() / (float)RAND_MAX;
	float buf = (float)(to - from);
	float r = random * buf;

	return from + r;
}

float nev::RNG::getRandomFloatBetween(float from, float to)
{
	if (from >= to)
		return from;

	float random = (float)rand() / (float)RAND_MAX;
	float buf = (float)(to - from);
	float r = random * buf;

	return from + r;
}

float nev::RNG::getRandomFloatBetween0and1()
{
	return (float)rand() / (float)RAND_MAX;
}

double nev::RNG::getRandomDoubleBetween(double from, double to)
{
	if (from >= to)
		return from;

	double random = (double)rand() / (double)RAND_MAX;
	double buf = (double)(to - from);
	double r = random * buf;

	return (double)from + r;
}
