#include "Species.h"

nev::Species::Species(const Genotype* representative, int speciesId)
	: m_representative(std::make_unique<Genotype>(*representative))
	, m_speciesId(speciesId)
{
	m_members.emplace_back(m_representative.get());
}

double nev::Species::calculateCompatibilityScore(Genotype* toTestGenotype, double excessFactor, double disjointFactor, double weightFactor)
{
	return Genotype::calculateCompatibilityScore(m_representative.get(), toTestGenotype, excessFactor, disjointFactor, weightFactor);
}

void nev::Species::reset()
{
	m_members.clear();
}

void nev::Species::addMemberToSpecies(Genotype* genotype)
{
	m_members.emplace_back(genotype);
}

void nev::Species::updateFitnessValues()
{
	m_totalCurrentAdjustedFitness = 0;

	for (auto member : m_members)
	{
		double adjustedFitness = member->getRawFitness() / m_members.size();
		m_totalCurrentAdjustedFitness += adjustedFitness;
		member->setAdjustedFitness(adjustedFitness);
	}

	if (m_totalCurrentAdjustedFitness > m_highestAdjustedFitnessEver)
	{
		m_highestAdjustedFitnessEver = m_totalCurrentAdjustedFitness;
		m_generationsNoImprovement = 0;
	}
}

void nev::Species::calculateSpawnAmount(double populationAverage)
{
	m_spawnAmount = 0;
	for (auto member : m_members)
		m_spawnAmount += member->getAdjustedFitness();

	m_spawnAmount /= populationAverage;
}

void nev::Species::incrementCurrentGeneration()
{
	m_currentGeneration++;
	m_generationsNoImprovement++;
}

std::unique_ptr<nev::Genotype> nev::Species::spawnNewGenotypeThroughRoulette() const
{
	double randomFitness = getRandomDouble(0, m_totalCurrentAdjustedFitness);
	double accumalatedFitness = 0;
	for (auto genotype : m_members)
	{
		accumalatedFitness += genotype->getAdjustedFitness();

		if (randomFitness <= accumalatedFitness)
			return std::make_unique<Genotype>(*genotype);
	}
	assert(!"Reached invalid state while spawning new Genotype");
	return std::make_unique<Genotype>(*m_representative);
}

std::unique_ptr<nev::Genotype> nev::Species::getDeepCopyOfSpeciesLeader() const
{
	double highestFitness = DBL_MIN;
	Genotype* highestFitnessGenotype = nullptr;

	for (auto member : m_members)
	{
		if (highestFitness <= member->getRawFitness())
		{
			highestFitnessGenotype = member;
			highestFitness = member->getRawFitness();
		}
	}

	assert(highestFitnessGenotype);
	if (!highestFitnessGenotype)
		return nullptr;

	return std::make_unique<Genotype>(*highestFitnessGenotype);
}

double nev::Species::getTotalCurrentAdjustedFitness() const
{
	return m_totalCurrentAdjustedFitness;
}

int nev::Species::getGenerationOfSpecies() const
{
	return m_currentGeneration;
}

int nev::Species::getSpeciesId() const
{
	return m_speciesId;
}

int nev::Species::getMemberCount() const
{
	return m_members.size();
}

int nev::Species::getSpawnAmount() const
{
	return m_spawnAmount;
}

int nev::Species::getGenerationNoImprovement() const
{
	return m_generationsNoImprovement;
}

bool nev::operator<(const Species& lhs, const Species& rhs)
{
	return lhs.m_totalCurrentAdjustedFitness > rhs.m_totalCurrentAdjustedFitness;
}
