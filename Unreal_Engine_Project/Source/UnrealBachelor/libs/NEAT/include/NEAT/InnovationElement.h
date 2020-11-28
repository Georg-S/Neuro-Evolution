#pragma once
#include "InnovationType.h"

struct InnovationElement {
	InnovationElement(int innovationId, int fromNeuronId, int toNeuronId, InnovationType innovationType) {
		neuronId = -1;
		this->innovationId = innovationId;
		this->fromNeuronId = fromNeuronId;
		this->toNeuronId = toNeuronId;
		this->innovationType = innovationType;
	}
	InnovationElement(int innovationID, int fromNeuronId, int toNeuronId, int neuronId, InnovationType innovationType) {
		this->fromNeuronId = fromNeuronId;
		this->toNeuronId = toNeuronId;
		this->innovationId = innovationID;
		this->neuronId = neuronId;
		this->innovationType = innovationType;
	}
	int innovationId;
	int fromNeuronId;
	int toNeuronId;
	int neuronId;
	InnovationType innovationType;
};