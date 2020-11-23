#include "Innovation.h"

nev::Innovation::Innovation()
{
}

nev::Innovation::Innovation(const int& currentInnovationNumber, const int& currentNeuronId,
	const std::vector<InnovationElement>& innovations)
{
	this->currentInnovationNumber = currentInnovationNumber;
	this->currentNeuronId = currentNeuronId;
	this->innovations = innovations;
}

nev::Innovation::~Innovation()
{
}

int nev::Innovation::createNewLinkInnovation(const int& fromId, const int& toId)
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

int nev::Innovation::createNewNeuronInnovation(const int& fromId, const int& toId)
{
	currentNeuronId++;
	innovations.push_back(InnovationElement(currentInnovationNumber, fromId, toId, currentNeuronId, InnovationType::newNeuron));
	currentInnovationNumber++;

	return currentNeuronId;
}

void nev::Innovation::setCurrentNeuronId(const int& currenNeuronId)
{
	this->currentNeuronId = currenNeuronId;
}

int nev::Innovation::getInnovationID(const int& fromID, const int& toID, const InnovationType& type) const
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

int nev::Innovation::getTotalInnovationsCount() const
{
	return innovations.size();
}


int nev::Innovation::getNeuronId(const int& fromNeuronId, const int& toNeuronId) const
{
	for (int i = 0; i < innovations.size(); i++) {
		if (innovations[i].innovationType != InnovationType::newNeuron)
			continue;

		if ((fromNeuronId == innovations[i].fromNeuronId) && (toNeuronId == innovations[i].toNeuronId))
			return innovations[i].neuronId;
	}

	return -1;
}

int nev::Innovation::getCountOfNewNeuronInnovations() const
{
	int countOfNewNeuronInnovations = 0;
	for (int i = 0; i < innovations.size(); i++) {
		if (innovations[i].innovationType == InnovationType::newNeuron)
			countOfNewNeuronInnovations++;
	}

	return countOfNewNeuronInnovations;
}

int nev::Innovation::getCountOfNewLinkInnovations() const
{
	int countOfNewLinkInnovation = 0;
	for (int i = 0; i < innovations.size(); i++) {
		if (innovations[i].innovationType == InnovationType::newLink)
			countOfNewLinkInnovation++;
	}

	return countOfNewLinkInnovation;
}

int nev::Innovation::getCurrentInnovationNumber() const
{
	return currentInnovationNumber;
}

int nev::Innovation::getCurrentNeuronId() const
{
	return currentNeuronId;
}

std::vector<nev::InnovationElement> nev::Innovation::getInnovations() const
{
	return innovations;
}

