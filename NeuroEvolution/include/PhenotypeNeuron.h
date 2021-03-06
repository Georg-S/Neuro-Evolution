#pragma once
#include <vector>
#include "NeuronType.h"

namespace nev {

	struct PhenotypeNeuron {
		struct Link {
			Link(PhenotypeNeuron* from, double weight) {
				this->fromNeuron = from;
				this->weight = weight;
			}
			PhenotypeNeuron* fromNeuron;
			double weight;
		};

		PhenotypeNeuron(NeuronType neuronType, int neuronID) {
			this->neuronType = neuronType;
			this->neuronID = neuronID;
			outputOfNeuron = 0.0;
		}
		std::vector<Link> linksIn;
		double outputOfNeuron;
		NeuronType neuronType;
		int neuronID;
	};

}