#pragma once

struct PhenotypeNeuron;
#include "PhenotypeNeuron.h"

struct PhenotypeLink {
	PhenotypeLink(PhenotypeNeuron* from, double weight) {
		this->fromNeuron = from;
		this->weight = weight;
	}
	PhenotypeNeuron* fromNeuron;
	double weight;
};