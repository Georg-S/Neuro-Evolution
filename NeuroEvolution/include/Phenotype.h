#pragma once
#include "PhenotypeNeuron.h"
#include <vector>


class Phenotype
{
public:
	Phenotype(std::vector<PhenotypeNeuron*> neurons, int maxDepth);
	~Phenotype();
	std::vector <double> calculateOutputSnapshot(const std::vector <double> &input);
	std::vector <double> calculateOutputActive(const std::vector <double> &input);
private:
	void setBias();
	void setInputs(const std::vector <double> &inputs);
	double activationFunction(const double &input);
	std::vector<double> updateOnce();
	void clearOutputOfNeurons();

	std::vector<PhenotypeNeuron*> neurons;
	int maxDepth;
};

