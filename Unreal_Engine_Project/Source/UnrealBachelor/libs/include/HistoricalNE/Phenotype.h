#pragma once
#include <vector>
#include "PhenotypeNeuron.h"
#include "NeuronType.h"

using namespace std;

class Phenotype
{
public:
	Phenotype();
	Phenotype(vector<PhenotypeNeuron*> neurons, int maxDepth);
	~Phenotype();
	vector<double> updateOnce();
	vector<double> updateSnapshot(const vector<double> &inputs);
	vector<double> updateActive(const vector<double> &inputs);
	double activationFunction(double input);

	void setInputNeurons(const vector<double> &inputs);
	void setOutputOfBiasNeuron();

	int getCountOfNeurons();
private:
	vector<PhenotypeNeuron*> neurons;
	int maxDepth;
};

