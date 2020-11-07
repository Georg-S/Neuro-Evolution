#pragma once
#include "Genotype.h"
#include "Species.h"
#include "Innovation.h"
#include "FileReader.h"
#include "FileWriter.h"
#include <string>
#include <vector>
#include <time.h>


class NEAT
{
public:
	NEAT();
	NEAT(string populationFileName, string innovationFileName);
	NEAT(int populationSize, int countOfInputs, int countOfOutputs);
	~NEAT();

	std::vector<std::vector<double>> calculateOutputSnapshot(const std::vector<double> &inputs);
	std::vector<std::vector<double>> calculateOutputActive(const std::vector<double>&inputs);
	std::vector<double> calculateOutputActiveOfSpecificGenotype(const std::vector<double>&inputs, int index);
	void iterateOneGeneration(const std::vector<double> &fitness);
	void writePopulationAndInnovationAsFiles(string populationFileName, string innovationFileName);


	int getPopulationSize();
	int getSpeciesCount();
	int getTotalCountOfInnovations();
	int getHighestGenotypeId();
	int getCurrentGeneration();

	void setWeightPertubation(double weightPertubation);
	void setAddNeuronProbability(double addNeuronProbability);
private:
	void deletePhenotypes();
	void resetSpecies();
	void setFitnessOfPopulation(const std::vector<double> &fitness);
	void speciate();
	void updateSpecies();
	void updateFitnessValues();
	void calculateSpawnAmoutOfSpecies();
	void populate();
	Genotype getHighestRawFitnessGenotyp();

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
	FileReader fileReader;
	FileWriter fileWriter;
	Innovation innovation;
	int currentGenotypeId = 0;
	int maxPopulationSize = 0;
	int countOfInputs = 0;
	int countOfOutputs = 0;
	int currentPopulationId = 0;
	int currentGeneration = 0;
};

