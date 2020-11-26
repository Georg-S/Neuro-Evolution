#pragma once
#include "PhenotypeLink.h"
#include <vector>
#include "NeuronType.h"

using namespace std;

struct PhenotypeNeuron {
	PhenotypeNeuron(NeuronType neuronType, int neuronID, int depth) {
		this->neuronType = neuronType;
		this->neuronID = neuronID;
		this->depth = depth;
		sumOfActivation = 0;
		outputOfNeuron = 0;
	}
	vector<PhenotypeLink> linksIn;
	vector<PhenotypeLink> linksOut;

	double sumOfActivation;
	double outputOfNeuron;
	NeuronType neuronType;
	int neuronID;
	int depth;
};
