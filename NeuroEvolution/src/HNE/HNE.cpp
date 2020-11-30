#include "HNE/HNE.h"


nev::HNE::HNE(int centuryDuration, int populationSize, int countOfInputs, int countOfOutputs)
{
	this->centuryDuration = centuryDuration;
	this->populationSize = populationSize;
	this->countOfInputs = countOfInputs;
	this->countOfOutputs = countOfOutputs;
	this->innovation = std::make_shared<nev::Innovation>();

	for (int i = 0; i < populationSize; i++) {
		population.push_back(nev::HistoricalGenotype(*innovation,countOfInputs, countOfOutputs));
	}
	setParametersOfPopulation();

	srand(time(NULL));
}

nev::HNE::HNE(vector<HistoricalGenotype> population, int countOfInputs, int countOfOutputs)
{
	this->population = population;
	this->countOfInputs = countOfInputs;
	this->countOfOutputs = countOfOutputs;
	setParametersOfPopulation();
	srand(time(NULL));
}

nev::HNE::~HNE()
{
	
	for (int i = 0; i < population.size(); i++)
		population[i].deletePhenotype();
}

vector<vector<double>> nev::HNE::getOutputs(const vector<double> &inputs)
{
	vector<vector<double>> outputs;

	if (inputs.size() != countOfInputs)
		return outputs;

	for (int i = 0; i < population.size(); i++) {
		outputs.push_back(population[i].calculateOutputSnapshotFromLastGenotype(inputs));
	}


	return outputs;
}

void nev::HNE::iterateOneGeneration(const vector<double>& fitness)
{
	if (fitness.size() != population.size())
		return;

	for (int i = 0; i < population.size(); i++) {
		population[i].deletePhenotype();
	}

	for (int i = 0; i < population.size(); i++) {
		population[i].setFitness(fitness[i]);
	}
	
	if (currentCentury < centuryDuration) {
		for (int i = 0; i < population.size(); i++) {
			population[i].iterate(*innovation);
		}
		currentCentury++;
	}
	else {
		int elitismCount = calculateElitismCount();
		int index = 0;
		sort(population.begin(), population.end());

		while (index < elitismCount) {
			population[index].purgeAllExceptHighestPerformingGenotype(*innovation);
			index++;
		}
		while (index < population.size()) {
			population[index].evolution(*innovation);
			index++;
		}
		currentCentury = 0;
	}
	currentGeneration++;
}

vector<double> nev::HNE::getOutputActiveByIndex(int index, const vector<double> &inputs)
{
	vector<double> output;

	if (index >= population.size()) {
		return output;
	}

	output = population[index].calculateOutputActiveFromLastGenotype(inputs);

	return output;
}

int nev::HNE::calculateElitismCount()
{
	return population.size() * elitismPercentage;
}

void nev::HNE::setParametersOfPopulation()
{
	for (int i = 0; i < population.size(); i++) {
		population[i].setAddLinkProbability(addLinkProbability);
		population[i].setAddNeuronProbability(addNeuronProbability);
		population[i].setMaxWeightPertubation(maxWeightPertubation);
		population[i].setMutateWeightProbability(mutateWeightProbability);
		population[i].setNewWeightProbability(newWeightProbability);
		population[i].setRecurrentAllowed(recurrentAllowed);
	}
}

int nev::HNE::getPopulationSize()
{
	return population.size();
}

void nev::HNE::setHighestFitnessAtStartOfCentury(double highestFitnessAtStartOfCentury)
{
	for (int i = 0; i < population.size(); i++) {
		population[i].setHighestFitnessAtStartOfCentury(highestFitnessAtStartOfCentury);
	}
}

void nev::HNE::setCenturyDuration(int centuryDuration)
{
	this->centuryDuration = centuryDuration;
}

void nev::HNE::setAddLinkProbability(double addLinkProbability)
{
	this->addLinkProbability = addLinkProbability;

	for (int i = 0; i < population.size(); i++) {
		population[i].setAddLinkProbability(addLinkProbability);
	}
}

void nev::HNE::setAddNeuronProbability(double addNeuronProbability)
{
	this->addNeuronProbability = addNeuronProbability;

	for (int i = 0; i < population.size(); i++) {
		population[i].setAddNeuronProbability(addNeuronProbability);
	}
}

void nev::HNE::setMutateWeightProbability(double mutateWeightProbability)
{
	this->mutateWeightProbability = mutateWeightProbability;

	for (int i = 0; i < population.size(); i++) {
		population[i].setMutateWeightProbability(mutateWeightProbability);
	}
}

void nev::HNE::setNewWeightProbability(double newWeightProbability)
{
	this->newWeightProbability = newWeightProbability;

	for (int i = 0; i < population.size(); i++) {
		population[i].setNewWeightProbability(newWeightProbability);
	}
}

void nev::HNE::setMaxWeightPertubation(double maxWeightPertubation)
{
	this->maxWeightPertubation = maxWeightPertubation;

	for (int i = 0; i < population.size(); i++) {
		population[i].setMaxWeightPertubation(maxWeightPertubation);
	}
}

void nev::HNE::setRecurrentAllowed(bool recurrentAllowed)
{
	this->recurrentAllowed = recurrentAllowed;

	for (int i = 0; i < population.size(); i++) {
		population[i].setRecurrentAllowed(recurrentAllowed);
	}
}

void nev::HNE::setElitismPercentage(double elitismPercentage)
{
	this->elitismPercentage = elitismPercentage;
}

double nev::HNE::getHighestFitness()
{
	double highestFitness = 0;

	for (int i = 0; i < population.size(); i++) {
		double fitness = population[i].getHighestFitness();

		if (fitness > highestFitness)
			highestFitness = fitness;
	}
	return highestFitness;
}

int nev::HNE::getCurrentGeneration()
{
	return currentGeneration;
}
