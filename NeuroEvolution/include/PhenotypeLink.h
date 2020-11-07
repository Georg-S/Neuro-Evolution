#pragma once

struct PhenotypeNeuron;
#include "PhenotypeNeuron.h"

struct PhenotypeLink {
	PhenotypeLink(PhenotypeNeuron* from, PhenotypeNeuron* to, double weight) {
		this->fromNeuron = from;
		this->to = to;
		this->weight = weight;
	}
	PhenotypeNeuron* fromNeuron;
	PhenotypeNeuron* to;
	double weight;
};