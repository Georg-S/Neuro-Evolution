#pragma once

struct PhenotypeNeuron;
#include "PhenotypeNeuron.h"

struct PhenotypeLink {
	PhenotypeLink(PhenotypeNeuron* from, PhenotypeNeuron* to, double weight, bool isRecurrent) {
		this->fromNeuron = from;
		this->to = to;
		this->weight = weight;
		this->isRecurrent = isRecurrent;
	}
	PhenotypeNeuron* fromNeuron;
	PhenotypeNeuron* to;
	double weight;
	bool isRecurrent;
};