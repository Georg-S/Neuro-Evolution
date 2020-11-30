#include "Species.h"


nev::Species::Species()
{
}

nev::Species::Species(std::shared_ptr<Genotype> representative, int speciesId)
{
	this->representative = std::make_shared<Genotype>(*representative.get());
	this->speciesId = speciesId;
	members.push_back(representative.get());
}


nev::Species::~Species()
{
}

double nev::Species::calculateCompatibilityScore(std::shared_ptr<Genotype> toTestGenotype, double excessFactor,
	double disjointFactor, double weightFactor)
{
	return Genotype::calculateCompatibilityScore(representative, toTestGenotype, excessFactor, disjointFactor, weightFactor);
}

void nev::Species::reset()
{
	members.clear();
}

void nev::Species::addMemberToSpecies(Genotype* genotype)
{
	members.push_back(genotype);
}

void nev::Species::updateFitnessValues()
{
	totalCurrentAdjustedFitness = 0;

	for (int i = 0; i < members.size(); i++) {
		double adjustedFitness = members[i]->getRawFitness() / (double)members.size();
		totalCurrentAdjustedFitness += adjustedFitness;
		members[i]->setAdjustedFitness(adjustedFitness);
	}

	if (totalCurrentAdjustedFitness > highestAdjustedFitnessEver) {
		highestAdjustedFitnessEver = totalCurrentAdjustedFitness;
		generationsNoImprovement = 0;
	}
}

void nev::Species::calculateSpawnAmount(double populationAverage)
{
	spawnAmount = 0;
	for (int i = 0; i < members.size(); i++)
		spawnAmount += members[i]->getAdjustedFitness();

	spawnAmount /= populationAverage;
}

void nev::Species::incrementCurrentGeneration()
{
	currentGeneration++;
	generationsNoImprovement++;
}

std::shared_ptr<nev::Genotype> nev::Species::spawnNewGenotypeThroughRoulette() const
{
	double randomFitness = RNG::getRandomDoubleBetween(0, totalCurrentAdjustedFitness);
	double accumalatedFitness = 0;
	for (int i = 0; i < members.size(); i++) {
		accumalatedFitness += members[i]->getAdjustedFitness();

		if (randomFitness <= accumalatedFitness)
			return std::make_shared<Genotype>(*(members[i]));
	}
	return representative;
}

std::shared_ptr<nev::Genotype> nev::Species::getDeepCopyOfSpeciesLeader() const
{
	double highestFitness = DBL_MIN;
	int highestFitnessIndex = -1;
	for (int i = 0; i < members.size(); i++) {
		if (highestFitness <= members[i]->getRawFitness()) {
			highestFitnessIndex = i;
			highestFitness = members[i]->getRawFitness();
		}
	}

	return (std::make_shared<Genotype>(*(members[highestFitnessIndex])));
}

double nev::Species::getTotalCurrentAdjustedFitness() const
{
	return totalCurrentAdjustedFitness;
}

int nev::Species::getGenerationOfSpecies() const
{
	return currentGeneration;
}

int nev::Species::getSpeciesId() const
{
	return speciesId;
}

int nev::Species::getMemberCount() const
{
	return members.size();
}

int nev::Species::getSpawnAmount() const
{
	return spawnAmount;
}

int nev::Species::getGenerationNoImprovement() const
{
	return generationsNoImprovement;
}

bool nev::operator<(const Species& lhs, const Species& rhs)
{
	return lhs.totalCurrentAdjustedFitness > rhs.totalCurrentAdjustedFitness;
}
