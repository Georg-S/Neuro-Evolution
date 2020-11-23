#pragma once
#include <vector>
#include <functional>
#include "PhenotypeNeuron.h"


class Phenotype
{
public:
	Phenotype(const std::vector<PhenotypeNeuron*>& neurons, const int& maxDepth);
	~Phenotype();
	std::vector <double> calculateOutputSnapshot(const std::vector <double> &input, 
		const std::function<double(const double& input)>& activationFunction);
	std::vector <double> calculateOutputActive(const std::vector <double> &input,
		const std::function<double(const double& input)>& activationFunction);
private:
	void setBias();
	void setInputs(const std::vector <double> &inputs);
	std::vector<double> updateOnce(const std::function<double(const double& input)>& activationFunction);
	void clearOutputOfNeurons();

	std::vector<PhenotypeNeuron*> neurons;
	int maxDepth;
};

