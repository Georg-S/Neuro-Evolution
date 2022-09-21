#pragma once

namespace nev
{
	enum class InnovationType
	{
		newNeuron, newLink
	};

	struct InnovationElement
	{
		InnovationElement(int innovationId, int fromNeuronId, int toNeuronId, InnovationType innovationType)
			: innovationId(innovationId), fromNeuronId(fromNeuronId), toNeuronId(toNeuronId), neuronId(-1), innovationType(innovationType)
		{
		}

		InnovationElement(int innovationId, int fromNeuronId, int toNeuronId, int neuronId, InnovationType innovationType)
			: innovationId(innovationId), fromNeuronId(fromNeuronId), toNeuronId(toNeuronId), neuronId(neuronId), innovationType(innovationType)
		{
		}

		int innovationId;
		int fromNeuronId;
		int toNeuronId;
		int neuronId;
		InnovationType innovationType;
	};
}