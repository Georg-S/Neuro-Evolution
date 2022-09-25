#include "Innovation.h"

nev::Innovation::Innovation(int currentInnovationNumber, int currentNeuronId, std::vector<InnovationElement>&& innovations)
	: m_currentInnovationNumber(currentInnovationNumber)
	, m_currentNeuronId(currentNeuronId)
	, m_innovations(std::move(innovations))
{
}

int nev::Innovation::createNewLinkInnovation(int fromId, int toId)
{
	m_currentNeuronId = std::max({ m_currentNeuronId, fromId, toId });
	int innovation = m_currentInnovationNumber;
	m_innovations.emplace_back(m_currentInnovationNumber, fromId, toId, InnovationType::newLink);
	m_currentInnovationNumber++;

	return innovation;
}

int nev::Innovation::createNewNeuronInnovation(int fromId, int toId)
{
	// TODO take a look if increment before is correct
	m_currentNeuronId++;
	m_innovations.emplace_back(m_currentInnovationNumber, fromId, toId, m_currentNeuronId, InnovationType::newNeuron);
	m_currentInnovationNumber++;

	return m_currentNeuronId;
}

void nev::Innovation::setCurrentNeuronId(int currenNeuronId)
{
	m_currentNeuronId = currenNeuronId;
}

int nev::Innovation::getInnovationID(int fromID, int toID, InnovationType type) const
{
	for (const auto& innovation : m_innovations)
	{
		if ((innovation.fromNeuronId == fromID) && (innovation.toNeuronId == toID) && (innovation.innovationType == type))
			return innovation.innovationId;
	}

	return -1;
}

int nev::Innovation::getTotalInnovationsCount() const
{
	return m_innovations.size();
}


int nev::Innovation::getNeuronId(int fromNeuronId, int toNeuronId) const
{
	for(const auto& innovation : m_innovations)
	{
		if (innovation.innovationType != InnovationType::newNeuron)
			continue;

		if ((fromNeuronId == innovation.fromNeuronId) && (toNeuronId == innovation.toNeuronId))
			return innovation.neuronId;
	}

	return -1;
}

static int getCountOfInnovations(const std::vector<nev::InnovationElement>& innovations, nev::InnovationType type) 
{
	int counter = 0;
	for (const auto& innovation : innovations)
	{
		if (innovation.innovationType == type)
			counter++;
	}

	return counter;
}

int nev::Innovation::getCountOfNewNeuronInnovations() const
{
	return getCountOfInnovations(m_innovations, InnovationType::newNeuron);
}

int nev::Innovation::getCountOfNewLinkInnovations() const
{
	return getCountOfInnovations(m_innovations, InnovationType::newLink);
}

int nev::Innovation::getCurrentInnovationNumber() const
{
	return m_currentInnovationNumber;
}

int nev::Innovation::getCurrentNeuronId() const
{
	return m_currentNeuronId;
}

std::vector<nev::InnovationElement> nev::Innovation::getInnovations() const
{
	return m_innovations;
}

