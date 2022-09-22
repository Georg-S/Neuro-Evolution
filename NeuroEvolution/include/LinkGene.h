#pragma once

namespace nev
{
	struct LinkGene
	{
		LinkGene() = default;
		LinkGene(int fromNeuronID, int toNeuronID, double weight, bool enabled, int innovationID, bool recurrent = false)
			: fromNeuronID(fromNeuronID)
			, toNeuronID(toNeuronID)
			, weight(weight)
			, enabled(enabled)
			, innovationID(innovationID)
			, recurrent(recurrent)
		{
		}
		int fromNeuronID;
		int toNeuronID;
		double weight;
		bool enabled;
		int innovationID;
		bool recurrent;

		friend bool operator<(const LinkGene& lhs, const LinkGene& rhs)
		{
			return (lhs.innovationID < rhs.innovationID);
		}
	};
}