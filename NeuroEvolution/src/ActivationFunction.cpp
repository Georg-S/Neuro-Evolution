#include "ActivationFunction.h"

double nev::Activation::steepenedSigmoid(double input)
{
	return 1.f / (1.f + exp(-4.9 * (input)));
}

double nev::Activation::relu(double input) {
	return std::max(0., input);
}

std::function<double(double input)> nev::Activation::getFunction(nev::af af) {
	return arr[(int)af];
}
