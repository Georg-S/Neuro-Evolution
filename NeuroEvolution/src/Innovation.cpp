#include "Innovation.h"

Innovation::Innovation()
{
}

Innovation::Innovation(const int& currentInnovationNumber, const int& currentNeuronId,
	const std::vector<InnovationElement>& innovations)
{
	this->currentInnovationNumber = currentInnovationNumber;
	this->currentNeuronId = currentNeuronId;
	this->innovations = innovations;
}

Innovation::~Innovation()
{
}

int Innovation::createNewLinkInnovation(const int& fromId, const int& toId)
{
	if (fromId > currentNeuronId)
		currentNeuronId = fromId;

	if (toId > currentNeuronId)
		currentNeuronId = toId;

	int innovation = currentInnovationNumber;
	innovations.push_back(InnovationElement(currentInnovationNumber, fromId, toId, InnovationType::newLink));
	currentInnovationNumber++;

	return innovation;
}

int Innovation::createNewNeuronInnovation(const int& fromId, const int& toId)
{
	currentNeuronId++;
	innovations.push_back(InnovationElement(currentInnovationNumber, fromId, toId, currentNeuronId, InnovationType::newNeuron));
	currentInnovationNumber++;

	return currentNeuronId;
}

void Innovation::setCurrentNeuronId(const int& currenNeuronId)
{
	this->currentNeuronId = currenNeuronId;
}

int Innovation::getInnovationID(const int& fromID, const int& toID, const InnovationType& type) const
{
	for (int i = 0; i < innovations.size(); i++) {
		if ((innovations[i].fromNeuronId == fromID)
			&& (innovations[i].toNeuronId == toID)
			&& (innovations[i].innovationType == type)) {

			return innovations[i].innovationId;
		}
	}
	return -1;
}

int Innovation::getTotalInnovationsCount() const
{
	return innovations.size();
}


int Innovation::getNeuronId(const int& fromNeuronId, const int& toNeuronId) const
{
	for (int i = 0; i < innovations.size(); i++) {
		if (innovations[i].innovationType != InnovationType::newNeuron)
			continue;

		if ((fromNeuronId == innovations[i].fromNeuronId) && (toNeuronId == innovations[i].toNeuronId))
			return innovations[i].neuronId;
	}

	return -1;
}

int Innovation::getCountOfNewNeuronInnovations() const
{
	int countOfNewNeuronInnovations = 0;
	for (int i = 0; i < innovations.size(); i++) {
		if (innovations[i].innovationType == InnovationType::newNeuron)
			countOfNewNeuronInnovations++;
	}

	return countOfNewNeuronInnovations;
}

int Innovation::getCountOfNewLinkInnovations() const
{
	int countOfNewLinkInnovation = 0;
	for (int i = 0; i < innovations.size(); i++) {
		if (innovations[i].innovationType == InnovationType::newLink)
			countOfNewLinkInnovation++;
	}

	return countOfNewLinkInnovation;
}

int Innovation::getCurrentInnovationNumber() const
{
	return currentInnovationNumber;
}

int Innovation::getCurrentNeuronId() const
{
	return currentNeuronId;
}

std::vector<InnovationElement> Innovation::getInnovations() const
{
	return innovations;
}

