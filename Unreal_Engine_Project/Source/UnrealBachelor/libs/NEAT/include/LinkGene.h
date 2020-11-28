#pragma once

struct LinkGene {
	LinkGene() {};
	LinkGene(int fromNeuronID, int toNeuronID, double weightOfLink, bool isEnabled, int innovationNumber, bool isRecurrent = false) {
		fromID = fromNeuronID;
		toID = toNeuronID;
		weight = weightOfLink;
		enabled = isEnabled;
		innovationID = innovationNumber;
		recurrent = isRecurrent;
	}
	friend bool operator<(const LinkGene& lhs, const LinkGene& rhs)
	{
		return (lhs.innovationID < rhs.innovationID);
	}

	int fromID;
	int toID;
	double weight;
	bool enabled;
	int innovationID;
	bool recurrent;
};