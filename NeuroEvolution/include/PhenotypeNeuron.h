#pragma once
#include <vector>
#include "NeuronType.h"

namespace nev
{
	struct PhenotypeNeuron
	{
		struct Link
		{
			Link(PhenotypeNeuron* from, double weight)
				: fromNeuron(from), weight(weight)
			{
			}
			PhenotypeNeuron* fromNeuron;
			double weight;
		};

		PhenotypeNeuron(NeuronType neuronType, int neuronID)
			: neuronID(neuronID), neuronType(neuronType), outputOfNeuron(0.0)
		{
		}
		std::vector<Link> linksIn;
		int neuronID;
		NeuronType neuronType;
		double outputOfNeuron;
	};
}