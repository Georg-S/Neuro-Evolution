#pragma once
#include <string>
#include <vector>
#include <time.h>
#include <functional>
#include <memory>
#include "Genotype.h"
#include "Species.h"
#include "Innovation.h"
#include "ActivationFunction.h"

namespace nev
{
	class NEAT
	{
	public:
		NEAT() = delete;
		NEAT(std::vector<std::unique_ptr<Genotype>>&& population, Innovation&& innovation);
		NEAT(int populationSize, int countOfInputs, int countOfOutputs,
			nev::af activationFunction = nev::af::steepenedSigmoid);
		std::vector<std::vector<double>> calculateOutputSnapshot(const std::vector<double>& inputs);
		std::vector<std::vector<double>> calculateOutputActive(const std::vector<double>& inputs);
		std::vector<double> calculateOutputActiveOfSpecificGenotype(const std::vector<double>& inputs, int index);
		void iterateOneGeneration(const std::vector<double>& fitness);
		int getPopulationSize() const;
		int getSpeciesCount() const;
		int getTotalCountOfInnovations() const;
		int getHighestGenotypeId() const;
		int getCurrentGeneration() const;
		double getWeightPertubation() const;
		int getHiddenNeuronCountOfSpecificGenotype(int index) const;
		int getCountOfEnabledLinksOfSpecificGenotype(int index) const;
		void setWeightPertubation(double weightPertubation);
		void setAddNeuronProbability(double addNeuronProbability);
		void refreshPopulationActivationFunction();

	private:
		void deletePhenotypes();
		void resetSpecies();
		void setFitnessOfPopulation(const std::vector<double>& fitness);
		void speciate();
		void updateSpecies();
		void updateFitnessValues();
		void calculateSpawnAmoutOfSpecies();
		void populate();
		std::unique_ptr<Genotype> getHighestRawFitnessGenotype() const;

		//NEAT-Parameters
		double m_excessFactor = 1;
		double m_disjointFactor = 1;
		double m_weightFactor = 0.4;
		double m_compatibilityDistanceThreshold = 3;
		int m_generationsNoImprovementAllowed = 10;
		int m_speciesRoughValue = 5;
		int m_maxCountSpecies = 20;
		bool m_recurrentAllowed = true;

		//Probabilities
		double m_crossOverProbability = 0.1;
		double m_addNeuronProbability = 0.03;
		double m_addLinkProbability = 0.05;
		double m_mutateLinkProbability = 0.8;
		double newLinkWeightProbability = 0.1;

		double weightPertubation = 0.1;

		nev::af m_activationFunction = nev::af::steepenedSigmoid;
		std::vector<std::unique_ptr<Genotype>> m_population;
		std::vector<Species> m_species;
		Innovation m_innovation;
		int m_currentGenotypeId = 0;
		int m_maxPopulationSize = 0;
		int m_countOfInputs = 0;
		int m_countOfOutputs = 0;
		int m_currentPopulationId = 0;
		int m_currentGeneration = 0;

		friend class FileIO;
	};
}