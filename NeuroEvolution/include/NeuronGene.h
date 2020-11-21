#pragma once
#include "NeuronType.h"

struct NeuronGene {
	NeuronGene() {
		this->id = -1;
	}
	NeuronGene(const NeuronType& neuronType, const int& id) {
		this->neuronType = neuronType;
		this->id = id;
	}
	NeuronType neuronType;
	int id;
	int depth;
};