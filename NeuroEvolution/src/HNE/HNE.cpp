#include "HNE/HNE.h"


nev::HNE::HNE(int centuryDuration, int populationSize, int countOfInputs, int countOfOutputs)
	: m_centuryDuration(centuryDuration)
	, m_populationSize(populationSize)
	, m_countOfInputs(countOfInputs)
	, m_countOfOutputs(countOfOutputs)
	, m_innovation(std::make_unique<nev::Innovation>())
{
	for (int i = 0; i < populationSize; i++)
		m_population.emplace_back(m_innovation.get(), countOfInputs, countOfOutputs);

	setParametersOfPopulation();
}

nev::HNE::HNE(const std::vector<HistoricalGenotype>& population, int countOfInputs, int countOfOutputs)
	: m_population(population)
	, m_countOfInputs(countOfInputs)
	, m_countOfOutputs(countOfOutputs)
{
	setParametersOfPopulation();
}

std::vector<std::vector<double>> nev::HNE::getOutputsSnapshot(const std::vector<double>& inputs)
{
	if (inputs.size() != m_countOfInputs)
	{
		assert(!"Input size mismatch");
		return {};
	}

	std::vector<std::vector<double>> outputs;

	for (auto& historicalGenotype : m_population)
		outputs.emplace_back(historicalGenotype.calculateOutputSnapshotFromLastGenotype(inputs));

	return outputs;
}

void nev::HNE::iterateOneGeneration(const std::vector<double>& fitness)
{
	if (fitness.size() != m_population.size())
	{
		assert(!"Input size mismatch");
		return;
	}

	for (int i = 0; i < m_population.size(); i++)
	{
		m_population[i].deletePhenotype();
		m_population[i].setFitness(fitness[i]);
	}

	m_currentGeneration++;

	if (m_currentCentury < m_centuryDuration)
	{
		for (auto& historicalGenotype : m_population)
			historicalGenotype.iterate(m_innovation.get());

		m_currentCentury++;
		return;
	}

	int elitismCount = calculateElitismCount();
	int index = 0;
	sort(m_population.begin(), m_population.end());

	while (index < elitismCount)
	{
		m_population[index].purgeAllExceptHighestPerformingGenotype(m_innovation.get());
		index++;
	}

	while (index < m_population.size())
	{
		m_population[index].evolution(m_innovation.get());
		index++;
	}

	m_currentCentury = 0;
}

std::vector<double> nev::HNE::getOutputActiveByIndex(int index, const std::vector<double>& inputs)
{
	if (index >= m_population.size())
		return {};

	return m_population[index].calculateOutputActiveFromLastGenotype(inputs);
}

int nev::HNE::calculateElitismCount()
{
	return m_population.size() * m_elitismPercentage;
}

void nev::HNE::setParametersOfPopulation()
{
	for (auto& historicalGenotype : m_population)
	{
		historicalGenotype.setAddLinkProbability(m_addLinkProbability);
		historicalGenotype.setAddNeuronProbability(m_addNeuronProbability);
		historicalGenotype.setMaxWeightPertubation(m_maxWeightPertubation);
		historicalGenotype.setMutateWeightProbability(m_mutateWeightProbability);
		historicalGenotype.setNewWeightProbability(m_newWeightProbability);
		historicalGenotype.setRecurrentAllowed(m_recurrentAllowed);
	}
}

int nev::HNE::getPopulationSize() const
{
	return m_population.size();
}

void nev::HNE::setHighestFitnessAtStartOfCentury(double highestFitnessAtStartOfCentury)
{
	for (auto& historicalGenotype : m_population)
		historicalGenotype.setHighestFitnessAtStartOfCentury(highestFitnessAtStartOfCentury);
}

void nev::HNE::setCenturyDuration(int centuryDuration)
{
	m_centuryDuration = centuryDuration;
}

void nev::HNE::setAddLinkProbability(double addLinkProbability)
{
	m_addLinkProbability = addLinkProbability;

	for (auto& historicalGenotype : m_population)
		historicalGenotype.setAddLinkProbability(addLinkProbability);
}

void nev::HNE::setAddNeuronProbability(double addNeuronProbability)
{
	m_addNeuronProbability = addNeuronProbability;

	for (auto& historicalGenotype : m_population)
		historicalGenotype.setAddNeuronProbability(addNeuronProbability);
}

void nev::HNE::setMutateWeightProbability(double mutateWeightProbability)
{
	m_mutateWeightProbability = mutateWeightProbability;

	for (auto& historicalGenotype : m_population)
		historicalGenotype.setMutateWeightProbability(mutateWeightProbability);
}

void nev::HNE::setNewWeightProbability(double newWeightProbability)
{
	m_newWeightProbability = newWeightProbability;

	for (auto& historicalGenotype : m_population)
		historicalGenotype.setNewWeightProbability(newWeightProbability);
}

void nev::HNE::setMaxWeightPertubation(double maxWeightPertubation)
{
	m_maxWeightPertubation = maxWeightPertubation;

	for (auto& historicalGenotype : m_population)
		historicalGenotype.setMaxWeightPertubation(maxWeightPertubation);
}

void nev::HNE::setRecurrentAllowed(bool recurrentAllowed)
{
	m_recurrentAllowed = recurrentAllowed;

	for (auto& historicalGenotype : m_population)
		historicalGenotype.setRecurrentAllowed(recurrentAllowed);
}

void nev::HNE::setElitismPercentage(double elitismPercentage)
{
	m_elitismPercentage = elitismPercentage;
}

double nev::HNE::getHighestFitness() const
{
	double highestFitness = -DBL_MAX;

	for (auto& historicalGenotype : m_population)
		highestFitness = std::max(highestFitness, historicalGenotype.getHighestFitness());

	return highestFitness;
}

int nev::HNE::getCurrentGeneration() const
{
	return m_currentGeneration;
}
