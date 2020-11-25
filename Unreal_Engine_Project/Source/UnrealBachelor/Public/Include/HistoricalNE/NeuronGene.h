#pragma once
#include "NeuronType.h"

struct NeuronGene {
	NeuronGene() {
		this->id = -1;
	}
	NeuronGene(NeuronType neuronType, int neuronID, bool recurrent = false) {
		this->previousNeuronIndex = previousNeuronIndex;
		this->type = neuronType;
		this->id = neuronID;
		this->recurrent = recurrent;
	}
	int previousNeuronIndex;
	NeuronType type;
	int id;
	int depth;
	bool recurrent;
};