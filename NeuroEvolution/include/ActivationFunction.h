#pragma once
#include <math.h>
#include <functional>

namespace nev {

	enum class af {
		relu, steepenedSigmoid
	};

	class Activation {
	public:
		static std::function<double(const double& input)> getFunction(const nev::af& af);

	private:
		static double relu(const double& input);
		static double steepenedSigmoid(const double& input);

		inline static const std::function<double(const double& input)> arr[]{relu, steepenedSigmoid};
	};
}