#pragma once
#include <vector>
#include "InnovationType.h"
#include "InnovationElement.h"

class Innovation
{
public:
	Innovation();
	Innovation(const int &currentInnovationNumber, const int &currentNeuronId,const std::vector<InnovationElement> &innovations);
	~Innovation();

	int createNewLinkInnovation(const int &fromId, const int &toId);
	int createNewNeuronInnovation(const int &fromId, const int &toId);

	void setCurrentNeuronId(const int &currenNeuronId);
	int getInnovationID(const int &fromID, const int &toID, const InnovationType &type) const;
	int getTotalInnovationsCount() const;
	int getNeuronId(const int &fromNeuronId, const int &toNeuronId) const;
	int getCountOfNewNeuronInnovations() const;
	int getCountOfNewLinkInnovations() const;
	int getCurrentInnovationNumber() const;
	int getCurrentNeuronId() const;
	std::vector<InnovationElement> getInnovations() const;

private:
	std::vector<InnovationElement> innovations;
	int currentInnovationNumber = 0;
	int currentNeuronId = -1;
};

