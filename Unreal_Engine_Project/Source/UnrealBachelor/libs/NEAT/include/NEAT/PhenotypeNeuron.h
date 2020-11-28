#pragma once
#include "PhenotypeLink.h"
#include <vector>
#include "NeuronType.h"

using namespace std;

struct PhenotypeNeuron {
	PhenotypeNeuron(NeuronType neuronType) {
		this->neuronType = neuronType;
		outputOfNeuron = 0.0;
	}
	vector<PhenotypeLink> linksIn;
	double outputOfNeuron;
	NeuronType neuronType;
	int neuronID;
};
