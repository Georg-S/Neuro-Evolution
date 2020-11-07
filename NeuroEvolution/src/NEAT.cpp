#include "NEAT.h"


NEAT::NEAT()
{
}

NEAT::NEAT(string populationFileName, string innovationFileName)
{
	this->countOfInputs = countOfInputs;
	this->countOfOutputs = countOfOutputs;
	fileReader = FileReader();
	population = fileReader.parsePopulationFromFile(populationFileName);
	innovation = fileReader.parseInnovationFromFile(innovationFileName);

	if (population.size() > 0) {
		this->countOfInputs = population[0].getCountOfInputs();
		this->countOfOutputs = population[0].getCountOfOutputs();
	}
}

NEAT::NEAT(int populationSize, int countOfInputs, int countOfOutputs)
{
	if (populationSize <= 0)
		return;

	innovation = Innovation();
	for (int i = 0; i < populationSize; i++) {
		population.push_back(Genotype(innovation, countOfInputs, countOfOutputs, currentGenotypeId++));
	}
	maxPopulationSize = population.size();
	this->countOfInputs = countOfInputs;
	this->countOfOutputs = countOfOutputs;
	fileWriter = FileWriter();
	std::srand(time(NULL));
}


NEAT::~NEAT()
{
}

std::vector<std::vector<double>> NEAT::calculateOutputSnapshot(const std::vector<double>& inputs)
{
	if(inputs.size() != countOfInputs)
		return std::vector<std::vector<double>>();
	
	std::vector<std::vector<double>> outputs;
	for (int i = 0; i < population.size(); i++) {
		outputs.push_back(population[i].calculateOutputSnapshot(inputs));
	}

	return outputs;
}

std::vector<std::vector<double>> NEAT::calculateOutputActive(const std::vector<double>& inputs)
{
	if (inputs.size() != countOfInputs)
		return std::vector<std::vector<double>>();

	std::vector<std::vector<double>> outputs;
	for (int i = 0; i < population.size(); i++) {
		outputs.push_back(population[i].calculateOutputActive(inputs));
	}

	return outputs;
}

std::vector<double> NEAT::calculateOutputActiveOfSpecificGenotype(const std::vector<double>& inputs, int index)
{
	if (inputs.size() != countOfInputs)
		return std::vector<double>();

	if (index >= population.size() || index < 0)
		return std::vector<double>();
		
	return population[index].calculateOutputActive(inputs);
}

void NEAT::iterateOneGeneration(const std::vector<double>& fitness)
{
	if (fitness.size() != population.size())
		return;

	deletePhenotypes();
	resetSpecies();
	setFitnessOfPopulation(fitness);
	speciate();
	updateFitnessValues();
	updateSpecies();
	calculateSpawnAmoutOfSpecies();
	populate();
	currentGeneration++;
}

void NEAT::writePopulationAndInnovationAsFiles(string populationFileName, string innovationFileName)
{
	fileWriter.writePopulationToFile(populationFileName,population);
	fileWriter.writeInnovationToFile(innovationFileName,innovation);
}

int NEAT::getPopulationSize()
{
	return population.size();
}

int NEAT::getSpeciesCount()
{
	return species.size();
}

int NEAT::getTotalCountOfInnovations()
{
	return innovation.getTotalInnovationsCount();
}

int NEAT::getHighestGenotypeId()
{
	int highestId = -1;
	for (int i = 0; i < population.size(); i++) {
		if (population[i].getGenotypeId() > highestId)
			highestId = population[i].getGenotypeId();
	}

	return highestId;
}

int NEAT::getCurrentGeneration()
{
	return currentGeneration;
}

void NEAT::setWeightPertubation(double weightPertubation)
{
	this->weightPertubation = weightPertubation;
}

void NEAT::setAddNeuronProbability(double addNeuronProbability)
{
	this->addNeuronProbability = addNeuronProbability;
}

void NEAT::deletePhenotypes()
{
	for (int i = 0; i < population.size(); i++)
		population[i].deletePhenotype();
}

void NEAT::resetSpecies()
{
	for (int i = 0; i < species.size(); i++) {
		species[i].reset();
	}
}

void NEAT::setFitnessOfPopulation(const std::vector<double>& fitness)
{
	for (int i = 0; i < population.size(); i++) {
		population[i].setRawFitness(fitness[i]);
	}
}

void NEAT::speciate()
{
	for (int i = 0; i < population.size(); i++) {
		double lowestCompatibilityScore = compatibilityDistanceThreshold+1;
		int lowestCompIndex = 0;

		for (int speciesIndex = 0; speciesIndex < species.size(); speciesIndex++) {
			double compatibilityScore = species[speciesIndex].calculateCompatibilityScore(population[i],
																exzessFactor, disjointFactor, weightFactor);

			if (compatibilityScore < lowestCompatibilityScore) {
				lowestCompatibilityScore = compatibilityScore;
				lowestCompIndex = speciesIndex;
			}
		}
		if (lowestCompatibilityScore <= compatibilityDistanceThreshold)
			species[lowestCompIndex].addMemberToSpecies(&population[i]);
		else
			species.push_back(Species(population[i], currentPopulationId++));
	}
}

void NEAT::updateSpecies()
{
	sort(species.begin(), species.end());
	std::vector<Species> updatedSpecies;

	for (int i = 0; i < species.size(); i++) {
		if (species[i].getMemberCount() > 0 ) {
			if (updatedSpecies.size() > speciesRoughValue && species[i].getGenerationNoImprovement() > generationsNoImprovementAllowed 
				|| updatedSpecies.size() >= maxCountSpecies)
				continue;

			species[i].incrementCurrentGeneration();
			updatedSpecies.push_back(species[i]);
		}
	}
	species = updatedSpecies;
}

void NEAT::populate()
{
	std::vector<Genotype> newPopulation;
	int currentSpawnedAmount = 0;
	Genotype baby;

	Genotype highestRawFitnessGenotype = getHighestRawFitnessGenotyp();
	newPopulation.push_back(highestRawFitnessGenotype);
	currentSpawnedAmount++;

	for (int speciesIndex = 0; speciesIndex < species.size(); speciesIndex++) {
		if (currentSpawnedAmount < maxPopulationSize) {
			int countToSpawn = round(species[speciesIndex].getSpawnAmount());

			bool elitSpawnedAlready = false;

			while (countToSpawn--) {
				if (!elitSpawnedAlready && species[speciesIndex].getMemberCount() >= 5) {
					elitSpawnedAlready = true;
					baby = species[speciesIndex].getLeader();
				}
				else {
					if (species[speciesIndex].getMemberCount() == 1) {
						baby = species[speciesIndex].spawnGenotype();
					}
					else {
						if (crossOverProbability > RNG::getRandomFloatBetween0and1()) {
							Genotype father = species[speciesIndex].spawnGenotype();
							Genotype mother = species[speciesIndex].spawnGenotype();
							baby = father.crossOver(mother, currentGenotypeId++);
						}else {
							baby = species[speciesIndex].spawnGenotype();
						}
						baby.randomlyMutateAllWeights(mutateLinkProbability, newLinkWeightProbability, weightPertubation);
						baby.randomlyAddLink(innovation, addLinkProbability, recurrentAllowed);
						baby.randomlyAddNeuron(innovation, addNeuronProbability);
					}
				}
				currentSpawnedAmount++;
				newPopulation.push_back(baby);
				if (currentSpawnedAmount == maxPopulationSize)
					countToSpawn = 0;
			}
		}
	}
	while (currentSpawnedAmount < maxPopulationSize) {
		currentSpawnedAmount++;
		newPopulation.push_back(Genotype(innovation, countOfInputs, countOfOutputs, currentGenotypeId++));
	}
	population.clear();
	population = newPopulation;
}

Genotype NEAT::getHighestRawFitnessGenotyp()
{
	double highestRawFitness = -1;
	int highestRawFitnessIndex = 0;

	for (int i = 0; i < population.size(); i++) {
		if (population[i].getRawFitness() > highestRawFitness) {
			highestRawFitnessIndex = i;
			highestRawFitness = population[i].getRawFitness();
		}
	}
	return population[highestRawFitnessIndex];
}

void NEAT::updateFitnessValues()
{
	for (int i = 0; i < species.size(); i++) {
		species[i].updateFitnessValues();
	}
}

void NEAT::calculateSpawnAmoutOfSpecies()
{
	double populationAverageFitness = 0;
	double populationTotalFitness = 0;
	for (int i = 0; i < population.size(); i++) {
		populationTotalFitness += population[i].getAdjustedFitness();
	}
	populationAverageFitness = populationTotalFitness / population.size();

	for (int i = 0; i < species.size(); i++) {
		species[i].calculateSpawnAmount(populationAverageFitness);
	}
}
