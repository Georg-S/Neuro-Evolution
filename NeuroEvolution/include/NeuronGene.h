#pragma once
#include "NeuronType.h"

namespace nev {

	struct NeuronGene {
		NeuronGene() {
			this->id = -1;
		}
		NeuronGene(NeuronType neuronType, int id) {
			this->neuronType = neuronType;
			this->id = id;
		}
		NeuronType neuronType;
		int id;
		int depth;
	};

}