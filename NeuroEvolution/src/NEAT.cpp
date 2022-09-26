#include "NEAT.h"


nev::NEAT::NEAT(std::vector<std::unique_ptr<Genotype>>&& population, Innovation&& innovation)
	: m_population(std::move(population))
	, m_innovation(std::move(innovation))
{
	assert(!m_population.empty());
	if (!m_population.empty())
	{
		m_countOfInputs = m_population[0]->getCountOfInputs();
		m_countOfOutputs = m_population[0]->getCountOfOutputs();
	}
	m_maxPopulationSize = m_population.size();
	m_currentGenotypeId = m_population.size();
}

nev::NEAT::NEAT(int populationSize, int countOfInputs, int countOfOutputs, nev::af activationFunction)
{
	if (populationSize <= 0)
		return;

	m_activationFunction = activationFunction;
	m_innovation = Innovation();
	for (int i = 0; i < populationSize; i++)
		m_population.emplace_back(std::make_unique<Genotype>(&m_innovation, countOfInputs, countOfOutputs, m_currentGenotypeId++, activationFunction));

	m_maxPopulationSize = m_population.size();
	m_countOfInputs = countOfInputs;
	m_countOfOutputs = countOfOutputs;
}

std::vector<std::vector<double>> nev::NEAT::calculateOutputSnapshot(const std::vector<double>& inputs)
{
	if (inputs.size() != m_countOfInputs)
	{
		assert(!"Input size mismatch");
		return {};
	}

	std::vector<std::vector<double>> outputs;
	for (const auto& genotype : m_population)
		outputs.emplace_back(genotype->calculateOutputSnapshot(inputs));

	return outputs;
}

std::vector<std::vector<double>> nev::NEAT::calculateOutputActive(const std::vector<double>& inputs)
{
	if (inputs.size() != m_countOfInputs)
	{
		assert(!"Input size mismatch");
		return {};
	}

	std::vector<std::vector<double>> outputs;
	for (const auto& genotype : m_population)
		outputs.emplace_back(genotype->calculateOutputActive(inputs));

	return outputs;
}

std::vector<double> nev::NEAT::calculateOutputActiveOfSpecificGenotype(const std::vector<double>& inputs, int index)
{
	if ((inputs.size() != m_countOfInputs) || (index >= m_population.size()) || (index < 0))
	{
		assert(!"Invalid input");
		return {};
	}

	return m_population[index]->calculateOutputActive(inputs);
}

void nev::NEAT::iterateOneGeneration(const std::vector<double>& fitness)
{
	if (fitness.size() != m_population.size())
	{
		//assert(!"Input size mismatch");
		return;
	}

	resetSpecies();
	setFitnessOfPopulation(fitness);
	speciate();
	updateFitnessValues();
	updateSpecies();
	calculateSpawnAmoutOfSpecies();
	populate();
	deletePhenotypes();
	m_currentGeneration++;
}

int nev::NEAT::getPopulationSize() const
{
	return m_population.size();
}

int nev::NEAT::getSpeciesCount() const
{
	return m_species.size();
}

int nev::NEAT::getTotalCountOfInnovations() const
{
	return m_innovation.getTotalInnovationsCount();
}

int nev::NEAT::getHighestGenotypeId() const
{
	int highestId = -1;
	for (const auto& genotype : m_population)
		highestId = std::max(highestId, genotype->getGenotypeId());

	return highestId;
}

int nev::NEAT::getCurrentGeneration() const
{
	return m_currentGeneration;
}

double nev::NEAT::getWeightPertubation() const
{
	return weightPertubation;
}

int nev::NEAT::getHiddenNeuronCountOfSpecificGenotype(int index) const
{
	if ((index >= 0) && (index < m_population.size()))
		return m_population[index]->getCountOfHiddenNeurons();
	return -1;
}

int nev::NEAT::getCountOfEnabledLinksOfSpecificGenotype(int index) const
{
	if ((index >= 0) && (index < m_population.size()))
		return m_population[index]->getCountOfEnabledLink();
	return -1;
}

void nev::NEAT::setWeightPertubation(double weightPertubation)
{
	weightPertubation = std::clamp(weightPertubation, 0., 1.);
}

void nev::NEAT::setAddNeuronProbability(double addNeuronProbability)
{
	addNeuronProbability = std::clamp(addNeuronProbability, 0., 1.);
}

void nev::NEAT::refreshPopulationActivationFunction()
{
	for (auto& geno : m_population)
		geno->setActivationFunction(m_activationFunction);
}

void nev::NEAT::deletePhenotypes()
{
	for (auto& genotype : m_population)
		genotype->deletePhenotype();
}

void nev::NEAT::resetSpecies()
{
	for (auto& species : m_species)
		species.reset();
}

void nev::NEAT::setFitnessOfPopulation(const std::vector<double>& fitness)
{
	for (int i = 0; i < m_population.size(); i++)
		m_population[i]->setRawFitness(fitness[i]);
}

void nev::NEAT::speciate()
{
	assert(m_compatibilityDistanceThreshold < DBL_MAX);

	for (auto& genotype : m_population)
	{
		double lowestCompatibilityScore = DBL_MAX;
		Species* lowestCompSpecies = nullptr;
		int lowestCompIndex = -1;

		for (auto& species : m_species)
		{
			double compatibilityScore = species.calculateCompatibilityScore(genotype.get(), m_excessFactor, m_disjointFactor, m_weightFactor);

			if (compatibilityScore < lowestCompatibilityScore)
			{
				lowestCompatibilityScore = compatibilityScore;
				lowestCompSpecies = &species;
			}
		}

		if (lowestCompatibilityScore <= m_compatibilityDistanceThreshold)
			lowestCompSpecies->addMemberToSpecies(genotype.get());
		else
			m_species.emplace_back(genotype.get(), m_currentPopulationId++);
	}
}

void nev::NEAT::updateSpecies()
{
	sort(m_species.begin(), m_species.end());
	std::vector<Species> updatedSpecies;

	for (auto& species : m_species)
	{
		if (species.getMemberCount() > 0)
		{
			if (updatedSpecies.size() >= m_maxCountSpecies)
				break;
			if ((updatedSpecies.size() > m_speciesRoughValue) && (species.getGenerationNoImprovement() > m_generationsNoImprovementAllowed))
				continue;

			species.incrementCurrentGeneration();
			updatedSpecies.emplace_back(std::move(species));
		}
	}
	m_species = std::move(updatedSpecies);
}

void nev::NEAT::populate()
{
	std::vector<std::unique_ptr<Genotype>> newPopulation;
	std::unique_ptr<Genotype> baby;
	int currentSpawnedAmount = 0;

	newPopulation.emplace_back(getHighestRawFitnessGenotype());
	currentSpawnedAmount++;

	for (const auto& species : m_species)
	{
		if (currentSpawnedAmount == m_maxPopulationSize)
			break;

		int spawnAmount = round(species.getSpawnAmount());
		bool spawnEliteOnce = species.getMemberCount() >= 5;

		for (int countToSpawn = 0; countToSpawn < spawnAmount; countToSpawn++)
		{
			if (spawnEliteOnce)
			{
				spawnEliteOnce = false;
				baby = species.getDeepCopyOfSpeciesLeader();
			}
			else
			{
				if (species.getMemberCount() == 1)
				{
					baby = species.spawnNewGenotypeThroughRoulette();
				}
				else
				{
					if (getRandomDouble(0, 1) < m_crossOverProbability)
					{
						auto father = species.getGenotypeThroughRoulette();
						auto mother = species.getGenotypeThroughRoulette();
						baby = Genotype::crossOver(father, mother, m_currentGenotypeId++);
					}
					else
					{
						baby = species.spawnNewGenotypeThroughRoulette();
						baby->randomlyMutateAllWeights(m_mutateLinkProbability, newLinkWeightProbability, weightPertubation);
						baby->randomlyAddLink(&m_innovation, m_addLinkProbability, m_recurrentAllowed);
						baby->randomlyAddNeuron(&m_innovation, m_addNeuronProbability);
					}
				}
			}
			currentSpawnedAmount++;
			newPopulation.emplace_back(std::move(baby));

			if (currentSpawnedAmount == m_maxPopulationSize)
				break;
		}
	}

	while (currentSpawnedAmount < m_maxPopulationSize)
	{
		newPopulation.emplace_back(std::make_unique<Genotype>(&m_innovation, m_countOfInputs, m_countOfOutputs, m_currentGenotypeId++, m_activationFunction));
		currentSpawnedAmount++;
	}

	m_population = std::move(newPopulation);
}

std::unique_ptr<nev::Genotype> nev::NEAT::getHighestRawFitnessGenotype() const
{
	double highestRawFitness = -DBL_MAX; // DBL_MIN is smallest positive DBL -> therefore use -DBL_MAX
	const Genotype* highestRawFitnessGeno = nullptr;

	for (const auto& genotype : m_population)
	{
		if (genotype->getRawFitness() > highestRawFitness)
		{
			highestRawFitness = genotype->getRawFitness();
			highestRawFitnessGeno = genotype.get();
		}
	}

	assert(highestRawFitness);
	if (!highestRawFitnessGeno)
		return nullptr;

	return std::make_unique<nev::Genotype>(*highestRawFitnessGeno);
}

void nev::NEAT::updateFitnessValues()
{
	for (auto& species : m_species)
		species.updateFitnessValues();
}

void nev::NEAT::calculateSpawnAmoutOfSpecies()
{
	double populationAverageFitness = 0;
	double populationTotalFitness = 0;

	for (const auto& genotype : m_population)
		populationTotalFitness += genotype->getAdjustedFitness();

	populationAverageFitness = populationTotalFitness / m_population.size();

	for (auto& species : m_species)
		species.calculateSpawnAmount(populationAverageFitness);
}
