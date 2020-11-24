#include "ActivationFunction.h"

double nev::Activation::steepenedSigmoid(const double& input)
{
	return 1.f / (1.f + exp(-4.9 * (input)));
}


double nev::Activation::relu(const double& input) {
	return std::max(0., input);
}

std::function<double(const double& input)> nev::Activation::getFunction(const nev::af& af) {
	return arr[(int)af];
}

