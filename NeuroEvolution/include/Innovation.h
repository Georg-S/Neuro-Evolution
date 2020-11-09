#pragma once
#include <vector>
#include "InnovationType.h"
#include "InnovationElement.h"

class Innovation
{
public:
	Innovation();
	Innovation(int currentInnovationNumber, int currentNeuronId, std::vector<InnovationElement> innovations);
	~Innovation();

	void setCurrentNeuronId(int currenNeuronId);
	int getInnovationID(int fromID, int toID, InnovationType type);
	int getTotalInnovationsCount();

	int createNewLinkInnovation(int fromId, int toId);
	int createNewNeuronInnovation(int fromId, int toId);
	int getNeuronId(int fromNeuronId, int toNeuronId);
	int getCountOfNewNeuronInnovations();
	int getCountOfNewLinkInnovations();
	int getCurrentInnovationNumber();
	int getCurrentNeuronId();
	std::vector<InnovationElement> getInnovations();

private:
	std::vector<InnovationElement> innovations;
	int currentInnovationNumber = 0;
	int currentNeuronId = -1;

};

