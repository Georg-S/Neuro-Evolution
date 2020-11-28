#pragma once
#include <vector>
#include "InnovationType.h"
#include "InnovationElement.h"

namespace nev {

	class Innovation
	{
	public:
		Innovation();
		Innovation(int currentInnovationNumber, int currentNeuronId, const std::vector<InnovationElement>& innovations);
		~Innovation();

		int createNewLinkInnovation(int fromId, int toId);
		int createNewNeuronInnovation(int fromId, int toId);

		void setCurrentNeuronId(int currenNeuronId);
		int getInnovationID(int fromID, int toID, InnovationType type) const;
		int getTotalInnovationsCount() const;
		int getNeuronId(int fromNeuronId, int toNeuronId) const;
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

}