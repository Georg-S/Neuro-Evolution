#include "Species.h"


Species::Species()
{
}

Species::Species(Genotype &representative, int speciesId)
{
	this->representative = representative;
	this->speciesId = speciesId;
	members.push_back(&(representative));
}


Species::~Species()
{
}

double Species::calculateCompatibilityScore(Genotype & toTestGenotype, const float & exzessFactor, const float & disjointFactor, const float & weightFactor)
{
	return representative.calculateCompatibilityScore(toTestGenotype, exzessFactor, disjointFactor, weightFactor);
}

void Species::reset()
{
	members.clear();
}

void Species::addMemberToSpecies(Genotype* genotype)
{
	members.push_back(genotype);
}

void Species::updateFitnessValues()
{
	totalCurrentAdjustedFitness = 0;

	for (int i = 0; i < members.size(); i++) {
		double adjustedFitness = members[i]->getRawFitness() / members.size();
		totalCurrentAdjustedFitness += adjustedFitness;
		members[i]->setAdjustedFitness(adjustedFitness);
	}

	if (totalCurrentAdjustedFitness > highestAdjustedFitnessEver) {
		highestAdjustedFitnessEver = totalCurrentAdjustedFitness;
		generationsNoImprovement = 0;
	}
}

void Species::calculateSpawnAmount(const double & populationAverage)
{
	spawnAmount = 0;
	for (int i = 0; i < members.size(); i++) {
		spawnAmount += (members[i]->getAdjustedFitness() / populationAverage);
	}
}

double Species::getTotalCurrentAdjustedFitness()
{
	return totalCurrentAdjustedFitness;
}

int Species::getGenerationOfSpecies()
{
	return currentGeneration;
}

int Species::getSpeciesId()
{
	return speciesId;
}

int Species::getMemberCount()
{
	return members.size();
}

int Species::getSpawnAmount()
{
	return spawnAmount;
}

int Species::getGenerationNoImprovement()
{
	return generationsNoImprovement;
}

Genotype Species::getLeader()
{
	double highestFitness = -1;
	int highestFitnessIndex = 0;
	for (int i = 0; i < members.size(); i++) {
		if (highestFitness <= members[i]->getRawFitness()) {
			highestFitnessIndex = i;
			highestFitness = members[i]->getRawFitness();
		}
	}

	return *(members[highestFitnessIndex]);
}

Genotype Species::spawnGenotype()
{
	double randomFitness = RNG::getRandomFloatBetween(0, totalCurrentAdjustedFitness);
	double accumalatedFitness = 0;
	for (int i = 0; i < members.size(); i++) {
		accumalatedFitness += members[i]->getAdjustedFitness();

		if (randomFitness <= accumalatedFitness)
			return *(members[i]);
	}
}

void Species::incrementCurrentGeneration()
{
	currentGeneration++;
	generationsNoImprovement++;
}
