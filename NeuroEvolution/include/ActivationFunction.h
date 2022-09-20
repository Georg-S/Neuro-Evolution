#pragma once
#include <math.h>
#include <functional>

namespace nev
{
	enum class af
	{
		relu, steepenedSigmoid
	};

	using activationFunction = std::function<double(double input)>;

	class Activation
	{
	public:
		static activationFunction getFunction(nev::af af);

	private:
		static double relu(double input);
		static double steepenedSigmoid(double input);
	};
}