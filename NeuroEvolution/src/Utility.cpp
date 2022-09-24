#include "Utility.h"

static std::mt19937 createRNG()
{
	auto id = std::this_thread::get_id();
	std::hash<std::thread::id> hasher;

	auto seed = hasher(id);
	seed += time(NULL);

	return std::mt19937(seed);
}

thread_local std::mt19937 generator = createRNG();

int nev::getRandomInt(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);

	return distribution(generator);
}

double nev::getRandomDouble(double min, double max)
{
	std::uniform_real_distribution<double> distribution(min, max);

	return distribution(generator);
}

bool nev::getRandomBool()
{
	return getRandomInt(0, 1) == 0;
}

std::mt19937& nev::getRNG()
{
	return generator;
}
