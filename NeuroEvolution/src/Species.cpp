#include "Species.h"


nev::Species::Species()
{
}

nev::Species::Species(const Genotype& representative, int speciesId)
{
	this->representative = representative;
	this->representative.phenotype = nullptr;
	this->speciesId = speciesId;
	members.push_back(&this->representative);
}


nev::Species::~Species()
{
}

double nev::Species::calculateCompatibilityScore(Genotype& toTestGenotype, double excessFactor,
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

nev::Genotype nev::Species::spawnGenotypeRoulette()
{
	double randomFitness = RNG::getRandomDoubleBetween(0, totalCurrentAdjustedFitness);
	double accumalatedFitness = 0;
	for (int i = 0; i < members.size(); i++) {
		accumalatedFitness += members[i]->getAdjustedFitness();

		if (randomFitness <= accumalatedFitness)
			return *(members[i]);
	}
}

nev::Genotype nev::Species::getLeader() const
{
	double highestFitness = DBL_MIN;
	int highestFitnessIndex = -1;
	for (int i = 0; i < members.size(); i++) {
		if (highestFitness <= members[i]->getRawFitness()) {
			highestFitnessIndex = i;
			highestFitness = members[i]->getRawFitness();
		}
	}

	return *(members[highestFitnessIndex]);
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
