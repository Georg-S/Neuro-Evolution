#pragma once
#include "InnovationType.h"

namespace nev {

	struct InnovationElement {
		InnovationElement(const int& innovationId, const int& fromNeuronId,
			const int& toNeuronId, const InnovationType& innovationType) {

			this->neuronId = -1;
			this->innovationId = innovationId;
			this->fromNeuronId = fromNeuronId;
			this->toNeuronId = toNeuronId;
			this->innovationType = innovationType;
		}
		InnovationElement(const int& innovationID, const int& fromNeuronId,
			const int& toNeuronId, const int& neuronId, const InnovationType& innovationType) {

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

}