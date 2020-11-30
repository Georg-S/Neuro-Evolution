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

namespace nev {

	class NEAT
	{
	public:
		NEAT();
		NEAT(const std::vector<std::shared_ptr<Genotype>>& population, const Innovation& innovation);
		NEAT(int populationSize, int countOfInputs, int countOfOutputs,
			const nev::af& activationFunction = nev::af::steepenedSigmoid);
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
		std::shared_ptr<Genotype> getHighestRawFitnessGenotype() const;

		//NEAT-Parameters
		double excessFactor = 1;
		double disjointFactor = 1;
		double weightFactor = 0.4;
		double compatibilityDistanceThreshold = 3;
		int generationsNoImprovementAllowed = 10;
		int speciesRoughValue = 5;
		int maxCountSpecies = 20;
		bool recurrentAllowed = true;

		//Probabilities
		double crossOverProbability = 0.1;
		double addNeuronProbability = 0.03;
		double addLinkProbability = 0.05;
		double mutateLinkProbability = 0.8;
		double newLinkWeightProbability = 0.1;

		double weightPertubation = 0.1;

		nev::af activationFunction = nev::af::steepenedSigmoid;
		std::vector<std::shared_ptr<Genotype>> population;
		std::vector<Species> species;
		Innovation innovation;
		int currentGenotypeId = 0;
		int maxPopulationSize = 0;
		int countOfInputs = 0;
		int countOfOutputs = 0;
		int currentPopulationId = 0;
		int currentGeneration = 0;

		friend class FileWriter;
		friend class FileReader;
	};

}