#pragma once
#include <vector>
#include "PhenotypeNeuron.h"


class Phenotype
{
public:
	Phenotype(const std::vector<PhenotypeNeuron*>& neurons, const int& maxDepth);
	~Phenotype();
	std::vector <double> calculateOutputSnapshot(const std::vector <double> &input);
	std::vector <double> calculateOutputActive(const std::vector <double> &input);
private:
	void setBias();
	void setInputs(const std::vector <double> &inputs);
	double steepenedSigmoidActivationFunction(const double &input);
	std::vector<double> updateOnce();
	void clearOutputOfNeurons();

	std::vector<PhenotypeNeuron*> neurons;
	int maxDepth;
};

