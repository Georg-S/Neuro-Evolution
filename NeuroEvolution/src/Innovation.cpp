#include "Innovation.h"


Innovation::Innovation()
{
}

Innovation::Innovation(int currentInnovationNumber, int currentNeuronId, vector<InnovationElement> innovations)
{
	this->currentInnovationNumber = currentInnovationNumber;
	this->currentNeuronId = currentNeuronId;
	this->innovations = innovations;
}


Innovation::~Innovation()
{
}

void Innovation::setCurrentNeuronId(int currenNeuronId)
{
	this->currentNeuronId = currenNeuronId;
}

int Innovation::getInnovationID(int fromID, int toID, InnovationType type)
{
	for (int i = 0; i < innovations.size(); i++) {
		if (innovations[i].fromNeuronId == fromID && innovations[i].toNeuronId == toID && innovations[i].innovationType == type) {
			return innovations[i].innovationId;
		}
	}
	return -1;
}

int Innovation::getTotalInnovationsCount()
{
	return innovations.size();
}

int Innovation::createNewLinkInnovation(int fromId, int toId)
{
	if (fromId > currentNeuronId)
		currentNeuronId = fromId;

	if (toId > currentNeuronId)
		currentNeuronId = toId;

	int innovation = currentInnovationNumber;
	innovations.push_back(InnovationElement(currentInnovationNumber, fromId, toId, newLink));
	currentInnovationNumber++;

	return innovation;
}

int Innovation::createNewNeuronInnovation(int fromId, int toId)
{
	currentNeuronId++;
	innovations.push_back(InnovationElement(currentInnovationNumber, fromId, toId, currentNeuronId, newNeuron));
	currentInnovationNumber++;

	return currentNeuronId;
}

int Innovation::getNeuronId(int fromNeuronId, int toNeuronId)
{
	for (int i = 0; i < innovations.size(); i++) {
		if (innovations[i].innovationType != newNeuron)
			continue;

		if (fromNeuronId == innovations[i].fromNeuronId && toNeuronId == innovations[i].toNeuronId)
			return innovations[i].neuronId;
	}

	return -1;
}

int Innovation::getCountOfNewNeuronInnovations()
{
	int countOfNewNeuronInnovations = 0;
	for (int i = 0; i < innovations.size(); i++) {
		if (innovations[i].innovationType == newNeuron)
			countOfNewNeuronInnovations++;
	}

	return countOfNewNeuronInnovations;
}

int Innovation::getCountOfNewLinkInnovations()
{
	int countOfNewLinkInnovation = 0;
	for (int i = 0; i < innovations.size(); i++) {
		if (innovations[i].innovationType == newLink)
			countOfNewLinkInnovation++;
	}

	return countOfNewLinkInnovation;
}

int Innovation::getCurrentInnovationNumber()
{
	return currentInnovationNumber;
}

int Innovation::getCurrentNeuronId()
{
	return currentNeuronId;
}

vector<InnovationElement> Innovation::getInnovations()
{
	return innovations;
}

