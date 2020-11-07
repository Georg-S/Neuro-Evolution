#include "RNG.h"


RNG::RNG()
{
}


RNG::~RNG()
{
}

int RNG::getRandomIntBetween(int from, int to)
{
	if (from == to || to < from)
		return from;

	int buf = (to - from) + 1;
	int randomNumber = (rand() % buf) + from;

	return randomNumber;
}

float RNG::getRandomFloatBetween(int from, int to)
{
	if (from == to)
		return from;

	float random = (float)rand() / (float)RAND_MAX;
	float buf = (float)(to - from);
	float r = random * buf;

	return from + r;
}

float RNG::getRandomFloatBetween(float from, float to)
{
	if (from == to)
		return from;

	float random = (float)rand() / (float)RAND_MAX;
	float buf = (float)(to - from);
	float r = random * buf;

	return from + r;
}

float RNG::getRandomFloatBetween0and1()
{
	return (float)rand() / (float)RAND_MAX;
}
