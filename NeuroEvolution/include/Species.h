#pragma once
#include "Genotype.h"
#include <vector>

class Species
{
public:
	Species();
	Species(const Genotype& representative, const int& speciesId);
	~Species();
	double calculateCompatibilityScore(Genotype& toTestGenotype, const float& exzessFactor,
		const float& disjointFactor, const float& weightFactor);
	void reset();
	void addMemberToSpecies(Genotype* genotype);
	void updateFitnessValues();
	void calculateSpawnAmount(const double& populationAverage);
	void incrementCurrentGeneration();
	Genotype spawnGenotype();
	Genotype getLeader() const;
	double getTotalCurrentAdjustedFitness() const;
	int getGenerationOfSpecies() const;
	int getSpeciesId() const;
	int getMemberCount() const;
	int getSpawnAmount() const;
	int getGenerationNoImprovement() const;
	friend bool operator<(const Species& lhs, const Species& rhs);

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

