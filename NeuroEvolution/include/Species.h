#pragma once
#include "Genotype.h"
#include <vector>

class Species
{
public:
	Species();
	Species(Genotype &representative, int speciesId);
	~Species();
	double calculateCompatibilityScore(Genotype &toTestGenotype, const float &exzessFactor,
									const float &disjointFactor, const float &weightFactor);
	void reset();
	void addMemberToSpecies(Genotype* genotype);
	void updateFitnessValues();
	void calculateSpawnAmount(const double& populationAverage);
	void incrementCurrentGeneration();
	double getTotalCurrentAdjustedFitness();
	int getGenerationOfSpecies();
	int getSpeciesId();
	int getMemberCount();
	int getSpawnAmount();
	int getGenerationNoImprovement();
	Genotype getLeader();
	Genotype spawnGenotype();

	friend bool operator<(const Species &lhs, const Species &rhs) {
		return lhs.totalCurrentAdjustedFitness > rhs.totalCurrentAdjustedFitness;
	}
private:
	Genotype representative;
	std::vector<Genotype*> members;

	int allTimeHighestRawFitness = 0;
	int speciesId;
	int generationsNoImprovement = 0;
	double highestAdjustedFitnessEver = 0;
	int currentGeneration = 0;
	double spawnAmount = 0;
	double totalCurrentAdjustedFitness = 0;
};

