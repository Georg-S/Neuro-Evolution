#pragma once

struct LinkGene {
	LinkGene() {};
	LinkGene(int fromNeuronID, int toNeuronID, double weightOfLink, bool isEnabled, bool isRecurrent = false) {
		fromID = fromNeuronID;
		toID = toNeuronID;
		weight = weightOfLink;
		enabled = isEnabled;
		recurrent = isRecurrent;
	}
	int fromID;
	int toID;
	double weight;
	bool enabled;
	bool recurrent;
};