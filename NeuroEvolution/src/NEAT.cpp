#include "NEAT.h"


nev::NEAT::NEAT()
{
}

nev::NEAT::NEAT(const std::string& populationFileName, const std::string& innovationFileName,
	std::function<double(const double& input)> activationFunction)
{
	population = nev::FileReader::parsePopulationFromFile(populationFileName);
	innovation = nev::FileReader::parseInnovationFromFile(innovationFileName);
	this->activationFunction = activationFunction;

	if (population.size() > 0) {
		this->countOfInputs = population[0].getCountOfInputs();
		this->countOfOutputs = population[0].getCountOfOutputs();
	}
}

nev::NEAT::NEAT(const int& populationSize, const int& countOfInputs, const int& countOfOutputs,
	std::function<double(const double& input)> activationFunction)
{
	std::srand(time(NULL));
	if (populationSize <= 0)
		return;
	this->activationFunction = activationFunction;
	innovation = Innovation();
	for (int i = 0; i < populationSize; i++)
		population.push_back(Genotype(innovation, countOfInputs, countOfOutputs, currentGenotypeId++, activationFunction));

	maxPopulationSize = population.size();
	this->countOfInputs = countOfInputs;
	this->countOfOutputs = countOfOutputs;
}

std::vector<std::vector<double>> nev::NEAT::calculateOutputSnapshot(const std::vector<double>& inputs)
{
	if (inputs.size() != countOfInputs)
		return std::vector<std::vector<double>>();

	std::vector<std::vector<double>> outputs;
	for (int i = 0; i < population.size(); i++)
		outputs.push_back(population[i].calculateOutputSnapshot(inputs));

	return outputs;
}

std::vector<std::vector<double>> nev::NEAT::calculateOutputActive(const std::vector<double>& inputs)
{
	if (inputs.size() != countOfInputs)
		return std::vector<std::vector<double>>();

	std::vector<std::vector<double>> outputs;
	for (int i = 0; i < population.size(); i++)
		outputs.push_back(population[i].calculateOutputActive(inputs));

	return outputs;
}

std::vector<double> nev::NEAT::calculateOutputActiveOfSpecificGenotype(const std::vector<double>& inputs, const int& index)
{
	if (inputs.size() != countOfInputs)
		return std::vector<double>();

	if (index >= population.size() || index < 0)
		return std::vector<double>();

	return population[index].calculateOutputActive(inputs);
}

void nev::NEAT::iterateOneGeneration(const std::vector<double>& fitness)
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

int nev::NEAT::getPopulationSize() const
{
	return population.size();
}

int nev::NEAT::getSpeciesCount() const
{
	return species.size();
}

int nev::NEAT::getTotalCountOfInnovations() const
{
	return innovation.getTotalInnovationsCount();
}

int nev::NEAT::getHighestGenotypeId() const
{
	int highestId = -1;
	for (int i = 0; i < population.size(); i++) {
		if (population[i].getGenotypeId() > highestId)
			highestId = population[i].getGenotypeId();
	}

	return highestId;
}

int nev::NEAT::getCurrentGeneration() const
{
	return currentGeneration;
}

void nev::NEAT::setWeightPertubation(const double& weightPertubation)
{
	this->weightPertubation = weightPertubation;
}

void nev::NEAT::setAddNeuronProbability(const double& addNeuronProbability)
{
	this->addNeuronProbability = addNeuronProbability;
}

void nev::NEAT::deletePhenotypes()
{
	for (int i = 0; i < population.size(); i++)
		population[i].deletePhenotype();
}

void nev::NEAT::resetSpecies()
{
	for (int i = 0; i < species.size(); i++)
		species[i].reset();
}

void nev::NEAT::setFitnessOfPopulation(const std::vector<double>& fitness)
{
	for (int i = 0; i < population.size(); i++)
		population[i].setRawFitness(fitness[i]);
}

void nev::NEAT::speciate()
{
	for (int i = 0; i < population.size(); i++) {
		double lowestCompatibilityScore = DBL_MAX;
		int lowestCompIndex = -1;

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

void nev::NEAT::updateSpecies()
{
	sort(species.begin(), species.end());
	std::vector<Species> updatedSpecies;

	for (int i = 0; i < species.size(); i++) {
		if (species[i].getMemberCount() > 0) {
			if (updatedSpecies.size() >= maxCountSpecies)
				break;
			if ((updatedSpecies.size() > speciesRoughValue) && (species[i].getGenerationNoImprovement() > generationsNoImprovementAllowed))
				continue;

			species[i].incrementCurrentGeneration();
			updatedSpecies.push_back(species[i]);
		}
	}
	species = updatedSpecies;
}

void nev::NEAT::populate()
{
	std::vector<Genotype> newPopulation;
	int currentSpawnedAmount = 0;
	Genotype baby;

	Genotype highestRawFitnessGenotype = getHighestRawFitnessGenotyp();
	newPopulation.push_back(highestRawFitnessGenotype);
	currentSpawnedAmount++;

	for (int speciesIndex = 0; speciesIndex < species.size(); speciesIndex++) {

		if (currentSpawnedAmount == maxPopulationSize)
			break;

		int spawnAmount = round(species[speciesIndex].getSpawnAmount());
		bool eliteSpawnedAlready = false;


		for (int countToSpawn = 0; countToSpawn < spawnAmount; countToSpawn++) {
			if (!eliteSpawnedAlready && species[speciesIndex].getMemberCount() >= 5) {
				eliteSpawnedAlready = true;
				baby = species[speciesIndex].getLeader();
			}
			else {
				if (species[speciesIndex].getMemberCount() == 1) {
					baby = species[speciesIndex].spawnGenotype();
				}
				else {
					if (RNG::getRandomFloatBetween0and1() < crossOverProbability) {
						Genotype father = species[speciesIndex].spawnGenotype();
						Genotype mother = species[speciesIndex].spawnGenotype();
						baby = Genotype::crossOver(father, mother, currentGenotypeId++);
					}
					else {
						baby = species[speciesIndex].spawnGenotype();
						baby.randomlyMutateAllWeights(mutateLinkProbability, newLinkWeightProbability, weightPertubation);
						baby.randomlyAddLink(innovation, addLinkProbability, recurrentAllowed);
						baby.randomlyAddNeuron(innovation, addNeuronProbability);
					}
				}
			}
			currentSpawnedAmount++;
			newPopulation.push_back(baby);
			if (currentSpawnedAmount == maxPopulationSize)
				break;
		}
	}

	while (currentSpawnedAmount < maxPopulationSize) {
		newPopulation.push_back(Genotype(innovation, countOfInputs, countOfOutputs, currentGenotypeId++, activationFunction));
		currentSpawnedAmount++;
	}
	population.clear();
	population = newPopulation;
}

nev::Genotype nev::NEAT::getHighestRawFitnessGenotyp() const
{
	double highestRawFitness = DBL_MIN;
	int highestRawFitnessIndex = -1;

	for (int i = 0; i < population.size(); i++) {
		if (population[i].getRawFitness() > highestRawFitness) {
			highestRawFitnessIndex = i;
			highestRawFitness = population[i].getRawFitness();
		}
	}
	return population[highestRawFitnessIndex];
}

void nev::NEAT::updateFitnessValues()
{
	for (int i = 0; i < species.size(); i++)
		species[i].updateFitnessValues();
}

void nev::NEAT::calculateSpawnAmoutOfSpecies()
{
	double populationAverageFitness = 0;
	double populationTotalFitness = 0;
	for (int i = 0; i < population.size(); i++)
		populationTotalFitness += population[i].getAdjustedFitness();

	populationAverageFitness = populationTotalFitness / (double)population.size();

	for (int i = 0; i < species.size(); i++)
		species[i].calculateSpawnAmount(populationAverageFitness);
}
