#pragma once
#include <string>
#include <vector>
#include <time.h>
#include "Genotype.h"
#include "Species.h"
#include "Innovation.h"
#include "FileReader.h"
#include "FileWriter.h"


class NEAT
{
public:
	NEAT();
	NEAT(const std::string &populationFileName, const std::string &innovationFileName);
	NEAT(const int &populationSize, const int &countOfInputs, const int &countOfOutputs);

	std::vector<std::vector<double>> calculateOutputSnapshot(const std::vector<double> &inputs);
	std::vector<std::vector<double>> calculateOutputActive(const std::vector<double>&inputs);
	std::vector<double> calculateOutputActiveOfSpecificGenotype(const std::vector<double>&inputs, const int &index);
	void iterateOneGeneration(const std::vector<double> &fitness);
	void writePopulationAndInnovationAsFiles(const std::string &populationFileName, const std::string &innovationFileName);


	int getPopulationSize() const;
	int getSpeciesCount() const;
	int getTotalCountOfInnovations() const;
	int getHighestGenotypeId() const;
	int getCurrentGeneration() const;

	void setWeightPertubation(const double &weightPertubation);
	void setAddNeuronProbability(const double &addNeuronProbability);
private:
	void deletePhenotypes();
	void resetSpecies();
	void setFitnessOfPopulation(const std::vector<double> &fitness);
	void speciate();
	void updateSpecies();
	void updateFitnessValues();
	void calculateSpawnAmoutOfSpecies();
	void populate();
	Genotype getHighestRawFitnessGenotyp() const;

//NEAT-Parameters
	double exzessFactor = 1;
	double disjointFactor = 1;
	double weightFactor = 0.4;
	double compatibilityDistanceThreshold = 3;
	int generationsNoImprovementAllowed = 10;
	int speciesRoughValue = 5;
	int maxCountSpecies = 20;
	bool recurrentAllowed = true;

	//Probabilitys
	double crossOverProbability = 0.1;
	double addNeuronProbability = 0.03;
	double addLinkProbability = 0.05;
	double mutateLinkProbability = 0.8;
	double newLinkWeightProbability = 0.1;

	double weightPertubation = 0.1;


	std::vector<Genotype> population;
	std::vector<Species> species;
	Innovation innovation;
	int currentGenotypeId = 0;
	int maxPopulationSize = 0;
	int countOfInputs = 0;
	int countOfOutputs = 0;
	int currentPopulationId = 0;
	int currentGeneration = 0;
};

