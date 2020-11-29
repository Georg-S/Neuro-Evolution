#pragma once
#include <vector>
#include <memory>
#include "Genotype.h"


namespace nev {

	class Species
	{
	public:
		Species();
		Species(std::shared_ptr<Genotype> representative, int speciesId);
		~Species();
		double calculateCompatibilityScore(std::shared_ptr<Genotype> toTestGenotype, double excessFactor,
			double disjointFactor, double weightFactor);
		void reset();
		void addMemberToSpecies(Genotype* genotype);
		void updateFitnessValues();
		void calculateSpawnAmount(double populationAverage);
		void incrementCurrentGeneration();
		std::shared_ptr<Genotype> spawnGenotypeRoulette();
		std::shared_ptr<Genotype> getLeader() const;
		double getTotalCurrentAdjustedFitness() const;
		int getGenerationOfSpecies() const;
		int getSpeciesId() const;
		int getMemberCount() const;
		int getSpawnAmount() const;
		int getGenerationNoImprovement() const;
		friend bool operator<(const Species& lhs, const Species& rhs);

	private:
		std::shared_ptr<Genotype>  representative;
		std::vector<Genotype*> members;

		int allTimeHighestRawFitness = 0;
		int speciesId;
		int generationsNoImprovement = 0;
		double highestAdjustedFitnessEver = 0;
		int currentGeneration = 0;
		double spawnAmount = 0;
		double totalCurrentAdjustedFitness = 0;
	};

}