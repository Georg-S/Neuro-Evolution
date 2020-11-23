#pragma once
#include <math.h>

namespace nev {
	enum class af_Identifier {
		relu, steepenedSigmoid
	};

	static double relu(const double& input) {
		return std::max(0., input);
	}

	static double steepenedSigmoid(const double& input)
	{
		return 1.f / (1.f + exp(-4.9 * (input)));
	}
}