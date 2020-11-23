#pragma once

namespace nev {

	struct LinkGene {
		LinkGene() {}
		LinkGene(const int& fromNeuronID, const int& toNeuronID, const double& weight, const bool& enabled,
			const int& innovationID, const bool& recurrent = false) {

			this->fromNeuronID = fromNeuronID;
			this->toNeuronID = toNeuronID;
			this->weight = weight;
			this->enabled = enabled;
			this->innovationID = innovationID;
			this->recurrent = recurrent;
		}
		friend bool operator<(const LinkGene& lhs, const LinkGene& rhs)
		{
			return (lhs.innovationID < rhs.innovationID);
		}

		int fromNeuronID;
		int toNeuronID;
		double weight;
		bool enabled;
		int innovationID;
		bool recurrent;
	};

}