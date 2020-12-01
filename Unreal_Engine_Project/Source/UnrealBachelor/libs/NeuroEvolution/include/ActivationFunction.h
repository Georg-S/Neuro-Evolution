#pragma once
#include <math.h>
#include <functional>

namespace nev {

	enum class af {
		relu, steepenedSigmoid
	};

	class Activation {
	public:
		static std::function<double(double input)> getFunction(nev::af af);

	private:
		static double relu(double input);
		static double steepenedSigmoid(double input);
	};

}