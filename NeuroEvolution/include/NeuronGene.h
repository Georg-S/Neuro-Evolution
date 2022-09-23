#pragma once
#include "NeuronType.h"

namespace nev
{
	struct NeuronGene
	{
		NeuronGene() = default;
		NeuronGene(NeuronType neuronType, int id)
			: neuronType(neuronType), id(id)
		{
		}
		NeuronType neuronType = NeuronType::none;
		int id = -1;
		int depth = -1;
	};
}