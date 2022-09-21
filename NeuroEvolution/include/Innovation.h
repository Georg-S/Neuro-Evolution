#pragma once
#include <vector>
#include <algorithm>
#include "InnovationElement.h"

namespace nev {

	class Innovation
	{
	public:
		Innovation() = default;
		Innovation(int currentInnovationNumber, int currentNeuronId, std::vector<InnovationElement>&& innovations);

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
		int m_currentInnovationNumber = 0;
		int m_currentNeuronId = -1;
		std::vector<InnovationElement> m_innovations;
	};

}