#pragma once
#include <vector>
#include <memory>
#include <cassert>
#include "Genotype.h"

namespace nev
{
	class Species
	{
	public:
		Species() = default;
		Species(const Genotype* representative, int speciesId);
		double calculateCompatibilityScore(const Genotype* toTestGenotype, double excessFactor, double disjointFactor, double weightFactor) const;
		void reset();
		void addMemberToSpecies(Genotype* genotype);
		void updateFitnessValues();
		void calculateSpawnAmount(double populationAverage);
		void incrementCurrentGeneration();
		const Genotype* getGenotypeThroughRoulette() const;
		std::unique_ptr<Genotype> spawnNewGenotypeThroughRoulette() const;
		std::unique_ptr<Genotype> getDeepCopyOfSpeciesLeader() const;
		double getTotalCurrentAdjustedFitness() const;
		int getGenerationOfSpecies() const;
		int getSpeciesId() const;
		int getMemberCount() const;
		int getSpawnAmount() const;
		int getGenerationNoImprovement() const;
		friend bool operator<(const Species& lhs, const Species& rhs);

	private:
		std::unique_ptr<Genotype> m_representative;
		std::vector<Genotype*> m_members; // TODO can be made const * ???

		int m_speciesId;
		int m_generationsNoImprovement = 0;
		double m_highestAdjustedFitnessEver = 0;
		int m_currentGeneration = 0;
		double m_spawnAmount = 0;
		double m_totalCurrentAdjustedFitness = 0;
	};
}