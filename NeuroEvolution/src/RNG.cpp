#include "RNG.h"


int RNG::getRandomIntBetween(const int &from, const int &to)
{
	if (to <= from)
		return from;

	int buf = (to - from) + 1;
	int randomNumber = (rand() % buf) + from;

	return randomNumber;
}

float RNG::getRandomFloatBetween(const int& from, const int& to)
{
	if (from == to)
		return from;

	float random = (float)rand() / (float)RAND_MAX;
	float buf = (float)(to - from);
	float r = random * buf;

	return from + r;
}

float RNG::getRandomFloatBetween(const float& from, const float& to)
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

double RNG::getRandomDoubleBetween(const double& from, const double& to)
{
	if (from == to)
		return from;

	double random = (double)rand() / (double)RAND_MAX;
	double buf = (double)(to - from);
	double r = random * buf;

	return (double)from + r;
}
