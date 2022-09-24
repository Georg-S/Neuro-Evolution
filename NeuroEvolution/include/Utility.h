#pragma once
#include <random>
#include <iostream>
#include <thread>
#include <cassert>
#include <algorithm>

namespace nev
{
	int getRandomInt(int min, int max);
	double getRandomDouble(double min, double max);
	bool getRandomBool();
	std::mt19937& getRNG();

	/// Returns a random index of a vector
	template <typename T>
	int getRandomIndex(const std::vector<T>& vec)
	{
		assert(!vec.empty());
		return getRandomInt(0, vec.size() - 1);
	}

	template <typename T>
	T getRandomElement(const std::vector<T>& vec)
	{
		return vec[getRandomIndex(vec)];
	}
}
