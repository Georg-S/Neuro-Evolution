#pragma once
#include "PhenotypeLink.h"
#include <vector>
#include "NeuronType.h"

using namespace std;

struct PhenotypeNeuron {
	PhenotypeNeuron(NeuronType neuronType, int neuronID) {
		this->neuronType = neuronType;
		this->neuronID = neuronID;
		outputOfNeuron = 0.0;
	}
	vector<PhenotypeLink> linksIn;
	double outputOfNeuron;
	NeuronType neuronType;
	int neuronID;
};
