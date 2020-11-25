#pragma once
#include "PhenotypeNeuron.h"
#include <vector>

using namespace std;

class Phenotype
{
public:
	Phenotype(vector<PhenotypeNeuron*> neurons, int maxDepth);
	~Phenotype();
	vector <double> calculateOutputSnapshot(const vector <double> &input);
	vector <double> calculateOutputActive(const vector <double> &input);
private:
	vector<PhenotypeNeuron*> neurons;
	int maxDepth;

	void setBias();
	void setInputs(const vector <double> &inputs);
	double activationFunction(const double &input);
	vector<double> updateOnce();
	void clearOutputOfNeurons();

};

