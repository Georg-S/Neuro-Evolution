#include "HNE/HistoricalGenotype.h"

nev::HistoricalGenotype::HistoricalGenotype(Innovation* inno, int countOfInputs, int countOfOutputs)
	: m_countOfInputs(countOfInputs)
	, m_countOfOutputs(countOfOutputs)
{
	m_genotypeHistory.emplace_back(inno, countOfInputs, countOfOutputs, 0);
}

nev::HistoricalGenotype::HistoricalGenotype(Genotype genotype, int countOfInputs, int countOfOutputs)
	: m_countOfInputs(countOfInputs)
	, m_countOfOutputs(countOfOutputs)
{
	m_genotypeHistory.emplace_back(std::move(genotype));
}

std::vector<double> nev::HistoricalGenotype::calculateOutputSnapshotFromLastGenotype(const std::vector<double>& input)
{
	assert(!m_genotypeHistory.empty());
	return m_genotypeHistory.back().calculateOutputSnapshot(input);
}

std::vector<double> nev::HistoricalGenotype::calculateOutputActiveFromLastGenotype(const std::vector<double>& input)
{
	assert(!m_genotypeHistory.empty());
	return m_genotypeHistory.back().calculateOutputActive(input);
}

void nev::HistoricalGenotype::setFitness(const double& fitness)
{
	if (m_genotypeHistory.empty())
		return;

	m_genotypeHistory.back().setRawFitness(fitness);
}

void nev::HistoricalGenotype::iterate(Innovation* inno)
{
	Genotype currentGeno = m_genotypeHistory.back();
	currentGeno.setRawFitness(0);
	mutate(inno, currentGeno);
	currentGeno.deletePhenotype();
	m_genotypeHistory.emplace_back(std::move(currentGeno));
}

void nev::HistoricalGenotype::evolution(Innovation* inno)
{
	if (improvedEnough())
		purgeAllExceptHighestPerformingGenotype(inno);
	else
		reset(inno);
}

void nev::HistoricalGenotype::mutate(Innovation* inno, Genotype& genotype)
{
	genotype.randomlyAddLink(inno, m_addLinkProbability, m_recurrentAllowed);
	genotype.randomlyAddNeuron(inno, m_addNeuronProbability);
	genotype.randomlyMutateAllWeights(m_mutateWeightProbability, m_newWeightProbability, m_maxWeightPertubation);
}

void nev::HistoricalGenotype::deletePhenotype()
{
	for (auto& genotype : m_genotypeHistory)
		genotype.deletePhenotype();
}

void nev::HistoricalGenotype::purgeAllExceptHighestPerformingGenotype(Innovation* inno)
{
	m_highestFitnessAtStartOfCentury = getHighestFitness();
	Genotype topPerformer = getHighestPerformingGenotype(inno);
	topPerformer.deletePhenotype();
	m_genotypeHistory.clear();
	m_genotypeHistory.emplace_back(std::move(topPerformer));
}

bool nev::HistoricalGenotype::improvedEnough() const
{
	double fitnessNeeded = m_highestFitnessAtStartOfCentury + (m_highestFitnessAtStartOfCentury * m_improvementNeeded);
	if (fitnessNeeded <= getHighestFitness())
		return true;

	return false;
}

void nev::HistoricalGenotype::reset(Innovation* inno)
{
	m_genotypeHistory.clear();
	m_highestFitnessAtStartOfCentury = 0;
	m_genotypeHistory.emplace_back(inno, m_countOfInputs, m_countOfOutputs, 0);
}

int nev::HistoricalGenotype::getHistorySize() const
{
	return m_genotypeHistory.size();
}

double nev::HistoricalGenotype::getHighestFitness() const
{
	double highestFitness = -DBL_MAX;
	for (const auto& genotype : m_genotypeHistory)
		highestFitness = std::max(highestFitness, genotype.getRawFitness());

	return highestFitness;
}

double nev::HistoricalGenotype::getHighestFitnessAtStartOfCentury() const
{
	return m_highestFitnessAtStartOfCentury;
}

void nev::HistoricalGenotype::setImprovementNeeded(double improvementNeeded)
{
	m_improvementNeeded = improvementNeeded;
}

void nev::HistoricalGenotype::setHighestFitnessAtStartOfCentury(double highestFitnessAtStartOfCentury)
{
	m_highestFitnessAtStartOfCentury = highestFitnessAtStartOfCentury;
}

nev::Genotype nev::HistoricalGenotype::getHighestPerformingGenotype(Innovation* inno)
{
	std::sort(m_genotypeHistory.begin(), m_genotypeHistory.end());

	if (m_genotypeHistory.empty())
		return Genotype(inno, m_countOfInputs, m_countOfOutputs, 0);

	return m_genotypeHistory.front();
}

void nev::HistoricalGenotype::setAddLinkProbability(double addLinkProbability)
{
	m_addLinkProbability = addLinkProbability;
}

void nev::HistoricalGenotype::setAddNeuronProbability(double addNeuronProbability)
{
	m_addNeuronProbability = addNeuronProbability;
}

void nev::HistoricalGenotype::setMutateWeightProbability(double mutateWeightProbability)
{
	m_mutateWeightProbability = mutateWeightProbability;
}

void nev::HistoricalGenotype::setNewWeightProbability(double newWeightProbability)
{
	m_newWeightProbability = newWeightProbability;
}

void nev::HistoricalGenotype::setMaxWeightPertubation(double maxWeightPertubation)
{
	m_maxWeightPertubation = maxWeightPertubation;
}

void nev::HistoricalGenotype::setRecurrentAllowed(bool recurrentAllowed)
{
	m_recurrentAllowed = recurrentAllowed;
}

bool nev::operator<(const HistoricalGenotype& lhs, const HistoricalGenotype& rhs)
{
	return lhs.getHighestFitness() > rhs.getHighestFitness();
}
