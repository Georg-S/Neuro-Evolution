#pragma once
#include "InnovationType.h"
#include "InnovationElement.h"
#include <vector>

using namespace std;

class Innovation
{
public:
	Innovation();
	Innovation(int currentInnovationNumber, int currentNeuronId, vector<InnovationElement> innovations);
	~Innovation();

	int getInnovationID(int fromID, int toID, InnovationType type);

	int createNewLinkInnovation(int fromId, int toId);
	int createNewNeuronInnovation(int fromId, int toId);
	int getNeuronId(int fromNeuronId, int toNeuronId);

	int getCountOfNewNeuronInnovations();
	int getCountOfNewLinkInnovations();
	int getTotalInnovationsCount();
	int getCurrentInnovationNumber();
	int getCurrentNeuronId();
	void setCurrentNeuronId(int currenNeuronId);
	vector<InnovationElement> getInnovations();

private:
	vector<InnovationElement> innovations;
	int currentInnovationNumber = 0;
	int currentNeuronId = -1;

};

