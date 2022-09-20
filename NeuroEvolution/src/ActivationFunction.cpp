#include "ActivationFunction.h"

double nev::Activation::steepenedSigmoid(double input)
{
	return 1.f / (1.f + exp(-4.9 * (input)));
}

double nev::Activation::relu(double input)
{
	return std::max(0., input);
}

nev::activationFunction nev::Activation::getFunction(nev::af af)
{
	static const activationFunction arr[]{ relu, steepenedSigmoid };

	return arr[static_cast<int>(af)];
}
